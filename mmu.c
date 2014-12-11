#define PAGE_SIZE 4096
#define SECON_LVL_TT_COUN 256
#define SECON_LVL_TT_SIZE  SECON_LVL_TT_COUN*PAGE_SIZE 
#define FIRST_LVL_TT_COUN 4096
#define TOTAL_TT_SIZE FIRST_LVL_TT_SIZE + SECON_LVL_TT_SIZE*SECON_LVL_TT_COUN
#define FIRST_LVL_TT_SIZE FIRST_LVL_TT_COUN*4
init_kern_translation_table(void){
int retAdress;


    //level-1 pages
        for(){
            
            // put the flags values
            
            // level-2 pages
            for(){
                
                        //logic adress = physic adress between  0x0 and 0x50000
                if(){
                    __asmvolatile("mcrp15,0,%[addr],c2,c0,1": : [addr]"r"(pt_addr))
                }
                        //logic adress = physic adress between 0x20000000 and 0x20FFFFFF
                else if(){
                    //uint32 line code
                    uint32_t device_flags;
                    __asmvolatile("mcrp15,0,%[control],c1,c0,0": : [control]"r"(control));
                }

                //translation default
                else{
                    
                }
            }
            
        }



return retAdress;
}
