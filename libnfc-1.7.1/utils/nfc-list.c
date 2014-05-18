/**
# Rebar Design 2014 Custom NFC Device Scanner 
 */


// Libraries 
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif // HAVE_CONFIG_H

#include <err.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include <nfc/nfc.h>

#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "nfc-utils.h"

// Number of NFC Readers - devices 
#define MAX_DEVICE_COUNT 6

// Max number of tags a reader can scan at a time
#define MAX_TARGET_COUNT 2 // if 1 change line 199

// Server and socket parameters
int client_socket;
struct sockaddr_in servaddr,cliaddr;
struct hostent *server;

//Connection to server 
void init_socket(char *serverName)
{
        client_socket=socket(AF_INET,SOCK_STREAM,0);

        if (client_socket< 0){
                perror("ERROR: opening socket\n");
                exit(0);
        }

        server = gethostbyname(serverName);
        if (server == NULL) {
                perror("ERROR: no such host\n");
                exit(0);
        }

        memset(&servaddr,0,sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr=inet_addr(server->h_name);
        servaddr.sin_port=htons(8124);
        if(connect(client_socket, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        {
                fprintf(stderr, "Cannot connect to socket\n");
                close(client_socket);
                exit(1);
        }

}

int
main(int argc, const char *argv[])
{
    int fd[MAX_DEVICE_COUNT];
    char readbuffer[1000];	
    // Define pipe for fork
    pipe(fd);

    (void) argc;
  	const char *acLibnfcVersion;

  	size_t  i;  	
	int res = 0;
	bool verbose = false;

	// Define library and initialize
	nfc_context *context;
    nfc_init(&context);
    if (context == NULL) {
     ERR("Unable to init library");
     exit(EXIT_FAILURE);
    }

    // Display libnfc version
	acLibnfcVersion = nfc_version();
	printf("%s uses library %s\n", argv[0], acLibnfcVersion);

	// Debug mode
	#ifdef HAVE_LIBUSB
	#  ifdef DEBUG
	  usb_set_debug(4);
	#  endif
	#endif


	/* If specific device is wanted, i.e. an ARYGON device on /dev/ttyUSB0 
	#if 0
	  nfc_device_desc_t ndd;
	  ndd.pcDriver = "ARYGON";
	  ndd.pcPort = "/dev/ttyUSB0";
	  ndd.uiSpeed = 115200;
	  pnd = nfc_open(context, &ndd);
	#endif

	  /* If specific device is wanted, i.e. a SCL3711 on USB 
	#if 0
	  nfc_device_desc_t ndd;
	  ndd.pcDriver = "PN533_USB";
	  strcpy(ndd.acDevice, "SCM Micro / SCL3711-NFC&RW");
	  pnd = nfc_open(context, &ndd);
	#endif
	 */

	// Check how many devices have information pending ( connected ) 
	nfc_connstring connstrings[MAX_DEVICE_COUNT];

	// Store size of devices 
	size_t szDeviceFound = nfc_list_devices(context, connstrings, MAX_DEVICE_COUNT);

	// Error if no device sends information
	if (szDeviceFound == 0) {
    printf("No Volt Reader found.\n");
  	}

  	// Show how many readers found

  	if (szDeviceFound == 1 ) {
        printf("%d Volt Reader found .\n" , szDeviceFound);
    }
    else if (szDeviceFound > 1){
        printf("%d Volt Readers found .\n" , szDeviceFound);
    }

    // Set the modulation parameters for 0xff 0x1 | and ISO14443A targets
    nfc_modulation nm;
        nm.nmt = NMT_ISO14443A;
        nm.nbr = NBR_106; 

    // Define children
    pid_t  childpid;

    // Create fork for each Volt Reader
    for (i = 0; i < szDeviceFound; i++) {

    	// see if fork is child        
    	if ((childpid = fork()) == 0){  

    		// Assign NFC Device properties to detected Volt Reader 
    		nfc_device *pnd;

    		// Close fork ? 
            close(fd[0]);
            dup2(fd[1], 1);

            //Create an array with information from all tags
            nfc_target ant[MAX_TARGET_COUNT];

            // Open device with library parameters and buffered information 
            pnd = nfc_open(context, connstrings[i]);
            
            // Error if can't open device
            if (pnd == NULL) {
		      ERR("Unable to open NFC device: %s", connstrings[i]);
		      continue;
		    }
		    fflush(stdout);

		    //Reader as initializer 
		    if (nfc_initiator_init(pnd) < 0) {
		      nfc_perror(pnd, "nfc_initiator_init");
		      nfc_exit(context); // Deinitialize libnfc. Should be called after closing all open devices and before your application terminates.
		      exit(EXIT_FAILURE);
		    }
		    
	 		
	 		printf("Volt Reader: %s opened\n", nfc_device_get_name(pnd));

	 		// Variable for previously read tag info
	 		nfc_target pant[MAX_TARGET_COUNT];
		
			// Checking for tags loop. Loop continues after tag information is printed
			while(1){

				// Initiate device 
				nfc_initiator_init(pnd);

				// Get number of tags on reader 
				res = nfc_initiator_list_passive_targets(pnd, nm, ant, MAX_TARGET_COUNT);

				int n;
				// Loop through number of targets
				do {
                	for (n = 0; n < res; n++) {
	                	// Check if previous info in memory is similar to new one
	                	if( memcmp((void*)&pant[n], (void*)&ant[n], sizeof(ant[n])) != 0){
		                	printf("Reader number: %d\n",i);
			                print_nfc_target(&ant[n], verbose);
			                printf("\n");
			                memcpy((void*)&pant[n], (void*)&ant[n], sizeof(ant[n]));
			                fflush(stdout); // flush memory
	                	}
	            	}
	            // End loop when there is a target detected. Loop while no targets detected.
	            } while (nfc_initiator_list_passive_targets(pnd, nm, ant, MAX_TARGET_COUNT) == 0);
	            i++; continue;
	        }// end of tag read loop

	    //outside fork close
	    }
	    // if Parent
	    else { 
	    	init_socket("127.0.0.1");
            memset(readbuffer, 0, sizeof(readbuffer));
            sprintf(readbuffer, "Fork: %d\n", childpid);
            write(client_socket, readbuffer, strlen(readbuffer)); 
        }

	} // end of child script

	// Close fork ? 
	close(fd[1]);
    int nbytes;

    // Loop sending received info to server 
    while(1){
	    memset(readbuffer, 0, sizeof(readbuffer));
	    // Read in a string from the pipe
	    nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
	    if(nbytes > 0){
	    	write(client_socket, readbuffer, strlen(readbuffer));
	    }
    }//end server loop
}
