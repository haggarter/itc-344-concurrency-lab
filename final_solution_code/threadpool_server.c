#include <pthread.h>
#include <semaphore.h>

#include "help.h"

#define NUM_THREADS 8
#define QUEUE_SIZE 5

int queue[QUEUE_SIZE];
int front;

sem_t mutex;
sem_t available_slot;
sem_t client_pending;

void* consumer();

int main(int argc, char *argv[]) {
    //open a TCP socket to listen on
	int sfd = open_sfd(argv[1]);

    //define the starting point of the queue
	front = 0;

    //initialize semaphores
	sem_init(&mutex, 0, 1);
	sem_init(&available_slot, 0, QUEUE_SIZE);
	sem_init(&client_pending, 0, 0);

    //create an array of thread IDs
	pthread_t thread_ids[NUM_THREADS];

    //spawn each thread and add it to the threadpool
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_create(&thread_ids[i], NULL, consumer, NULL);
	}

    //wait indefinitely for clients to send a request
	while (1) {
		int c_sfd = accept_client(sfd);
        //wait for a space in the queue to free up
		sem_wait(&available_slot);
		sem_wait(&mutex);
        //add the client to the front of the queue
		queue[front] = c_sfd;
		sem_post(&mutex);
        //let the threadpool know there are clients pending
		sem_post(&client_pending);
	}
	return 0;
}

void* consumer() {
    //run as a detached thread
	pthread_detach(pthread_self());
    //wait indefinitely for a pending client
	while (1) {
		int sfd;
		sem_wait(&client_pending);
		sem_wait(&mutex);
        //grab the socket file descriptor from the queue
		sfd = queue[front];
        //check if the queue is at the end, if so wrap back to the beginning of the queue
		if (++front == QUEUE_SIZE)
			front = 0;
		sem_post(&mutex);
        //let main thread know that there is a new slot available to service clients
		sem_post(&available_slot);
        //handle the client's request
		handle_client(sfd);
	}
	pthread_exit(0);
}