#include "priority_scheduler.h"
#include "sched.h"



void elect_priority(){

    // STEP 0 : ON VERIFIE L'ETAT DU PROCESSUS COURANT QUI VA ETRE SWITCHE
    // S'IL EST A L'ETAT TERMINATED -> APPEL A REMOVE PCB
    if((*current_process).state == terminated){
        remove_process_priority();
    }


    //STEP 1 : recupération du premier process dans notre queue
    //*****

    struct pcb_s * process; // notre curseur
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


void init_pcb_priority(struct pcb_s* pcb, func_t f, void* args ,unsigned int stack_size, unsigned int prio){

    // STEP 1 : initialisation de la pcb lors de la creation d'un processus

    pcb->pid   = pid++;
    pcb->state = ready;

    pcb->priority = prio;

    pcb->co = (int) start_current_process; // l'adresse de start_current_process est stockee dans co (program counter)
    pcb->f     = f;
    pcb->argsPointer = args; //argument de la fonction f
    pcb->startStack = (char*) phyAlloc_alloc(stack_size);

    //On decale le pointeur de pile sp a cause du pop.

    pcb->sp = (unsigned int*) (pcb->startStack + stack_size);
    pcb->sp --;

    *(pcb->sp) = 0x13;
    pcb->sp--;
    *(pcb->sp) = (unsigned int) &start_current_process;

    pcb->sp -= 14;


    // STEP 2 : liste chainee facon FIFO

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


void remove_process_priority() {

     //case 1 : remove from the top
     if( current_process == head ) {

        //mise a jour de head
        head = head->next;

        //desallocation
        phyAlloc_free(current_process->startStack, STACK_SIZE);
        phyAlloc_free(current_process, sizeof(struct pcb_s)); //desallocation de pcb_init

        //changement de processus courant
        //retour a elect
     }

     //case 2 : remove from bottom
     else if(current_process == tail) {
        //mise a jour de tail
        struct pcb_s * previous;
        previous = head ;

        // parcours de la liste pour trouver le précédent de tail
        while(previous->next != tail){
            previous = previous->next;
        }
        tail = previous;

        //desallocation
        phyAlloc_free(current_process->startStack, STACK_SIZE);
        phyAlloc_free(current_process, sizeof(struct pcb_s)); //desallocation de pcb_init

        //changement de processus courant
        //retour a elect
     }

     //case 3 : remove from the center
     else{
        struct pcb_s * previous;
        previous = head ;

        // parcours de la liste pour trouver le précédent de tail
        while(previous->next != current_process){
            previous = previous->next;
        }

        previous->next = current_process->next;

        //desallocation
        phyAlloc_free(current_process->startStack, STACK_SIZE);
        phyAlloc_free(current_process, sizeof(struct pcb_s)); //desallocation de pcb_init

        //changement de processus courant
        //retour a elect
     }
}
