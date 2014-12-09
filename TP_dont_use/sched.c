#include "phyAlloc.h"
#include "hw.h"
#include "sched.h"


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



void init_pcb(struct pcb_s* pcb, func_t f, void* args ,unsigned int stack_size){

    //initialisation de la pcb lors de la création d'un processus

    pcb->pid   = pid++;
    pcb->state = ready;
    pcb->co = (int) start_current_process; // l'adresse de start_current_process est stockée dans co (program counter)
    pcb->f     = f;
    pcb->argsPointer = args; //argument de la fonction f
    pcb->startStack = (char*) phyAlloc_alloc(stack_size);

    //On décale le pointeur de pile sp a cause du pop.

    pcb->sp = (unsigned int*) (pcb->startStack + stack_size);
    pcb->sp --;

    *(pcb->sp) = 0x13;
    pcb->sp--;
    *(pcb->sp) = (unsigned int) &start_current_process;

    pcb->sp -= 14;

    //liste chainée de l'ordonnanceur round robins
    if(pcb->pid == 0 ){
        head = pcb;
        pcb->next = pcb;
    }else{
        tail->next = pcb;
        pcb->next = head;
    }

    tail = pcb;
}

void start_sched() {

    head->state=ready;

    struct pcb_s* pcb_init = phyAlloc_alloc(sizeof(struct pcb_s));

    //init_pcb(&pcb_init,NULL,NULL,STACK_SIZE);

    (*pcb_init).pid=666;
    (*pcb_init).next = head;
    current_process = pcb_init;

    set_tick_and_enable_timer();
    ENABLE_IRQ();

    while(1) {
        ;
    }


}

void create_process(func_t f, void* args ,unsigned int stack_size)
{
   struct pcb_s* unPCB = phyAlloc_alloc(sizeof(struct pcb_s));
   init_pcb(unPCB, f, &args ,stack_size );

   // **************************************************************************
   // Appel à la méthode d'ajout instancié en fonction du bloc scheduler choisi
   // **************************************************************************

}

void __attribute__ ((naked)) ctx_switch() {
    __asm("sub lr, lr, #4");
    __asm("srsdb sp!, #0x13");

    __asm("push {r0-r12, lr}");
    __asm("mov %0, sp" : "=r"(current_process->sp));

    elect();

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

      elect();

      __asm("mov sp, %0" : : "r"(current_process->sp));

      set_tick_and_enable_timer();

      __asm("pop {r0-r12, lr}");

      ENABLE_IRQ();

      __asm("rfeia sp!");
}



