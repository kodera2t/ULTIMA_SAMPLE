/*
	Brrr...It's alive!
	CAUTION: Too big for arduino UNO or similar...
*/
#include <Wire.h>
#include <SPI.h>
#include <SSD_13XX.h>

#include "_images/eye/eye0.c"
#include "_images/eye/eye1.c"
#include "_images/eye/eye2.c"
#include "_images/eye/eye3.c"
#include "_images/eye/eye4.c"
#include "_images/eye/eye5.c"
#include "_images/eye/eye6.c"
#include "_images/eye/eye7.c"
#include "_images/eye/eye8.c"
#include "_images/eye/eye9.c"
#include "_images/eye/eye10.c"
#include "_images/eye/eye11.c"
#include "_images/eye/eye12.c"
#include "_images/eye/eye13.c"
#include "_images/eye/eye14.c"
#include "_images/eye/eye15.c"
#include "_images/eye/eye16.c"
#include "_images/eye/eye17.c"
#include "_images/eye/eye93.c"

const tPicture animation[19] = {
	eye0, eye1, eye2, eye3, eye4, eye5, eye6, eye7,
	eye8, eye9, eye10, eye11, eye12, eye13, eye14, eye15, eye16,
	eye17, eye93
};

#define __CS1   8
#define __DC  10
#define __RST 9


SSD_13XX tft1 = SSD_13XX(__CS1, __DC, __RST);

#define APDS9960_ADR  0x39
#define APDS9960_ID   0xAB

String        SERIAL_STRING;
unsigned char DATA_U,DATA_D,DATA_L,DATA_R;
unsigned char OLD_U,OLD_D,OLD_L,OLD_R;
unsigned char work;
unsigned char U_PEAK_END_FLAG,D_PEAK_END_FLAG,L_PEAK_END_FLAG,R_PEAK_END_FLAG;
unsigned char STATUS_UD,STATUS_LR;
unsigned char OLD_STATUS_UD,OLD_STATUS_LR;
unsigned char DISP_FLAG;
unsigned char NOISE_LEVEL = 2;
unsigned char DECIDE_FLAG;
unsigned int  PHASE_COUNTER;
unsigned int  U_PEAK,D_PEAK,L_PEAK,R_PEAK;
unsigned int oldpos,newpos;

void setup()
{
  Wire.begin();
	tft1.begin();
  randomSeed(A0);
  work = I2C_READ(0x92);        //READ REGISTER 0x92 (ADPS9960's ID)

  //
  I2C_WRITE(0x80,B01000101); //POWER ON<0>, GESTURE ENABLE<6>, PROXIMITY DETECT ENALBE<2>,AEN=0
  I2C_WRITE(0x90,B00110000); //Gesture LED Drive Strength 300%(max)
  I2C_WRITE(0xA3,B01100100); //Reserve0, Gain x8(11), LED Drive 100mA(00), Wait Time see under number
                             //111=39.2mS 110=30.8mS 101=22.4mS 100=14.0mS 011=8.4mS 010=5.6mS 001=2.8ms 000=0mS
  I2C_WRITE(0xA4,70);        //U MINUS OFFSET
  I2C_WRITE(0xA5,0);         //D MINUS OFFSET
  I2C_WRITE(0xA7,10);        //L MINUS OFFSET
  I2C_WRITE(0xA9,34);        //R MINUS OFFSET
  I2C_WRITE(0xAB,B00000001); //GIEN off<1>(INTERRUPT DISABLE), GMODE ON<0>
  RESET_VARIABLE();
  //pinMode(2, OUTPUT);
  //pinMode(3, OUTPUT);
  //pinMode(4, OUTPUT);
//  disp(5);
  newpos=1;
  oldpos=1;
  eye_move(newpos,oldpos);
}

