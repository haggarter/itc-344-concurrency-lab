#include <stdlib.h>
#include <unistd.h>

#include "../helper_code/help.h"

int main(int argc, char *argv[]) {
    //TODO: Open a socket to receive client connections using open_sfd(char* port) in help.c

    //wait indefinitely for clients to send a request
	while (1) {
		//TODO: Accept a client using accept_client(int sfd) in help.c
        
        //TODO: Fork a process to handle the client
        
            //TODO: Handle the client using handle_client(int sfd) in help.c

	}
	return 0;
}