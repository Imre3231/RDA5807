#include <xc.h> 
#include "p18f25k80.h"
#include "CONFIG.h"
#include "DELAY.h"

#include "RDA5807.h" 
#include "lcd.h" 




 





#define Button1 PORTAbits.RA0
#define Button2 PORTAbits.RA1






unsigned int x=00000;
unsigned int x2=00000;
unsigned int xhigh;
unsigned int xlow;
 


void Set( unsigned char  Number)
{
    
      
             
            if(Number==1){Frequency(txt1x);PRINT(txt1x); PRINT2(txt1);} 
            if(Number==2){Frequency(txt2x);PRINT(txt2x); PRINT2(txt2);}   
            if(Number==3){Frequency(txt3x);PRINT(txt3x); PRINT2(txt3);} 
            if(Number==4){Frequency(txt4x);PRINT(txt4x); PRINT2(txt4);} 
            if(Number==5){Frequency(txt5x);PRINT(txt5x); PRINT2(txt5);} 
            if(Number==6){Frequency(txt6x);PRINT(txt6x); PRINT2(txt6);} 

   
    
}



 

void main()
{
 ADCON1=0; 
 ANCON0=0;
 
  TRISA=0xff;
  TRISC = 0XFF;  
  TRISB = 0XFF;
   
 
   delay_ms(100);
   OpenI2C(MASTER, SLEW_OFF);
   SSPADD = 250;
    delay_ms(10);
 
  

    LCD_init();

    CLS();
    LCD_cmd(LCD_LINE1+(1)); 
    
    
    
    
    

  RDA5807M_Init();  

   RDA5807M_PowerOn();

  
   
 xlow=Read_b_eep(1);
 xhigh=Read_b_eep(2);
 x=((xhigh<<8)|(xlow&0xff));
   
   
  
   if(x< (1)){x= (1); }
   if(x>(6)){x=(6); }
  
    
 Set(x); 
   
   
   
   
  while(1)
  {
      
      
      
      if ( Button1==0)
      { x++; 
       
      
         if(x< (1)){x= (1); }
         if(x>(5)){x=(6); }
      
       Set(x);  }
          
      
      if ( Button2==0)
      { x-- ;  
      
         if(x< (1)){x= (1); }
         if(x>(6)){x=(6); }
      
      
      Set(x);  }  
           
   
     
 
      
      
      xlow=(x&0xFF);
      xhigh=((x>>8)&0xFF);
      
      
      
      if(x!=x2){Write_b_eep(1,xlow);Write_b_eep(2,xhigh);  x2=x;}
      
 
      

   
       
      
      
   }

    
      
   
  
      
  }
  
 