void loop()
{
  int timing,tmp;
  timing=millis();
  tmp=1000*random(1,5);
  if(timing%tmp==0){
    eye_blink();
  }
  work = I2C_READ(0xAE);  //READ GESTUR FIFO LEVEL REGISTER
  if(work != 0)           //IF FIFO HAS SOME DATA
  {
    DATA_U = I2C_READ(0xFC);
    DATA_D = I2C_READ(0xFD);
    DATA_L = I2C_READ(0xFE);
    DATA_R = I2C_READ(0xFF);
    if((DATA_U > NOISE_LEVEL) && (DATA_D > NOISE_LEVEL)&& (DATA_L> NOISE_LEVEL) && (DATA_R > NOISE_LEVEL)) //NOISE CANCEL
    {
      DATA_SYORI();       // 
      PHASE_COUNTER++;    //
      DISP_FLAG = 1;      //
    }
    else 
    {
      if(DISP_FLAG)
      {
        DISP_FLAG = 0;
        //DISP_DIR();
      }
      RESET_VARIABLE();
    }
  }
  if(newpos!=oldpos){
  eye_move(newpos,oldpos);  
  }  
//  int x=random(10,20);
//  int y=random(0,10);
//	for (int i = y; i < x; i++) {
//    for(int i=0;i<20;i++){
//		tft1.clearScreen();
//		tft1.drawImage(0, 0, &animation[i]);
//		tft1.drawImage(47, 0, &animation[i]);
//    tft1.setCursor(10,50);
//    tft1.print(i);
//		delay(1000);
//	}
// delay(2000);
  oldpos=newpos;
}


