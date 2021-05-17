/*
 * overlay/overlay.c: implementation of an overlay process, which 
 * sets up TCP connections given a network topology. It has following
 * steps: 
 * 1. connect to all neighbors
 * 2. start listen_to_neighbor threads, each of which keeps 
 *    receiving packets from a neighbor and forwards the received 
 * 		packets to a network process (your program in lab4).
 * 3. waits for the connection from a network process (your 
 * 		program). Once connected, it monitors packets from the 
 * 		network process and sends the received packets to neighbors.
 *
 * CS60, Spring 2021. 
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <sys/utsname.h>
#include <assert.h>

#include "topology.h"
#include "overlay.h"
#include "neighbortable.h"

/**************** local constants ****************/
// start the overlay processes on all the overlay hosts within this period of time
#define OVERLAY_START_DELAY 30

// the port number used for nodes to interconnect each other to form an overlay, you should change this to a random value to avoid conflicts with other students
#define CONNECTION_PORT 60562


/**************** global variables ****************/
nbr_entry_t* nt; // neighbor table
int network_conn; // TCP connection

/**************** local function prototypes ****************/
void* waitNbrs(void* arg); 
int connectNbrs(); 
void* listen_to_neighbor(void* arg); 
void waitNetwork(); 
void overlay_stop(); 

/**************** main function ****************/
int main() {
	/* start overlay initialization */
	printf("Overlay: Node %d initializing...\n", topology_getMyNodeID());

	/* create a neighbor table */
	nt = nt_create();
	/* initialize network_conn to -1, means no network process is connected yet */
	network_conn = -1;

	/* register a signal handler which is sued to terminate the process */
	signal(SIGINT, overlay_stop);

	/* print out all the neighbors */
	int nbrNum = topology_getNbrNum(TOPO_FILE);
	for (int i = 0; i < nbrNum; i++) {
		printf("Overlay: neighbor %d:%d\n", i + 1, nt[i].nodeID);
	}

	/* start the waitNbrs thread to wait for incoming connections from neighbors with larger node IDs */
	pthread_t waitNbrs_thread;
	pthread_create(&waitNbrs_thread, NULL, waitNbrs, (void *) 0);

	/* wait for other nodes to start */
	sleep(OVERLAY_START_DELAY);

	/* connect to neighbors with smaller node IDs */
	connectNbrs();

	/* wait for waitNbrs thread to return */
	pthread_join(waitNbrs_thread, NULL);

	/* at this point, all connections to the neighbors are created */

	/* create threads listening to all the neighbors */
	for (int i = 0; i < nbrNum; i++) {
		int *idx = (int *) malloc(sizeof(int));
		*idx = i;
		pthread_t nbr_listen_thread;
		pthread_create(&nbr_listen_thread, NULL, listen_to_neighbor, (void *) idx);
	}
	printf("Overlay: node initialized...\n");
	printf("Overlay: waiting for connection from the network process...\n");

	/* waiting for connection from the network process */
	waitNetwork();
}

/**************** helper functions ****************/

