#include "FCFS_scheduler.h"


void elect(){

    //STEP 1 : recupération du premier process dans notre queue
    //*****

    struct pcb_s * process;
    process = head;

    while( (*process).state!= ready){
          if(process->next == NULL){    // si on a parcourut toute la liste on retourne à la tete de pile
            process = head;
          }
         process = process->next;
    }

    //STEP 2 : redirection du pointeur current_process vers le process récupéré
    //*****

    current_process = process;

    (*current_process).state = running;  // on passe le processus a l'etat running

}


void init_pcb(struct pcb_s* pcb, func_t f, void* args ,unsigned int stack_size){

    // STEP 1 : initialisation de la pcb lors de la création d'un processus

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


    // STEP 2 : liste chainée façon FIFO

    if(pcb->pid == 0 ){
        //etape initiale premier process cree
        head = pcb;
        pcb->next = NULL;

    }else{
        // on ajoute le process a la fin
        tail->next = pcb;

    }

    //et on met a jour tail
    tail = pcb;
}