void eye_move(int po,int opo){
//po: 1 to 5, opo is old position
if(po==opo){
    tft1.drawImage(0, 0, &animation[po]);
    tft1.drawImage(47, 0, &animation[po]); 
}

if((po==2)&&(opo==1)){
  for(int k=1;k<7;k++){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
}

if((po==1)&&(opo==2)){
  for(int k=6;k>0;k--){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
}

if((po==3)&&(opo==1)){
  for(int k=1;k<17;k++){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
}

if((po==1)&&(opo==3)){
  for(int k=16;k>0;k--){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
}

if((po==4)&&(opo==1)){
  for(int k=1;k<4;k++){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
}

if((po==1)&&(opo==4)){
  for(int k=3;k>0;k--){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
}

if((po==5)&&(opo==1)){
//    tft1.drawImage(0, 0, &animation[19]);
//    tft1.drawImage(47, 0, &animation[19]);  
    tft1.fillScreen(BLACK);   
}

if((po==1)&&(opo==5)){
    tft1.drawImage(0, 0, &animation[1]);
    tft1.drawImage(47, 0, &animation[1]);     
}

if((po==3)&&(opo==2)){
  for(int k=6;k<17;k++){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
}

if((po==2)&&(opo==3)){
  for(int k=16;k>5;k--){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
}

if((po==4)&&(opo==2)){
  for(int k=6;k<8;k++){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
}

if((po==2)&&(opo==4)){
  for(int k=7;k>5;k--){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
}

if((po==5)&&(opo==2)){
  for(int k=6;k<20;k++){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
  tft1.fillScreen(BLACK);
}

if((po==2)&&(opo==5)){
  for(int k=19;k>5;k--){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
}

if((po==4)&&(opo==3)){
  for(int k=16;k>6;k--){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
}

if((po==3)&&(opo==4)){
  for(int k=7;k<17;k++){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
}

if((po==5)&&(opo==4)){
  for(int k=7;k<20;k++){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
  tft1.fillScreen(BLACK);
}

if((po==4)&&(opo==5)){
  for(int k=19;k>6;k--){
    tft1.drawImage(0, 0, &animation[k]);
    tft1.drawImage(47, 0, &animation[k]);     
  }
}


}

void eye_blink(){
  tft1.drawImage(0,0, &animation[1]);
  tft1.drawImage(47,0, &animation[1]);
  //delay(200);  
  //tft1.fillScreen(BLACK);
  tft1.drawImage(0,0, &animation[18]);
  tft1.drawImage(47,0, &animation[18]);
  delay(100);  
  tft1.drawImage(0,0, &animation[1]);
  tft1.drawImage(47,0, &animation[1]);
  newpos=1;  
}


//***************************************************
unsigned char I2C_READ(unsigned char REG_ADR)
{
  Wire.beginTransmission(APDS9960_ADR);
  Wire.write(REG_ADR);
  Wire.endTransmission(false);
  Wire.requestFrom(APDS9960_ADR,1);
  return  Wire.read();
}

void I2C_WRITE(unsigned char REG_ADR, unsigned char DATA)
{
  Wire.beginTransmission(APDS9960_ADR);
  Wire.write(REG_ADR);
  Wire.write(DATA);
  Wire.endTransmission(false);
}
//************************************************************

void RESET_VARIABLE(void)
{
  PHASE_COUNTER = 0;
  U_PEAK=0;
  D_PEAK=0;
  L_PEAK=0;
  R_PEAK=0;
  OLD_U = 0;
  OLD_D = 0;
  OLD_L = 0;
  OLD_R = 0;
  U_PEAK_END_FLAG = 0;
  D_PEAK_END_FLAG = 0;
  L_PEAK_END_FLAG = 0;
  R_PEAK_END_FLAG = 0;
  STATUS_UD = 0;
  STATUS_LR = 0;
  OLD_STATUS_UD = 0;
  OLD_STATUS_LR = 0;
  SERIAL_STRING = "";
  DISP_FLAG = 0;
  DECIDE_FLAG = 0;
}
//----------------------------------------------------------------------
void DISP_DIR(void)
{
  if(!(SERIAL_STRING == "")) ;//SerialUSB.println(SERIAL_STRING);
}
//---------------------------------------------------------------------------------------
void DATA_SYORI(void)
{

  if (DATA_U > OLD_U)                //IF NEW_DATA > OLD_DATA_BUFFER(APROACH TO PEAK)
  {
    OLD_U = DATA_U;                  //SAVE NEW_DATA TO OLD_DATA_BUFFER
    U_PEAK = PHASE_COUNTER;          //PEAK_PHASE RENEWAL
    U_PEAK_END_FLAG = 0;             //STILL PEAK or APROACH TO PEAK
  }
  else
  {
    U_PEAK_END_FLAG = 1;             //PEAK WAS GONE
  }
  //**************************
  if (DATA_D > OLD_D)
  {
    OLD_D = DATA_D;
    D_PEAK = PHASE_COUNTER;
    D_PEAK_END_FLAG = 0;
  }
  else
  {
    D_PEAK_END_FLAG = 1;
  }
  //**************************
  if (DATA_L > OLD_L)
  {
    OLD_L = DATA_L;
    L_PEAK = PHASE_COUNTER;
    L_PEAK_END_FLAG = 0;
  }
  else
  {
    L_PEAK_END_FLAG = 1;
  }
  //*************************
  if (DATA_R > OLD_R)
  {
    OLD_R = DATA_R;
    R_PEAK = PHASE_COUNTER;
    R_PEAK_END_FLAG = 0;
  }
  else
  {
    R_PEAK_END_FLAG = 1;
  }
  //**************************
  if(U_PEAK_END_FLAG && D_PEAK_END_FLAG && L_PEAK_END_FLAG && R_PEAK_END_FLAG) //IF ALL PEAK WAS GONE
  {

    DECIDE_FLAG = 0;
    if ((U_PEAK > D_PEAK) & (U_PEAK >= L_PEAK) & (U_PEAK >= R_PEAK))           //U_PEAK WAS LAST
    {
     //resetLED();
     //LEDon(3);
     //LEDon(4);
     //disp(4);
     newpos=5;
      SERIAL_STRING = "DOWN";
      DECIDE_FLAG = 1;
    }
    if ((D_PEAK > U_PEAK) & (D_PEAK >= L_PEAK) & (D_PEAK >= R_PEAK))           //D_PEAK WAS LAST
    {
          //resetLED();
          //LEDon(2);
          //LEDon(3);
          //disp(3);
          newpos=4;
      SERIAL_STRING = "UP";
      DECIDE_FLAG = 1;
    }
    if ((L_PEAK >= U_PEAK) & (L_PEAK >= D_PEAK) & (L_PEAK > R_PEAK))           //L_PEAK WAS LAST
    {
          //resetLED();
          //LEDon(4);
          //disp(2);
          newpos=3;
      SERIAL_STRING = "RIGHT";
      DECIDE_FLAG =1;
    }
    if ((R_PEAK >= U_PEAK) & (R_PEAK >= D_PEAK) & (R_PEAK > L_PEAK))           //R_PEAK WAS LAST
    {
          //resetLED();
          //LEDon(2);
          //LEDon(3);
          //disp(1);
          newpos=2;
      SERIAL_STRING = "LEFT";
      DECIDE_FLAG = 1;
    }
    if (!DECIDE_FLAG);//SERIAL_STRING = "NONE";                                   //CAN'T DECIDE
 }
}

