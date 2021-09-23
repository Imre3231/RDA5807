 







//--------------------------------------------------------------
// Global Functions
//--------------------------------------------------------------
void WriteAll(void);
void WriteReg(int address,int data);
void Init(void);
void Readregisters(void);
 
void PowerOn(void);
void PowerOff(void);
void RDS(void);
void Mute(void);
void Mono(void);
void Softmute(void);
void SoftBlend(void);
void BassBoost(void);
void AFC(void);
void Volume(int vol);
void SeekUp(void);
void SeekDown(void);
void Frequency(float Freq);
void Channel(void);
void getRDS(void);
 
// Initialise internal variables before starting or after a change to another channel.
void RDSinit();
 
// Pass all available RDS data through this function.
void ProcessData();
 
//--------------------------------------------------------------
// Definitions
//--------------------------------------------------------------
/* I2C Address */
#define RDASequential     0x20  // Write address
#define RDARandom         0x22  // Read address
/* Initialization Options */
#define StartingFreq    87.00
#define EndingFreq      108.00
#define DefaultFreq     87.50
#define InitialVolume        4
/* RDA5807M Function Code  */
/* Register 0x02H (16Bits) */
#define RDA_DHIZ        0x8000
#define RDA_MUTE        0x8000
#define RDA_MONO_ON     0x2000
#define RDA_MONO_OFF    0xDFFF
#define RDA_BASS_ON     0x1000
#define RDA_BASS_OFF    0xEFFF
#define RDA_RCLK_MODE   0x0800
#define RDA_RCLK_DIRECT 0x0400
#define RDA_SEEK_UP     0x0300
#define RDA_SEEK_DOWN   0x0100
#define RDA_SEEK_STOP   0xFCFF
#define RDA_SEEK_WRAP   0x0080
#define RDA_SEEK_NOWRAP 0xFF7F
#define RDA_CLK_0327    0x0000
#define RDA_CLK_1200    0x0010
#define RDA_CLK_2400    0x0050
#define RDA_CLK_1300    0x0020
#define RDA_CLK_2600    0x0060
#define RDA_CLK_1920    0x0030
#define RDA_CLK_3840    0x0070
#define RDA_RDS_ON      0x0008
#define RDA_RDS_OFF     0xFFF7
#define RDA_NEW         0x0004
#define RDA_RESET       0x0002
#define RDA_POWER       0x0001
/* Register 0x03H (16Bits) */
#define RDA_TUNE_ON     0x0010
#define RDA_TUNE_OFF    0xFFEF




#define RADIO_REG_RDSA   0x0C
#define RADIO_REG_RDSB   0x0D
#define RADIO_REG_RDSC   0x0E
#define RADIO_REG_RDSD   0x0F



//--------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------
//
int tuneok,nochannel,power,volume,softblend,bass,mute,softmute,mono,afc,stereo,signal,fmready,fmstation;
int rdsblockerror,rdssynchro,rdsready,rds,block1,block2,block3,block4;
 
char    StationName[10];    // Station Name. 8 characters
char    RDSText[66];        // RDS test message 64 characters
char    CTtime[12];         // CT time string formatted as 'CT hh:mm'  
int     minutes;            // CT minutes transmitted on the minute
float   freq;               // Tuned frequency
 
//--------------------------------------------------------------






unsigned int RDA5807M_WriteReg[8],RDA5807M_ReadReg[7],RDA5807M_RDS[32];
 
    // ----- actual RDS values
    int textAB, lasttextAB, lastTextIDX;
    int rdsGroupType, rdsTP, rdsPTY;
    
    int mins;           // RDS time in minutes
    int lastmins;
    int i,n;
    int idx;            // index of rdsText  
    int offset;         // RDS time offset and sign
    char c1, c2;
    char RDSTxt[66];
    char PSName[10];    // including trailing '\00' character.
    char PSName1[10];   
    char PSName2[10];   
    
    
    
    
     
unsigned int RDA5807M_WriteRegDef[6] ={0xC004,0x0000,0x0100,0x84D4,0x4000,0x0000}; // initial data




 
void RDA5807M_Init(){
    int i;
    for(i=0; i<6; i++){
        RDA5807M_WriteReg[i] = RDA5807M_WriteRegDef[i];
        WriteAll();
    }
}
 
void RDA5807M_PowerOn(){
    RDA5807M_WriteReg[1] = RDA5807M_WriteReg[1] | RDA_TUNE_ON;
    RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | RDA_POWER;
    WriteAll(); power=1;
    RDA5807M_WriteReg[1] = RDA5807M_WriteReg[1] & 0xFFEF;   //Disable tune after PowerOn operation
}
 
void RDA5807M_PowerOff(){
    RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] ^ RDA_POWER;
    WriteAll();power=0;
}
 
 
void RDA5807M_Volume(int vol){
    if(vol > 15){
        vol = 15;
    }
    if(vol < 0){
        vol = 0;
    }
    RDA5807M_WriteReg[3] = (RDA5807M_WriteReg[3] & 0xFFF0)| vol;   // Set New Volume
    volume=vol;
    WriteAll();
}
    

 
void  WriteAll()
{ 
    char buf[30];  
    int i,x = 0;
    for(i=0; i<12; i=i+2){
        buf[i] = RDA5807M_WriteReg[x] >> 8;
        x++;}
    x = 0;
    for(i=1; i<13; i=i+2){
        buf[i] = RDA5807M_WriteReg[x] & 0xFF;
        x++;}
    
    
    StartI2C();
    WriteI2C(0x20);
    for(i=0; i<14; i++)
    {
    WriteI2C(buf[i]);
    
    }
    
    StopI2C();
    
    
   
}




void SeekUp(){
    RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | RDA_SEEK_UP;   // Set Seek Up
    WriteAll();
    RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] & RDA_SEEK_STOP;   // Disable Seek
}
 
void SeekDown(){
    RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | RDA_SEEK_DOWN;   // Set Seek Down
    WriteAll();
    RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] & RDA_SEEK_STOP;   // Disable Seek
}



void  Frequency(float Freq){
    int Channel;
    Channel = ((Freq-(StartingFreq*10)) )+0.05;
    Channel = Channel & 0x03FF;
    RDA5807M_WriteReg[1] = Channel*64 + 0x10;  // Channel + TUNE-Bit + Band=00(87-108) + Space=00(100kHz)
    WriteAll();
    RDA5807M_WriteReg[1] = RDA5807M_WriteReg[1] & RDA_TUNE_OFF;
}
 


 


