/*
 * balancer.h
 *
 *  Created on: Feb 17, 2012
 *      Author: Au
 */

#ifndef BALANCER_H_
#define BALANCER_H_

#include "../pipe/pipe_util.h"

typedef struct plnode {
	pipe_producer_t* in;
	pipe_consumer_t* out;
	struct plnode* next;
} plnode;

plnode *create_worker(int size, int elms_size, pipe_processor_t worker,
		void *aux);
plnode *add_worker(plnode *oldring, int size, int elms_size,
		pipe_processor_t worker, void *aux);
plnode *rem_worker(plnode *node, int num);
void free_all(plnode *node);
void push_roundrobin(plnode *_node, void *elms);

#endif /* BALANCER_H_ */
