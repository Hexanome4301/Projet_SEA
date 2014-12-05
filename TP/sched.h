#ifndef SCHED_H
#define SCHED_H
#define NULL 0
#define STACK_SIZE 1280

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

struct ctx_s* current_ctx;

unsigned int pid;
//fonctions

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size);
void __attribute__ ((naked)) switch_to(struct ctx_s* ctx);


//partie 4

struct pcb_s * head;
struct pcb_s * tail;

struct pcb_s * current_process;

void init_pcb(struct pcb_s* pcb, func_t f, void* args ,unsigned int stack_size);
void create_process(func_t f, void* args ,unsigned int stack_size);
void elect();
void __attribute__ ((naked)) ctx_switch();
void start_sched();
void __attribute__ ((naked)) ctx_switch_from_irq();


#endif
