#include "vmem.h"
#include <stdint.h>
#define PAGE_SIZE 4096
#define FIRST_LVL_TT_COUN 4096
#define FIRST_LVL_TT_SIZE FIRST_LVL_TT_COUN*4  //Car la taille d'une entrée est 32bits donc 4 octects
#define SECON_LVL_TT_COUN 256
#define SECON_LVL_TT_SIZE  SECON_LVL_TT_COUN*4 //Car la taille d'une entrée est 32bits donc 4 octects 
#define TOTAL_TT_SIZE FIRST_LVL_TT_SIZE + SECON_LVL_TT_SIZE*FIRST_LVL_TT_COUN
#define FIRST_TABLE_START_POINTER 0x48000
#define SECON_LVL_TT_POS FIRST_TABLE_START_POINTER + FIRST_LVL_TT_SIZE // 0x4C000


uint32_t mask = 0x3ffffc00;		
    // 4 242 304
    //level-1 pages 64384 
    uint32_t * ft  = (uint32_t *)FIRST_TABLE_START_POINTER;
	uint32_t flags_first_level = (0 << 9) | // P
			(0 << 5) | // Domain 
			(0 << 4) | // SBZ 
			(0 << 3) | // NS 
			(0 << 2) | // SBZ 
			(0 << 1) | // Always 0
			(1 << 0); // Always 1

    uint32_t flags_second_level = (0 << 11) | // nG = 0
				(0 << 10) | // S = 0
				(0 << 9) | // APX = 0
				(001 << 6) | // TEX = 001
				(00 << 4) | // AP = 00
				(1 << 3) | // C = 1
				(1 << 2) | // B = 1
				(1 << 1) | // Always 1
				(0 << 0); // XN = 0
              
    uint32_t flags_second_level_io = (0 << 11) | // nG = 0
				(0 << 10) | // S = 0
				(0 << 9) | // APX = 0
				(001 << 6) | // TEX = 001
				(00 << 4) | // AP = 00
				(1 << 3) | // C = 1
				(1 << 2) | // B = 1
				(1 << 1) | // Always 1
				(0 << 0); // XN = 0
int init_kern_translation_table(void){
        
uint32_t *first_table_pointer = (uint32_t *)FIRST_TABLE_START_POINTER;
uint32_t page_i = 0;

        for(int i=0; i<FIRST_LVL_TT_SIZE;i++){
			
            uint32_t* second_table_pointer = (uint32_t *)(SECON_LVL_TT_POS + (i << 10));
			first_table_pointer[i] = (uint32_t) flags_first_level | ((uint32_t)second_table_pointer & 0xFFFFFC00); // [31…second_lvl_table_addr(22MSBs)…10|9…flags…0]
           
                       
            // level-2 pages
            for(int j=0; j<SECON_LVL_TT_SIZE;j++){
				 uint32_t val = 0; // Page Fault

                     // put the right flags values
            
            if(i<0x20000000) //normal case
            {
				val = flags_second_level | (page_i << 12); // [31…page_address(20MSBs)…12|11…flags…0]
			}
			else if (0x20FFFFFF) // I/O case
			{
						val = flags_second_level_io | (page_i << 12);
			}
           
				second_table_pointer[j] = val;
            	page_i ++;


            }
            
        }

 return 0;

}

// Start the MMU
bool start_mmu_c() {
register uint32_t control;
	__asm("mcr p15, 0, %[zero], c1, c0, 0" : : [zero] "r"(0)); //Disable cache
	__asm("mcr p15, 0, r0, c7, c7, 0"); //Invalidate cache (data and instructions) */
	__asm("mcr p15, 0, r0, c8, c7, 0"); //Invalidate TLB entries
	/* Enable ARMv6 MMU features (disable sub-page AP) */
	control = (1<<23) | (1 << 15) | (1 << 4) | 1;
	/* Invalidate the translation lookaside buffer (TLB) */
	__asm volatile("mcr p15, 0, %[data], c8, c7, 0" : : [data] "r" (0));
	/* Write control register */
	__asm volatile("mcr p15, 0, %[control], c1, c0, 0" : : [control] "r" (control));
return 0;
}
// Configure the MMU for pointing to the first TT
bool configure_mmu_c() {
	register uint32_t pt_addr = ((unsigned int)FIRST_TABLE_START_POINTER);
	/* Translation table 0 */
	__asm volatile("mcr p15, 0, %[addr], c2, c0, 0" : : [addr] "r" (pt_addr));
	/* Translation table 1 */
	__asm volatile("mcr p15, 0, %[addr], c2, c0, 1" : : [addr] "r" (pt_addr));
	/* Use translation table 0 for everything */
	__asm volatile("mcr p15, 0, %[n], c2, c0, 2" : : [n] "r" (0));
	/* Set Domain 0 ACL to "Manager", not enforcing memory permissions
	* Every mapped section/page is in domain 0
	*/
	__asm volatile("mcr p15, 0, %[r], c3, c0, 0" : : [r] "r" (0x3));
return 0;
}
