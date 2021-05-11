/*
 * topology/topology.c: implementation of functions to parse the 
 * topology file 
 *
 * CS60, Spring 2021. 
 */

#include "topology.h"

/**************** functions ****************/
// return node ID of the given hostname. 
// 1) get host structure from gethostbyname
// 2) get ip address using inet_ntoa(host->h_addr_list[0].sin_addr)
// 3) find index of last . of ip address, call it i
// 4) return atoi(ipaddress + i + 1) // + 1 to get character after .
int topology_getNodeIDfromname(char* hostname) {
	struct hostent *host;
	char *ip_address;
	host = gethostbyname(hostname);
#ifdef TEST
	//printf("getting node ID for host name %s\n", hostname);
#endif

	if (host != NULL) {
		struct sockaddr_in sin;
		memset(&sin, 0, sizeof(struct sockaddr_in)); 
		memcpy(&sin.sin_addr, host->h_addr_list[0], host->h_length);
		ip_address = inet_ntoa(sin.sin_addr);
#ifdef TEST
		//printf("IP address of this host is %s\n", ip_address);
#endif

		int dotnum = 3;
		int i = 0;
		while (dotnum) {
			if (ip_address[i] == '.') {
				dotnum--;
			}
			i++;
		}
		char *nodeID_addr = &ip_address[i];
		int nodeID = atoi(nodeID_addr);
		return nodeID;
	} else {
		return -1;
	}
}


// return the node ID based on an IP address.  
// 1) ipaddress = inet_ntoa(*addr)
// 2) find index of last . of ip adress, call it i
// 3) return atoi(ipaddress + i + 1) // + 1 to get character after .
int topology_getNodeIDfromip(struct in_addr* addr) {
	char *ip_address = inet_ntoa(*addr);

	if (ip_address != NULL) {
		int dotnum = 3;
		int i = 0;
		while (dotnum) {
			if (ip_address[i] == '.') {
				dotnum--;
			}
			i++;
		}
		char *nodeID_addr = &ip_address[i];
		int nodeID = atoi(nodeID_addr);
		return nodeID;
	} else {
		return -1;
	}
}


// return my node ID. 
// 1) get system information in ugnm using uname syscall
// 2) reutnr getNodeIDfromname(ugnm.nodename)
int topology_getMyNodeID() {
	struct utsname ugum;
	if (uname(&ugum) < 0) {
		return -1;
	} else {
		return topology_getNodeIDfromname(ugum.nodename);
	}
}


// parse the topology file and return the number of neighbors.
// 1) myNodeID = getMyNodeID(); nbrNum = 0
// 2) open topology file
// 3) Go through each line, get host1 and host2 names
//      if host1 == myNodeID || host2 == myNodeID
//        nbrNum++
// 4) close topology file
// 5) return nbrNum
int topology_getNbrNum(char* topoFile) {
	int myNodeID = topology_getMyNodeID();
	char hostname1[60];
	char hostname2[60];
	int cost;
	int nbrNum = 0;

	FILE *ifp = fopen(topoFile, "r");
	assert(ifp != NULL);

	while (fscanf(ifp, "%s %s %d",
	              hostname1, hostname2, &cost) != EOF) {
		if (topology_getNodeIDfromname(hostname1) == myNodeID
				|| topology_getNodeIDfromname(hostname2) == myNodeID) {
			nbrNum++;
		}
	}
	fclose(ifp);
	return nbrNum;
}


// parse the topology file and return the number of nodes in 
// the overlay network. 
int topology_getNodeNum(char* topoFile) {
	int nodes[MAX_NODE_NUM];
	char hostname1[60];
	char hostname2[60];
	int cost;
	int nodeNum = 0;

	FILE *ifp = fopen(topoFile, "r");
	assert(ifp != NULL);

	while (fscanf(ifp, "%s %s %d", hostname1, hostname2, &cost) != EOF) {
		/* add first node */
		int node1 = topology_getNodeIDfromname(hostname1);
		int toAdd = 1;
		for (int i = 0; i < nodeNum; i++) {
			if (nodes[i] == node1) {
				toAdd = 0;
				break;
			}
		}
		if (toAdd) {
			nodes[nodeNum] = node1;
			nodeNum++;
		}
		/* add second node */
		int node2 = topology_getNodeIDfromname(hostname2);
		toAdd = 1;
		for (int i = 0; i < nodeNum; i++) {
			if (nodes[i] == node2) {
				toAdd = 0;
				break;
			}
		}
		if (toAdd) {
			nodes[nodeNum] = node2;
			nodeNum++;
		}
	}
	fclose(ifp);
	return nodeNum;
}


