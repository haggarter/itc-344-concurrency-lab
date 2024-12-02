#include <pthread.h>

#include "../helper_code/help.h"

#define NUM_THREADS 8
#define QUEUE_SIZE 5

int queue[QUEUE_SIZE];
int front;

void* consumer();

int main(int argc, char *argv[]) {
    //open a TCP socket to listen on
	int sfd = open_sfd(argv[1]);

    //define the starting point of the queue
	front = 0;

    //create an array of thread IDs
	pthread_t thread_ids[NUM_THREADS];

    //spawn each thread and add it to the threadpool
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_create(&thread_ids[i], NULL, consumer, NULL);
	}

    //wait indefinitely for clients to send a request
	while (1) {
		int c_sfd = accept_client(sfd);
        //add the client to the front of the queue
		queue[front] = c_sfd;
	}
	return 0;
}

void* consumer() {
    //run as a detached thread
	pthread_detach(pthread_self());
    //wait indefinitely for a pending client
	while (1) {
		int sfd;
		sfd = queue[front];
        //check if the queue is at the end, if so wrap back to the beginning of the queue
		if (++front == QUEUE_SIZE)
			front = 0;
        //handle the client's request
		handle_client(sfd);
	}
	pthread_exit(0);
}