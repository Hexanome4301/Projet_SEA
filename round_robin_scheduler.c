#include "round_robin_scheduler.h"
#include "sched.h"
#include "hw.h"
#include "phyAlloc.h"

void elect_round_robin(){

    struct pcb_s * process;
    process = current_process;

    // On cherche un process qui soit prêt

    while (process->next->state == terminated) {
         remove_process_round_robin();
    }

    while((process->next)->state != ready){
        process = process->next;
    }
    current_process = process->next;

}

void remove_process_round_robin() {

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



void init_pcb_round_robin(struct pcb_s* pcb, func_t f, void* args ,unsigned int stack_size){

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
