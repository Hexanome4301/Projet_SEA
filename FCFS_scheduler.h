#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H


//Fonctions

struct pcb_s * head;
struct pcb_s * tail;
void init_pcb_fcfs(struct pcb_s* pcb, func_t f, void* args ,unsigned int stack_size);
void elect_fcfs();
void remove_process_fcfs();

#endif
