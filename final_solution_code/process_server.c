#include <stdlib.h>
#include <unistd.h>

#include "../helper_code/help.h"

int main(int argc, char *argv[]) {
    //open a TCP socket to listen on
	int sfd = open_sfd(argv[1]);

    //wait indefinitely for clients to send a request
	while (1) {
		int c_sfd = accept_client(sfd);
        
        //fork a process to handle the new client
        if (fork() == 0) {
            //handle the client
            handle_client(c_sfd);
            exit(0);
        }
	}
	return 0;
}