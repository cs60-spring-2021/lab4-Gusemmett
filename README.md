# CS60 Spring 2021, Lab 4
## Your name
## GitHub username:

*Please add your README content.* 

## Overlay Process 

To compile the overlay, simply run 'make'. 

To start the overlay process, type './overlay&' to start it as a background process so that you can run other foreground process in the same terminal window. Wait until seeing printed message *'Overlay: waiting for connection from the network process...'*. 

Make sure to run overlay on all machines involved in the given topology. 

To stop the overlay process, first find out the process ID of the overlay via 'ps' command, then send a SIGINT signal via 'kill -2 PID'. We will need to do so on all involved machines. 

Please configure the OVERLAY_PORT (defined in overlay.h) and CONNECTION_PORT (defined in overlay.c) carefully to avoid potential conflict with others. 



