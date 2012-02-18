/*
 ============================================================================
 Name        : pipe_robin.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "balancer/balancer.h"
#include "pipe/pipe_util.h"
#include <sys/time.h>

void work(const void* elem_in, size_t count, pipe_producer_t* elem_out,
		void* aux) {
	printf("hello %d\n", elem_in);
}
int ring_count(plnode *head) {
	int count = 0;
	plnode *node = head;
	while (head != node->next) {
		count++;
		node = node->next;
	}
	count++;
	return count;
}

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	puts("START");
	int i;
	int elms = 100;
	plnode *head = create_worker(5, sizeof(int), work, NULL);
	printf("%d\n",ring_count(head));
	add_worker(head, 5, sizeof(int), work, NULL);
	printf("%d\n",ring_count(head));
	push_roundrobin(head, NULL);
	for (i = 0; i < 5; ++i) {
		pipe_push(head->in, &elms, 0);
		head = head->next;
	}
	for (i = 0; i < 20; ++i) {
		push_roundrobin(NULL, &elms);
		struct timespec ts = { .tv_sec = 0, .tv_nsec = 500000000 };
		nanosleep(&ts, 0);
	}
	rem_worker(head, 2);
//	free_all(head);
	printf("DONE!\n");
	while (1) {
		sleep(10);
	}
}
