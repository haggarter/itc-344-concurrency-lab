#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "../helper_code/help.h"


void reap_children(int sig) {
    //Suppress unused parameter warning
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char *argv[]) {
    //Set up a signal handler for SIGCHLD to reap children
    struct sigaction sa;
    sa.sa_handler = reap_children;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
    
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