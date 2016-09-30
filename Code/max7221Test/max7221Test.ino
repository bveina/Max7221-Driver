


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

#define TESTDELAY 500

void setup() {
  Serial.begin(115200);
  
  // put your setup code here, to run once:
  SPI.begin();
  
  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);

  delay(100);

  spiWrite(MAX7221_SHUTDOWN,0x1); //normal operation
  spiWrite(MAX7221_INTENSITY,0x0F);
  spiWrite(MAX7221_SCAN,0x07);

  spiWrite(MAX7221_TEST,0x01);
  delay(TESTDELAY);
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
uint8_t font[16] = {0x7E,0x30,0x6D,0x79,0x33,0x5B,0x5F,0x70,0x7f,0x7b,0x77,0x1f,0x4e,0x3d,0x4f,0x47};

void decodedTest()
{
  Serial.println("decoded Test:");
  spiWrite(MAX7221_DECODE,0xff);
  setAllDigits(0x0f);
  
  for (uint8_t i=0;i<16;i++)
  {
    Serial.print(i); Serial.print(" ");
    setAllDigits(i);
    delay(TESTDELAY);
    setAllDigits(0x0f);
  }

  Serial.println();
  decodedDotTest();
  
}

void decodedDotTest()
{
  uint8_t j;
  Serial.println("decoded Dot Test:");
  spiWrite(MAX7221_DECODE,0xff);
  setAllDigits(0xf);
  for (j=0;j<4;j++)
  {
    Serial.print(j); Serial.print(" ");
    setAllDigits(0x88);
    delay(TESTDELAY);
    setAllDigits(0x0F);
    delay(TESTDELAY);
  }

  for (j=0;j<4;j++)
  {
    Serial.print(j); Serial.print(" ");
    setAllDigits(0x8F);
    delay(TESTDELAY);
    setAllDigits(0x0F);
    delay(TESTDELAY);
  }
  Serial.println();
}

void setAllDigits(uint8_t x)
{
  for(uint8_t k = 0;k<8;k++)
  {
    spiWrite(MAX7221_DIG0+k,x);
  }
}

void segmentTest()
{
  Serial.print("segment Test: ");
  spiWrite(MAX7221_DECODE,0x00);
  setAllDigits(0);
    
  for (uint8_t i=0x80;i>0;i>>=1)
  {
    Serial.print(i,HEX); Serial.print(" ");
    setAllDigits( i);
    delay(TESTDELAY);
    setAllDigits(0);
  }  
  Serial.println();
}

uint8_t reverseBits(uint8_t x)
{
  x= ((x & 0xaa) >> 1) | ((x & 0x55) << 1);
  x= ((x & 0xcc) >> 2) | ((x & 0x33) << 2);
  x= ((x & 0xf0) >> 4) | ((x & 0x0f) << 4);
}

void fontTest()
{
  Serial.println("Font Test");
  spiWrite(MAX7221_DECODE,0x00);
  setAllDigits(0x00);

  for (uint8_t i=0;i<16;i++)
  {
    Serial.print(i); Serial.print(" ");
    setAllDigits(font[i]);
    delay(TESTDELAY);
    setAllDigits(0x0f);
  }
  Serial.println();
}

void digitTest()
{
  Serial.println("digit Test");
  spiWrite(MAX7221_DECODE,0xFF);
  setAllDigits(0x0f);
  for(uint8_t j = 0;j<8;j++)
  {
    Serial.print(j); Serial.print(" ");
    spiWrite(MAX7221_DIG0+lut[j],0x88);
    delay(TESTDELAY);
    spiWrite(MAX7221_DIG0+lut[j],0x0f);
  }
  Serial.println();
}
void loop() {
   digitTest();
   segmentTest();
   decodedTest();
   fontTest();
}
