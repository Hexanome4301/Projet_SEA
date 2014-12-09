#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H


//Fonctions

struct pcb_s * head;
struct pcb_s * tail;
void init_pcb(struct pcb_s* pcb, func_t f, void* args ,unsigned int stack_size);
#endif
