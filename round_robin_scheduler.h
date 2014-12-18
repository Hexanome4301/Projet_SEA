#ifndef ROUND_ROBIN_SCHEDULER_H
#define ROUND_ROBIN_SCHEDULER_H
#define NULL 0

//partie 4
typedef void (*func_t) ( void*);
struct pcb_s * head;
struct pcb_s * tail;

void elect_round_robin();

void init_pcb_round_robin(struct pcb_s* pcb, func_t f, void* args ,unsigned int stack_size);

void remove_process_round_robin();
#endif
