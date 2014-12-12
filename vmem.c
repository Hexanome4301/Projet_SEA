#define PAGE_SIZE 4096
#define FIRST_LVL_TT_COUN 4096
#define FIRST_LVL_TT_SIZE FIRST_LVL_TT_COUN*4  //Car la taille d'une entrée est 32bits donc 4 octects
#define SECON_LVL_TT_COUN 256
#define SECON_LVL_TT_SIZE  SECON_LVL_TT_COUN*4 //Car la taille d'une entrée est 32bits donc 4 octects 
#define TOTAL_TT_SIZE FIRST_LVL_TT_SIZE + SECON_LVL_TT_SIZE*FIRST_LVL_TT_COUN
#define FIRST_TABLE_START_POINTER 0x48000
init_kern_translation_table(void){
uint32_t retAdress;
uint32_t mask = 0x3ffffc00;		
    // 4 242 304
    //level-1 pages 64384 
    uint32_t ft [] = FIRST_TABLE_START_POINTER;
    
    uint32_t device_flags = 0;
            device_flags |= (1 << 0);   //XN
            device_flags  |=(1 << 1) ;  //1
            device_flags  |=(1 << 2) ;  //B 
            device_flags  |=(1 << 3);   //C  
            device_flags  |=(11 << 4);  //AP
            device_flags  |=(001 << 6); //TEX
    
        for(int i=0; i<FIRST_LVL_TT_SIZE;i++){
            
            // put the right flags values
            if() //normal case
            {
			}
			else if () // I/O case
			{
			}
            
            
            // level-2 pages
            for(){
                
                     // put the right flags values
            if() //normal case
            {
			}
			else if () // I/O case
			{
			}
           
            
                        //logic adress = physic adress between  0x0 and 0x50000
                if(){
                    __asmvolatile("mcrp15,0,%[addr],c2,c0,1": : [addr]"r"(pt_addr))
                }
                        //logic adress = physic adress between 0x20000000 and 0x20FFFFFF
                else if(){
                    //uint32 line code
                   // uint32_t device_flags = ;
                   uint32_t temp_adress = ; // get the basic table entry
                   temp_adress >> 12; // shift the previous variable in order to get only the adress (put away the other bits)
                   
                    __asmvolatile("mcrp15,0,%[control],c1,c0,0": : [control]"r"(control));
                }

                //translation default
                else{
                    
                }
            }
            
        }



return retAdress;
}