// parse the topology file and return a dynamically allocated 
// array with all nodes' IDs in the overlay network. 
int* topology_getNodeArray(char* topoFile) {
	int nodes[MAX_NODE_NUM];
	char hostname1[60];
	char hostname2[60];
	int cost;
	int nodeNum = 0;

	FILE *ifp = fopen(topoFile, "r");
	assert(ifp != NULL);

	while (fscanf(ifp,"%s %s %d",hostname1,hostname2, &cost) != EOF) {
		/* add first node */
		int node1 = topology_getNodeIDfromname(hostname1);
		int toAdd1 = 1;
		for(int i = 0; i < nodeNum; i++) {
			if(nodes[i]==node1) {
				toAdd1 = 0;
				break;
			}
		}
		if(toAdd1) {
			nodes[nodeNum] = node1;
			nodeNum++;
		}
		/* add second node */
		int node2 = topology_getNodeIDfromname(hostname2);
		int toAdd2 = 1;
		for(int i = 0; i < nodeNum; i++) {
			if(nodes[i]==node2) {
				toAdd2 = 0;
				break;
			}
		}
		if(toAdd2) {
			nodes[nodeNum] = node2;
			nodeNum++;
		}
	}
	int* nodeArray = (int*)malloc(nodeNum*sizeof(int));
	assert(nodeArray!=NULL);
	for(int i = 0; i < nodeNum; i++) {
		nodeArray[i] = nodes[i];
	}
	fclose(ifp);
	return nodeArray;
}


// parse the topology file and return a dynamically allocated 
// array with all neighbors' IDs. 
int* topology_getNbrArray(char* topoFile) {
	int myNodeID = topology_getMyNodeID();
	int nodes[MAX_NODE_NUM];
	char hostname1[60];
	char hostname2[60];
	int cost;
	int nodeNum = 0;

	FILE *ifp = fopen(topoFile, "r");
	assert(ifp != NULL);

	while (fscanf(ifp, "%s %s %d", hostname1, hostname2, &cost) != EOF) {
		int node1 = topology_getNodeIDfromname(hostname1);
		int node2 = topology_getNodeIDfromname(hostname2);
		if (node1 == myNodeID) {
			nodes[nodeNum] = node2;
			nodeNum++;
		} else if (node2 == myNodeID) {
			nodes[nodeNum] = node1;
			nodeNum++;
		}
	}

	if (nodeNum == 0)
		return 0;

	int *nodeArray = (int *) malloc(nodeNum * sizeof(int));
	assert(nodeArray != NULL);
	for (int i = 0; i < nodeNum; i++) {
		nodeArray[i] = nodes[i];
	}
	fclose(ifp);
	return nodeArray;
}


// parse the topology information stored in the topology file 
// and return the link cost between two given nodes. 
unsigned int topology_getCost(int fromNodeID, int toNodeID, char* topoFile) {
	char hostname1[60];
	char hostname2[60];
	int cost;

	FILE *ifp = fopen(topoFile, "r");
	assert(ifp != NULL);

	if (fromNodeID == toNodeID)
		return 0;

	while (fscanf(ifp, "%s %s %d", hostname1, hostname2, &cost) != EOF) {
		if ((topology_getNodeIDfromname(hostname1) == fromNodeID
		        && topology_getNodeIDfromname(hostname2) == toNodeID)
		        || (topology_getNodeIDfromname(hostname1) == toNodeID
		        && topology_getNodeIDfromname(hostname2) == fromNodeID)) {
			fclose(ifp);
			return cost;
		}
	}
	fclose(ifp);
	return INFINITE_COST;
}
