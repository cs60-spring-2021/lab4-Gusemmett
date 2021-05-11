/*
 * overlay/neighbortable.c: implementation of APIs for the neighbor 
 * table.  
 *
 * CS60, Spring 2021. 
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "neighbortable.h"
#include "topology.h"
#include "overlay.h"

/**************** functions ****************/

// initiate neighbor table
nbr_entry_t *nt_create() {
	/* allocate memory for neighbor table */
	int nbrNum = topology_getNbrNum(TOPO_FILE);
	nbr_entry_t *nt = (nbr_entry_t *)malloc(nbrNum * sizeof(nbr_entry_t));
	assert(nt != NULL);

	/* fill node ID into neighbor table */
	char hostname1[60];
	char hostname2[60];
	int cost;

	FILE *ifp = fopen(TOPO_FILE, "r");
	assert(ifp != NULL);

	struct hostent* host;
	int idx =0;
	while (fscanf(ifp, "%s %s %d", hostname1, hostname2, &cost) != EOF) {
		if (topology_getNodeIDfromname(hostname1) == topology_getMyNodeID()) {
			nt[idx].nodeID = topology_getNodeIDfromname(hostname2);
			host = gethostbyname(hostname2);
			assert(host != NULL);
			nt[idx].nodeIP = *((in_addr_t *) host->h_addr_list[0]);
			nt[idx].conn = -1;
			idx++;
		} else if (topology_getNodeIDfromname(hostname2) == topology_getMyNodeID()) {
			nt[idx].nodeID = topology_getNodeIDfromname(hostname1);
			host = gethostbyname(hostname1);
			assert(host != NULL);
			nt[idx].nodeIP = *((in_addr_t *) host->h_addr_list[0]);
			nt[idx].conn = -1;
			idx++;
		}
	}
	return nt;
}

// close all neighbor connections and free neighbor table 
void nt_destroy(nbr_entry_t *nt) {
	int nbrNum = topology_getNbrNum(TOPO_FILE);
	for (int i = 0; i < nbrNum; i++) {
		if (nt[i].conn != -1) {
			close(nt[i].conn); // close the file descriptor/socket
		}
	}
	free(nt);
}


// add a socket connection to the neighbor table
int nt_addconn(nbr_entry_t *nt, int nodeID, int conn) {
	int nbrNum = topology_getNbrNum(TOPO_FILE);
	for (int i = 0; i < nbrNum; i++) {
		if (nt[i].nodeID == nodeID) {
			nt[i].conn = conn; // set the connection
			return 1;
		}
	}
	return -1;
}
