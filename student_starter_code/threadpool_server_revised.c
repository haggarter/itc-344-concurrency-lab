#include <pthread.h>
#include <semaphore.h>

#include "../helper_code/help.h"

#define NUM_THREADS 8
#define QUEUE_SIZE 5

int queue[QUEUE_SIZE];
int front;

sem_t mutex;
sem_t available_slot;
sem_t client_pending;

void* consumer(void* ptr);

int main(int argc, char *argv[]) {
    //TODO: Open a socket to receive client connections using open_sfd(char* port) in help.c

    //define the starting point of the queue
	front = 0;

    //TODO: initialize semaphores

    //TODO: create an array of thread IDs

    //TODO: use pthread_create to spawn NUM_THREADS threads and add their thread IDs to the ID array

    //wait indefinitely for clients to send a request
	while (1) {
		//TODO: Accept a client using accept_client(int sfd) in help.c

		//TODO: Implement semaphores to solve the consumer-producer problem

        //TODO: add the client to the front of the queue

		//TODO: Implement semaphores to solve the consumer-producer problem
		
	}
	return 0;
}

void* consumer(void* ptr) {
    //run as a detached thread
	pthread_detach(pthread_self());
    //wait indefinitely for a pending client
	while (1) {
		int sfd;

		//TODO: Implement semaphores to solve the consumer-producer problem

        //TODO: grab the client socket file descriptor from the front of the queue
		
        //check if the queue is at the end, if so wrap back to the beginning of the queue
		if (++front == QUEUE_SIZE)
			front = 0;
		
		//TODO: Implement semaphores to solve the consumer-producer problem
		
		//TODO: Handle the client using handle_client(int sfd) in help.c

	}
	pthread_exit(0);
}