// wait for my neighbors with IDs larger than me to connect
// 1) Get number of neighbors, nbrNum
// 2) Get my node ID, myNodeID
// 3) Count number of incoming neighbors, incoming_neighbors
// 4) Create socket to listen on at port CONNECTION_PORT
// 5) while(incoming_neighbors > 0)
//      accept connection on socket
//      get id of neighbor, nbrID
//      add nbrID and connection to neighbor table
// 6) close the listening socket
void* waitNbrs(void* arg) {
	int nbrNum = topology_getNbrNum(TOPO_FILE);
	int myNodeID = topology_getMyNodeID();

	/* if neighborID is larger than myID, the neighbor connects to me */
	int incoming_neighbors = 0;
	for (int i = 0; i < nbrNum; i++) {
		if (myNodeID < nt[i].nodeID)
			incoming_neighbors++;
	}

	// open a socket on CONNECTION_PORT, wait until all incoming neighbors are connected
	int serv_sd;
	struct sockaddr_in serv_addr;

	serv_sd = socket(AF_INET, SOCK_STREAM, 0);
	assert(serv_sd > 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(CONNECTION_PORT);
	assert(bind(serv_sd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0);
	assert(listen(serv_sd, incoming_neighbors) == 0);

	while (incoming_neighbors > 0) {
		int connection;
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = 999;
		connection = accept(serv_sd, (struct sockaddr *) &client_addr, &client_addr_len);
		int nbrID = topology_getNodeIDfromip(&client_addr.sin_addr);
		if (nt_addconn(nt, nbrID, connection) > 0) {
			printf("Overlay: got connection from Node %d\n", nbrID);
			incoming_neighbors--;
		}

	}
	printf("Overlay: all incoming connections received\n");
	close(serv_sd);
	pthread_exit(NULL);
}

// connecting to neighbors with ID smaller than my ID
// 1) nbrNum = get neighbor num
// 2) myNodeID = getMyNodeID
// 3) for each neighbor
//      if myNodeID > nt[i].nodeID
//        make a socket on CONNECTION_PORT and connect to it
//        add this neighbor to neighbor table with connection
// 4) return 1
int connectNbrs() {
	int nbrNum = topology_getNbrNum(TOPO_FILE);
	int myNodeID = topology_getMyNodeID();

	// connect to a neighbor with ID smaller than mine
	for (int i = 0; i < nbrNum; i++) {
		if (myNodeID > nt[i].nodeID) {
			int out_conn;
			struct sockaddr_in servaddr;

			servaddr.sin_family = AF_INET;
			memcpy(&servaddr.sin_addr.s_addr, &nt[i].nodeIP, sizeof(in_addr_t));
			servaddr.sin_port = htons(CONNECTION_PORT);

			out_conn = socket(AF_INET, SOCK_STREAM, 0);
			if (out_conn <= 0)
				return -1;
			if (connect(out_conn, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0)
				return -1;
			printf("Overlay: connected to Node %d\n", nt[i].nodeID);
			nt_addconn(nt, nt[i].nodeID, out_conn);
		}
	}
	printf("Overlay: all outgoing connections setup\n");
	return 1;
}

// listening thread for a given neighbor
void* listen_to_neighbor(void* arg) {
	//parse the arguments
	int *nbr_idx_ptr = (int *) arg;
	int nbr_idx = *nbr_idx_ptr;

	//free the neighbor index
	free(nbr_idx_ptr);

	//monitor packets from the given neighbor and forward them 
	char buf[MAX_PKT_LEN];
	int recv_num;
	memset(buf, 0, MAX_PKT_LEN);
	while ((recv_num = read(nt[nbr_idx].conn, buf, MAX_PKT_LEN)) > 0) {
		write(network_conn, buf, recv_num);
	}

	/* close neighbor */
	close(nt[nbr_idx].conn);
	nt[nbr_idx].conn = -1;

	pthread_exit(NULL);
}

// wait for local network process to connect. Once connected, it 
// monitors packet from the network process and sends it to 
// its neighbors
void waitNetwork() {
	int serv_sd;
	struct sockaddr_in serv_addr;
	assert(serv_sd = socket(AF_INET, SOCK_STREAM, 0));
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(OVERLAY_PORT);
	assert(bind(serv_sd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0);
	assert(listen(serv_sd, 1) == 0);

	while (1) {
		struct sockaddr_in client_addr;
		socklen_t client_addr_len;
		network_conn = accept(serv_sd, (struct sockaddr *) &client_addr, &client_addr_len);

		// monitor packets from the network process and send them to neighbors
		char buf[MAX_PKT_LEN];
		int recv_num;
		memset(buf, 0, MAX_PKT_LEN);
		while ((recv_num = read(network_conn, buf, MAX_PKT_LEN)) > 0) {
			int nbrNum = topology_getNbrNum(TOPO_FILE);
			int i;
			for (i = 0; i < nbrNum; i++) {
				if (nt[i].conn >= 0)
					write(nt[i].conn, buf, recv_num);
			}
		}
		close(network_conn);
		network_conn = -1;
	}
}

// stop the overlay process
void overlay_stop() {
	if (network_conn != -1)
		close(network_conn);
	nt_destroy(nt);
	printf("Overlay: stopped..\n");
	exit(0);
}


