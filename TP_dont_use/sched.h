#ifndef SCHED_H
#define SCHED_H
#define NULL 0
#define STACK_SIZE 1280


// Variables globales a tous les scheduler
//****************************************

typedef void (*func_t) ( void*);

typedef enum ProcessState {

    wait = 0,
    ready = 1,
    running = 2,
    terminated = 3,
    created = 4

} ProcessState;

struct pcb_s {

    unsigned int pid;
    ProcessState state;

    unsigned int co;
    unsigned int* sp;
    char *startStack;

    void* argsPointer;
    func_t f;

    struct pcb_s * next;

};

struct ctx_s {
	
    unsigned int varSP;
    unsigned int varPC;
	
};

unsigned int pid;
struct ctx_s* current_ctx;
struct pcb_s * current_process;

//**************** fin variables globales



//Fonctions globales necessaire à l'initialisation d'un scheduler quelconque
//*************************************************************************

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size);
void __attribute__ ((naked)) switch_to(struct ctx_s* ctx);
void __attribute__ ((naked)) ctx_switch();
void __attribute__ ((naked)) ctx_switch_from_irq();


void init_pcb(struct pcb_s* pcb, func_t f, void* args ,unsigned int stack_size);
void create_process(func_t f, void* args ,unsigned int stack_size);

// ***************************************************fin fonctions globales
void start_sched();


// fonction instancié en fonction du scheduler choisi
void elect();

#endif
