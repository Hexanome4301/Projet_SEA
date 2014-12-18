#include "sched.h"
#include "FCFS_scheduler.h"
#include "phyAlloc.h"


void elect_fcfs(){

    // STEP 0 : ON VERIFIE L'ETAT DU PROCESSUS COURANT QUI VA ETRE SWITCHE
    // S'IL EST A L'ETAT TERMINATED -> APPEL A REMOVE PCB
    // TODO


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


void init_pcb_fcfs(struct pcb_s* pcb, func_t f, void* args ,unsigned int stack_size){

    // STEP 1 : initialisation de la pcb lors de la creation d'un processus

    pcb->pid   = pid++;
    pcb->state = ready;
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


void remove_process_fcfs() {

//    struct pcb_s * process;
//    struct pcb_s * previous;
//    process = current_process->next;

//    // On cherche un process qui soit prêt

//    while((process)->state != ready)  {

//        process = process->next;

//    }

//    if(head == current_process) {

//        //nouveau head = head.next

//        tail->next = head->next;
//        head = current_process->next;

//        }
//    else {

//        if(tail == current_process) {

//            previous = current_process;

//            //on parcourt la liste pour trouver tail.previous

//            while(previous->next != tail){

//                previous = previous->next;

//            }

//            //nouveau tail.next = head

//            previous->next = head;

//            //nouveau tail = tail.previous

//            tail = previous;

//        }
//        else {
//        }

//        previous = current_process;

//        //on parcourt la liste pour trouver tail.previous

//        while(previous->next != current_process){

//            previous = previous->next;

//        }

//        previous->next = current_process->next;

//        set_tick_and_enable_timer();
//        ENABLE_IRQ();

//    }

//    //desallocation
//    phyAlloc_free(current_process->startStack, STACK_SIZE);
//    phyAlloc_free(current_process, sizeof(struct pcb_s)); //desallocation de pcb_init

//    //changement de processus courant

//    current_process = process;
}
