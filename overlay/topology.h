/*
 * topology/topology.h: definitions of functions to parse the 
 * topology file 
 *
 * CS60, Spring 2021.
 */

#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/utsname.h>
#include <assert.h>


/**************** constants ****************/
// max node number supported by the overlay network 
#define MAX_NODE_NUM 10
// infinite link cost value, if two nodes are disconnected, they will have link cost INFINITE_COST
#define INFINITE_COST 999
// topology file
#define TOPO_FILE "../topology.dat"

/**************** functions ****************/

/**
 * This function returns node ID of the given hostname. The node ID 
 * is an integer of the last 8 digits of the node's IP address. For 
 * example, a node with IP address 202.120.92.3 will have node ID 3. 
 *
 * @param hostname
 * Return -1 if the node ID can't be retrieved, otherwise 1
 */
int topology_getNodeIDfromname(char* hostname);

/**
 * This function returns the node ID based on an IP address. 
 *
 * Return -1 if my node ID can't be retrieved, otherwise 1
 */
int topology_getNodeIDfromip(struct in_addr* addr);

/**
 * This functions returns my node ID. 
 *
 * Return -1 if my node ID can't be retrieved, otherwise 1
 */
int topology_getMyNodeID();

/**
 * This functions parses the topology information stored in 
 * a given topology file
 *
 * Return the number of neighbors
 */
int topology_getNbrNum(char* topoFile);

/**
 * This functions parses the topology information stored in 
 * a given topology file
 *
 * Return the number of total nodes in the overlay
 */
int topology_getNodeNum(char* topoFile);

/**
 * This functions parses the topology information stored in 
 * a given topology file, and returns a dynamically allocated array 
 * which contains all the nodes' IDs in the overlay network. 
 *
 */
int* topology_getNodeArray(char* topoFile);

/**
 * This functions parses the topology information stored in 
 * a topology file and returns a dynamically allocated array which 
 * contains all the neighbors'IDs
 */
int* topology_getNbrArray(char* topoFile);

/**
 * This functions returns the link cost between two nodes defined in 
 * the given topology file
 *
 * Return the cost of the direct link between the two given nodes. 
 * Return INFINITE_COST if no direct link between the two nodes. 
 */
unsigned int topology_getCost(int fromNodeID, int toNodeID, char* topoFile);

#endif //TOPOLOGY_H
