/*
 * int
divide( int dividend, int divisor)
{
int result = 0;
int remainder = dividend;
while(remainder >= divisor) {
result++;
remainder -= divisor;
}

return result;

}

int compute_volume( int rad)
{
    int rad3 = rad * rad * rad;
    return divide(4*355*rad3, 3*113);
}
*/

#include "hw.h"
#include "sched.h"

struct ctx_s ctx_ping;
struct ctx_s ctx_pong;
struct ctx_s ctx_init;

/*
void ping() {

    while( 1 ) {
    switch_to(&ctx_pong);
    }
}

void pong() {

    while ( 1 ) {
    switch_to(&ctx_ping);
    }
}
*/

/*
void ping() {

int cpt = 0;
while ( 1 ) {
    cpt ++;
    switch_to(&ctx_pong);
    }
}

void pong() {
int
cpt = 1;
while ( 1 ) {
    cpt += 2 ;
    switch_to(&ctx_ping);
    }
}*/

void funcA()
{
    int cptA = 0;
    while ( 1 ) {
        cptA ++;
    }
}

void funcB()
{
    int cptB = 1;
    while ( 1 )   {
       cptB += 2 ;
    }

    cptB += 2;
}

void funcC()
{
    int cptC = 1;
    while ( 1 ) {
         cptC += 2 ;
     }

}

int kmain( void ) {

    init_hw();
    //init_ctx(&ctx_ping, ping, STACK_SIZE);
    //init_ctx(&ctx_pong, pong, STACK_SIZE);
    //current_ctx = &ctx_init;
    //switch_to(&ctx_ping);
    pid = 0;
    create_process(funcB, NULL, STACK_SIZE);
    create_process(funcA, NULL, STACK_SIZE);
    create_process(funcC, NULL, STACK_SIZE);
    start_sched();

    /*Pas atteignable vues nos 2 fonctions */
    return (0);

}
