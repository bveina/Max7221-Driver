


/* hookup guide (arduino Nano)
PIN 11 to SIN
PIN 12 to SOUT
PIN 17 to SCK
PIN 10 to !CS

just for completeness....
VCC to VCC
GND to GND
*/


#include <SPI.h>
#define MAX7221_NOOP 0x00
#define MAX7221_DIG0 0x01
#define MAX7221_DIG1 0x02
#define MAX7221_DIG2 0x03
#define MAX7221_DIG3 0x04
#define MAX7221_DIG4 0x05
#define MAX7221_DIG5 0x06
#define MAX7221_DIG6 0x07
#define MAX7221_DIG7 0x08
#define MAX7221_DECODE 0x09
#define MAX7221_INTENSITY 0x0A
#define MAX7221_SCAN 0x0B
#define MAX7221_SHUTDOWN 0x0C
#define MAX7221_TEST 0x0F


void setup() {
  // put your setup code here, to run once:
  SPI.begin();
  
  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);

  delay(100);

  spiWrite(MAX7221_SHUTDOWN,0x1); //normal operation
  spiWrite(MAX7221_INTENSITY,0x0F);
  spiWrite(MAX7221_SCAN,0x07);

  spiWrite(MAX7221_TEST,0x01);
  delay(1000);
  spiWrite(MAX7221_TEST,0x00);
  spiWrite(MAX7221_DECODE,0x00);
 
}

void spiWrite(uint8_t addr,uint8_t value)
{
  digitalWrite(10,LOW);
  SPI.transfer(addr);
  SPI.transfer(value);
  digitalWrite(10,HIGH);
  delay(1);
}

uint8_t lut[8]={0,6,2,4,3,7,5,1};

void decodedTest()
{
  for (uint8_t i=0;i<15;i++)
  {
    for(uint8_t j = 0;j<8;j++)
    {
      spiWrite(MAX7221_DIG0+lut[j],i);
      delay(100);
      spiWrite(MAX7221_DIG0+lut[j],0xf);
    }
    delay(100);
  }
}

void codedTest()
{
  for (uint8_t i=0;i<255;i++)
  {
    for(uint8_t j = 0;j<8;j++)
    {
      spiWrite(MAX7221_DIG0+lut[j],i);
      delay(100);
      spiWrite(MAX7221_DIG0+lut[j],0);
    }
    delay(100);
  }  
}

void loop() {
   codedTest();
   decodedTest();
}
