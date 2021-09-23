 
#define SDA_TRIS  TRISCbits.TRISC4
#define SCL_TRIS  TRISCbits.TRISC3
#define SDA       PORTCbits.RC4
#define SCL       PORTCbits.RC3

#define ACK       0x00
#define NACK      0x80



#define BYTE      unsigned char
#define WORD      unsigned int
#define DWORD      unsigned long 


void IdleI2Cx(void)
{
 
 
 Nop();
    
    
}




//....................................................................
// This function generates an I2C Start Condition
//....................................................................
void StartI2Cx(void)
{
unsigned int i;

SDA_TRIS = 1;                   // ensure SDA & SCL are high
SCL = 1;
SDA_TRIS = 0;                   // SDA = output
SDA = 0;                        // pull SDA low
for (i=0;i<2;i++) delay_us(1);
SCL = 0;                        // pull SCL low
}


//....................................................................
// This function generates an I2C Stop Condition
//....................................................................
void StopI2Cx(void)
{
unsigned int i;

SCL = 0;                        // ensure SCL is low
SDA_TRIS = 0;                   // SDA = output
SDA = 0;                        // SDA low
for (i=0;i<3;i++) delay_us(1);
SCL = 1;                        // pull SCL high
SDA_TRIS = 1;                   // allow SDA to be pulled high
for (i=0;i<3;i++) delay_us(1);
SCL=0;                          // ensure SCL is low
}


//....................................................................
// Outputs a bit to the I2C bus
//....................................................................
void bit_out(unsigned char data)
{
unsigned int i;

SCL = 0;                        // ensure SCL is low
SDA_TRIS=0;                     // configure SDA as an output
SDA= (data>>7);                 // output the MSB
for (i=0;i<2;i++) delay_us(1);
SCL = 1;                        // pull SCL high to clock bit
for (i=0;i<3;i++) delay_us(1);
SCL = 0;                        // pull SCL low for next bit
}


//....................................................................
// Inputs a bit from the I2C bus
//....................................................................
void bit_in(unsigned char *data)
{
unsigned int i;

SCL = 0;                        // ensure SCL is low
SDA_TRIS = 1;                   // configure SDA as an input
SCL = 1;                        // bring SCL high to begin transfer
for (i=0;i<3;i++) delay_us(1);
*data |= SDA;                   // input the received bit
SCL = 0;                        // bring SCL low again.
}


//....................................................................
// Writes a byte to the I2C bus
//....................................................................
unsigned char WriteI2Cx(unsigned char data)
{
unsigned char i;                // loop counter
unsigned char ack;              // ACK bit

ack = 0;
for (i = 0; i < 8; i++)         // loop through each bit
    {
    bit_out(data);              // output bit
    data = data << 1;           // shift left for next bit
    }

bit_in(&ack);                   // input ACK bit
return ack;
}


//....................................................................
// Reads a byte from the I2C bus
//....................................................................
unsigned char ReadI2Cx( )
{
unsigned char i;                // loop counter
unsigned char ret=0;            // return value

for (i = 0; i < 8; i++)         // loop through each bit
    {
    ret = ret << 1;             // shift left for next bit
    bit_in(&ret);               // input bit
    }

 
return ret;
}


//.............................................................................
//          Polls the bus for ACK from device
//.............................................................................
void AckI2Cx ( )
{
unsigned char result=1;

while(result)
	{
	StartI2Cx();            // generate Restart condition
	result=WriteI2Cx(0xff ); // send control byte (WRITE command)
        }

StopI2Cx();                     // generate Stop condition
}

