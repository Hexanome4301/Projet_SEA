#include "phyAlloc.h"
#include "hw.h"
#include "sched.h"

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

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size)
{
	char* startStack ;
    ctx->varPC = (unsigned int) f ;
	startStack = (char*) phyAlloc_alloc(stack_size);	// On définit le nombre d'octet à allouer
    ctx->varSP = (int) (startStack+stack_size-4);	// On se déplace de stack_size pour ne pas taper dans les éléments
											// plus bas de la pile et on enleve 4 octets car qd on ajoute stack_size on dépasse de 1 case mémoire
}


void start_current_process() {

    current_process->f(current_process->argsPointer);

    //retour de l'appel de f = fin du processus

    current_process->state = terminated;

    //sortie du round robin

    struct pcb_s * process;
    struct pcb_s * previous;
    process = current_process->next;

    // On cherche un process qui soit prêt

    while((process)->state != ready)  {

        process = process->next;

    }

    if(head == current_process) {

        //nouveau head = head.next

        tail->next = head->next;
        head = current_process->next;

        }
    else {

        if(tail == current_process) {

            previous = current_process;

            //on parcourt la liste pour trouver tail.previous

            while(previous->next != tail){

                previous = previous->next;

            }

            //nouveau tail.next = head

            previous->next = head;

            //nouveau tail = tail.previous

            tail = previous;

        }
        else {
        }

        previous = current_process;

        //on parcourt la liste pour trouver tail.previous

        while(previous->next != current_process){

            previous = previous->next;

        }

        previous->next = current_process->next;

        set_tick_and_enable_timer();
        ENABLE_IRQ();

    }

    //desallocation
    phyAlloc_free(current_process->startStack, STACK_SIZE);
    phyAlloc_free(current_process, sizeof(struct pcb_s)); //desallocation de pcb_init

    //changement de processus courant

    current_process = process;
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

void create_process(func_t f, void* args ,unsigned int stack_size)
{
   struct pcb_s* unPCB = phyAlloc_alloc(sizeof(struct pcb_s));
   init_pcb(unPCB, f, &args ,stack_size );

}


void elect(){

    struct pcb_s * process;
    process = current_process;

    // On cherche un process qui soit prêt

    while (process->next->state == terminated) {
         /* gestion de la terminaison de process->next*/
    }

    while((process->next)->state != ready){
        process = process->next;
    }
    current_process = process->next;

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



