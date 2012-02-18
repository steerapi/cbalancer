/*
 * balancer.c
 *
 *  Created on: Feb 17, 2012
 *      Author: Au
 */

#include "balancer.h"

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

plnode *create_ring(int size) {
	int i;
	plnode* head = malloc(sizeof(plnode));
	plnode* node = head;
	for (i = 0; i < size - 1; ++i) {
		node->next = malloc(sizeof(plnode));
		node = node->next;
	}
	node->next = head;
	return head;
}

plnode *create_worker(int size, int elms_size, pipe_processor_t worker,
		void *aux) {
	int i;
	plnode *head = create_ring(size);
	plnode *node = head;
	for (i = 0; i < size; ++i) {
		pipe_t* pipe = pipe_new(elms_size, 0);
		node->in = pipe_producer_new(pipe);
		node->out = pipe_consumer_new(pipe);
		pipe_free(pipe);
		pipe = pipe_new(elms_size, 0);
		pipe_connect(node->out, worker, aux, pipe_producer_new(pipe));
		pipe_free(pipe);
		node = node->next;
	}
	return head;
}

void merge_ring(plnode *oldring, plnode *newring) {
	plnode *old = oldring->next;
	oldring->next = newring;
	plnode *node = newring;
	while(node->next!=newring){
		node = node->next;
	}
	node->next = old;
}

plnode *add_worker(plnode *oldring, int size, int elms_size,
		pipe_processor_t worker, void *aux) {
	plnode *newring = create_worker(size, elms_size, worker, aux);
	merge_ring(oldring, newring);
	return newring;
}

void free_plnode(plnode *torem) {
	pipe_producer_free(torem->in);
	pipe_consumer_free(torem->out);
	free(torem);
}

plnode *rem_worker(plnode *node, int num) {
	int i;
	for (i = 0; i < num; ++i) {
		if (node->next == node) {
			free_plnode(node);
			return NULL;
		}
		plnode *torem = node->next;
		node->next = node->next->next;
		free_plnode(torem);
	}
	return node;
}

void free_all(plnode *node) {
	while (node->next != node) {
		plnode *torem = node->next;
		node->next = node->next->next;
		free_plnode(torem);
	}
	free_plnode(node);
}

void push_roundrobin(plnode *_node, void *elms) {
	static plnode *node;
	if (_node)
		node = _node;
	if (elms) {
		pipe_push(node->in, elms, 1);
		node = node->next;
	}
}
