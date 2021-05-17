# CS60 Spring 2021, Lab 4
###### Angus Emmett
###### GitHub username: gusemmett

## Main.py
### General 
This file is where most of the computation is done for this lab. First I start off by connecting the the local overlay process, and then I init the distance vector table (DVT) for the specific node. To do this I go through the topology.dat file and where ever it refers to a neighbour I add that cost to my DVT, otherwise I use a large number for unknown nodes.

Then I open 2 threads one for recieving DVTs and the other for sending them. This file sends a DVT to the overlay process evey five seconds, and the program will run for anywhere between __60-70__ seconds.

When I recieve a DVT from another node I update my local DVT and then recalculate the local DVT, with the Bellman-Ford algorithm.


### Functions
```py
class DVR():
	# Inits basic variables and starts connection
	def __init__(self, server, port, topology_file):
	# Recive Thread
	def recv(self):
	# Send Thread
	def send(self):
	# This function takes in a table and updates the values in the local DVT
	def update_table(self, node_table):
	# After the DVT has been updated this function recalculates the table
	def recalculate_table(self):
	# This function takes the path and returns the original DVT that just contains the neighbours of the node
	def init_dv(self, topology_file):
```

### Bonus
* Can handle changes in link costs on the fly
* Will not send DVTs to other nodes if the local DVT isnt updated


## Utils.py
### General
This file just handles formatting the DVTs. It has three functions all of which are quite self explainatory.
```py
def print_table(table):
def table_to_str(table):
def str_to_table(strn):
```

## Overlay Process 

To compile the overlay, simply run 'make'. 

To start the overlay process, type './overlay&' to start it as a background process so that you can run other foreground process in the same terminal window. Wait until seeing printed message *'Overlay: waiting for connection from the network process...'*. 

Make sure to run overlay on all machines involved in the given topology. 

To stop the overlay process, first find out the process ID of the overlay via 'ps' command, then send a SIGINT signal via 'kill -2 PID'. We will need to do so on all involved machines. 

Please configure the OVERLAY_PORT (defined in overlay.h) and CONNECTION_PORT (defined in overlay.c) carefully to avoid potential conflict with others. 