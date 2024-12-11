#include <stdlib.h>
#include <unistd.h>

#include "help.h"

int main(int argc, char *argv[]) {
    //open a TCP socket to listen on
	int sfd = open_sfd(argv[1]);

    //wait indefinitely for clients to send a request
	while (1) {
		//TODO: Accept a client using accept_client(int sfd) in help.c
        
        //TODO: Fork a process to handle the client
        
            //TODO: Handle the client using handle_client(int sfd) in help.c

	}
	return 0;
}