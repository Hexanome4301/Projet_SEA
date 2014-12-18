#include "round_robin_scheduler.h"
#include "phyAlloc.h"
#include "hw.h"
#include "sched.h"
#include "FCFS_scheduler.h"
#define ROUND_ROBIN_SCHEDULER 1
#define FCFS_SCHEDULER 2
#define CURRENT_SCHEDULER FCFS_SCHEDULER



void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size)
{
    char* startStack ;
    ctx->varPC = (unsigned int) f ;
    startStack = (char*) phyAlloc_alloc(stack_size);	// On définit le nombre d'octet à allouer
    ctx->varSP = (int) (startStack+stack_size-4);	   // On se déplace de stack_size pour ne pas taper dans les éléments
                                                        // plus bas de la pile et on enleve 4 octets car qd on ajoute stack_size on dépasse de 1 case mémoire
}

void __attribute__ ((naked)) switch_to(struct ctx_s* ctx) {

    // Sauvegarde du contexte courant

	__asm("push {r0-r12}"); // on s'arrete à r12 car on va enregistrer sp, lr dans current_ctx
    __asm("mov %0, sp" : "=r"(current_ctx->varSP));
	__asm("mov %0, lr" : "=r"(current_ctx->varPC)); 
	
	// Changement du contexte

	current_ctx = ctx;

	// Restauration du contexte
	// On recharge varSP et varPC dans sp et pc respectivement

	__asm("mov sp, %0" : : "r"(current_ctx->varSP));	
	__asm("mov lr, %0" : : "r"(current_ctx->varPC));
	__asm("pop {r0-r12}");
	__asm("bx lr") ; // on branche a l'adresse contenu dans lr
}

void start_sched() {

    //head->state=ready;

    struct pcb_s* pcb_init = phyAlloc_alloc(sizeof(struct pcb_s));

    //init_pcb(&pcb_init,NULL,NULL,STACK_SIZE);

    pcb_init->startStack = (char*) phyAlloc_alloc(STACK_SIZE);
    (*pcb_init).pid=666;
    (*pcb_init).next = head;

    current_process = pcb_init;

    set_tick_and_enable_timer();
    ENABLE_IRQ();

    while(1) {
        ;
    }
}

void create_process(func_t f, void* args ,unsigned int stack_size) {

   struct pcb_s* unPCB = phyAlloc_alloc(sizeof(struct pcb_s));

   // **************************************************************************
   // Appel à la méthode d'ajout instancié en fonction du bloc scheduler choisi
   // **************************************************************************
    switch(CURRENT_SCHEDULER) {
        case 1 :
            init_pcb_round_robin(unPCB, f, &args ,stack_size );
            break;

        case FCFS_SCHEDULER :
            init_pcb_fcfs(unPCB,f,&args , stack_size);
            break;

        default:
            init_pcb_round_robin(unPCB, f, &args ,stack_size );
    }
}

void start_current_process() {

    current_process->f(current_process->argsPointer);

    //retour de l'appel de f = fin du processus

    current_process->state = terminated;

    //sortie du round robin
    switch(CURRENT_SCHEDULER) {

        case ROUND_ROBIN_SCHEDULER  :
            remove_process_round_robin();
            break;
        case FCFS_SCHEDULER :
            remove_process_round_robin();
            break;
        default:
            remove_process_round_robin();
    }

}

void __attribute__ ((naked)) ctx_switch() {
    __asm("sub lr, lr, #4");
    __asm("srsdb sp!, #0x13");

    __asm("push {r0-r12, lr}");
    __asm("mov %0, sp" : "=r"(current_process->sp));
    switch(CURRENT_SCHEDULER) {

        case ROUND_ROBIN_SCHEDULER :
            elect_round_robin();
            break;
        case FCFS_SCHEDULER :
            elect_fcfs();
            break;
        default:
            elect_round_robin();
    }
    __asm("mov sp, %0" : : "r"(current_process->sp));
    __asm("pop {r0-r12, lr}");
    __asm("rfeia sp!");

}

void __attribute__ ((naked)) ctx_switch_from_irq() {

      DISABLE_IRQ();

      __asm("sub lr, lr, #4");
      __asm("srsdb sp!, #0x13");
      __asm("cps #0x13");

      __asm("push {r0-r12, lr}");
      __asm("mov %0, sp" : "=r"(current_process->sp));

      switch(CURRENT_SCHEDULER) {

      case ROUND_ROBIN_SCHEDULER :
          elect_round_robin();
          break;
      case FCFS_SCHEDULER :
          elect_fcfs();
          break;
      default:
          elect_round_robin();
      }

      __asm("mov sp, %0" : : "r"(current_process->sp));

      set_tick_and_enable_timer();

      __asm("pop {r0-r12, lr}");

      ENABLE_IRQ();

      __asm("rfeia sp!");
}



