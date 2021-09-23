



#define HIGH 1
#define LOW 0
/****************** PIN Mapping *******************/
#define RS PORTCbits.RC6

#define EN PORTCbits.RC5


#define DATAPORT LATB
/************** Line Addr Mapping ******************/
#define LCD_LINE1 0x80
#define LCD_LINE2 0xC0
#define LCD_LINE3 0x94
#define LCD_LINE4 0xD4

#define CLRSCR 0x01
#define DISPLAY_ON 0x0C
#define DISPLAY_OFF 0x08
#define CURSOR_ON 0x0A
#define CURSOR_OFF 0x08
#define CURSOR_INC 0x06
#define MODE_8BIT 0x38
#define MODE_4BIT 0x28

void LCD_init(void);
void LCD_data(unsigned char data);
void LCD_cmd(unsigned char cmd);
void LCD_string(const char *ptr);
void LCD_isbusy(void);
void LCD_blink(void);




void LCD_init(void){
    delay_us(100);
    TRISB=0x00; 
    TRISC = 0X00;
    TRISCbits.TRISC4=1;
    TRISCbits.TRISC3=1;
    
    LCD_cmd(MODE_8BIT);                   // 2 Line, 5x7 display, 8 bit
    LCD_cmd(CLRSCR);                      // clear the screen
    LCD_cmd(CURSOR_INC);                  // Cursor Incremetns on each write
    LCD_cmd(DISPLAY_ON | CURSOR_OFF);     // Display on and Cursor Off
    return;
}	

void LCD_data(unsigned char data){
    delay_us(100);
    RS = HIGH;
   
    EN = HIGH;
    DATAPORT = data;
    delay_us(100);
    EN = LOW;
}
	
void LCD_cmd(unsigned char cmd){
   delay_us(100);
    RS = LOW;
    
    EN = HIGH;
    DATAPORT = cmd;
    delay_us(100);
    EN = LOW;
}
 

void LCD_text(unsigned char *buffer)
{
    while(*buffer)              // Write data to LCD up to null
    {
        
        LCD_data(*buffer);      // Write character to LCD
        buffer++;               // Increment buffer
    }
}



void CLS (void)
{

LCD_cmd(CLRSCR); 


};


 
 

unsigned char txt1[15]={"Danko   Radio "};
unsigned char txt2[15]={"Kossuth Radio "};
unsigned char txt3[15]={"Petofi  Radio "};
unsigned char txt4[15]={"Retro   Radio "};
unsigned char txt5[15]={"Romania Actual"};
unsigned char txt6[15]={"Bartok  Radio "};



unsigned int txt1x =914;
unsigned int txt2x =997;
unsigned int txt3x =1027;
unsigned int txt4x =1035;
unsigned int txt5x =1041;
unsigned int txt6x =1055;

unsigned char buf1[8];



void PRINT( unsigned int  Numberx)
{
    
       
    
          LCD_cmd(LCD_LINE1+(1)); 
            
            Numberx=(Numberx)*10;
            
            buf1[0]=((Numberx/10000)%10)+48;
            buf1[1]=((Numberx/1000)%10)+48;
            buf1[2]=((Numberx/100)%10)+48 ;
            buf1[3]=(46);
            buf1[4]=((Numberx/10)%10)+48;
            buf1[5]=(0); 
             
            
            LCD_text(buf1);
            
            LCD_cmd((LCD_LINE1+(8))); 
            
            LCD_text("Mhz   ");

}



void PRINT2(unsigned char *buf2)
{
    
          
    
            LCD_cmd(LCD_LINE2+(0)); 
         
            LCD_text(buf2);
            
            
   
}







