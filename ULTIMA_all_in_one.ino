/***************************************************************************
  This is a library example for the HMC5883 magnentometer/compass

  Designed specifically to work with the Adafruit HMC5883 Breakout
  http://www.adafruit.com/products/1746
 
  *** You will also need to install the Adafruit_Sensor library! ***

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries with some heading example from
  Love Electronics (loveelectronics.co.uk)
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the version 3 GNU General Public License as
 published by the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 ***************************************************************************/
// ULTIMA4 sample version
 

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <SPI.h>
#include <SSD_13XX.h>

#define BME280_ADDRESS 0x76
#define __CS1   8
#define __DC  10
#define __RST 9

SSD_13XX oled = SSD_13XX(__CS1, __DC,__RST);
int xval,yval;
float newdeg,olddeg;
int disp;

unsigned long int hum_raw,temp_raw,pres_raw;
signed long int t_fine;

uint16_t dig_T1;
 int16_t dig_T2;
 int16_t dig_T3;
uint16_t dig_P1;
 int16_t dig_P2;
 int16_t dig_P3;
 int16_t dig_P4;
 int16_t dig_P5;
 int16_t dig_P6;
 int16_t dig_P7;
 int16_t dig_P8;
 int16_t dig_P9;
 int8_t  dig_H1;
 int16_t dig_H2;
 int8_t  dig_H3;
 int16_t dig_H4;
 int16_t dig_H5;
 int8_t  dig_H6;



/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
//  SerialUSB.println("------------------------------------");
//  SerialUSB.print  ("Sensor:       "); SerialUSB.println(sensor.name);
//  SerialUSB.print  ("Driver Ver:   "); SerialUSB.println(sensor.version);
//  SerialUSB.print  ("Unique ID:    "); SerialUSB.println(sensor.sensor_id);
//  SerialUSB.print  ("Max Value:    "); SerialUSB.print(sensor.max_value); SerialUSB.println(" uT");
//  SerialUSB.print  ("Min Value:    "); SerialUSB.print(sensor.min_value); SerialUSB.println(" uT");
//  SerialUSB.print  ("Resolution:   "); SerialUSB.print(sensor.resolution); SerialUSB.println(" uT");  
//  SerialUSB.println("------------------------------------");
//  SerialUSB.println("");
//  delay(500);
}

void setup(void) 
{
      uint8_t osrs_t = 1;             //Temperature oversampling x 1
    uint8_t osrs_p = 1;             //Pressure oversampling x 1
    uint8_t osrs_h = 1;             //Humidity oversampling x 1
    uint8_t mode = 3;               //Normal mode
    uint8_t t_sb = 5;               //Tstandby 1000ms
    uint8_t filter = 0;             //Filter off 
    uint8_t spi3w_en = 0;           //3-wire SPI Disable
    
    uint8_t ctrl_meas_reg = (osrs_t << 5) | (osrs_p << 2) | mode;
    uint8_t config_reg    = (t_sb << 5) | (filter << 2) | spi3w_en;
    uint8_t ctrl_hum_reg  = osrs_h;


  //SerialUSB.begin(9600);
 // SerialUSB.println("HMC5883 Magnetometer Test"); SerialUSB.println("");
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
 //   SerialUSB.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();

  oled.begin();
  //initialize();
  oled.setRotation(0);
  oled.setTextScale(1);
  oled.setTextColor(YELLOW);
  oled.setCursor(0,0);
  oled.print("ULTIMA");
  oled.drawCircle(48,28,18,GREEN);
  oled.setCursor(46,0);
    oled.setTextColor(GREEN);
  oled.print("N");
  oled.setTextColor(YELLOW);
  oled.setCursor(16,28);
  oled.print("W");
  oled.setTextColor(BLUE);
  oled.setCursor(75,28);
  oled.print("E");
   oled.setTextColor(RED);
  oled.setCursor(46,48);
  oled.print("S"); 
    writeReg(0xF2,ctrl_hum_reg);
    writeReg(0xF4,ctrl_meas_reg);
    writeReg(0xF5,config_reg);
    readTrim();                    //
  
}

void loop(void) 
{
  for(int i=0;i<20;i++){
    displayHMC();
    delay(500);
  }
  for(int i=0;i<20;i++){
    displayBME();
    delay(500);
  }

}

void displayHMC(void){
    /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
  oled.fillRect(0,0,96,64,BLACK); 

///////setup
  oled.setTextScale(1);
  oled.setTextColor(YELLOW);
  oled.setCursor(0,0);
  oled.print("ULTIMA");
  oled.drawCircle(48,28,18,GREEN);
  oled.setCursor(46,0);
    oled.setTextColor(GREEN);
  oled.print("N");
  oled.setTextColor(YELLOW);
  oled.setCursor(16,28);
  oled.print("W");
  oled.setTextColor(BLUE);
  oled.setCursor(75,28);
  oled.print("E");
   oled.setTextColor(RED);
  oled.setCursor(46,48);
  oled.print("S"); 
/////////



   
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  //SerialUSB.print("X: "); SerialUSB.print(event.magnetic.x); SerialUSB.print("  ");
  //SerialUSB.print("Y: "); SerialUSB.print(event.magnetic.y); SerialUSB.print("  ");
  //SerialUSB.print("Z: "); SerialUSB.print(event.magnetic.z); SerialUSB.print("  ");SerialUSB.println("uT");

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.22;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  
//  SerialUSB.print("Heading (degrees): "); SerialUSB.println(headingDegrees);

  //oled.setCursor(0,55);
  //oled.print(headingDegrees);
  //oled.setCursor(50,55);
  newdeg=headingDegrees-90;
  //oled.print(newdeg);
  xval=48+15*cos(-2*PI*(olddeg/360));
  yval=28+15*sin(-2*PI*(olddeg/360));
  oled.drawLine(48,28,xval,yval,BLACK);
  xval=48+15*cos(-2*PI*(newdeg/360.0));
  yval=28+15*sin(-2*PI*(newdeg/360.0));
  oled.drawLine(48,28,xval,yval,RED);
  olddeg=newdeg;  
//  delay(500);
}

void displayBME(void){
     double temp_act = 0.0, press_act = 0.0,hum_act=0.0;
    signed long int temp_cal;
    unsigned long int press_cal,hum_cal;
    float index;
    readData();
    
    temp_cal = calibration_T(temp_raw);
    press_cal = calibration_P(pres_raw);
    hum_cal = calibration_H(hum_raw);
    temp_act = (double)temp_cal / 100.0;
    press_act = (double)press_cal / 100.0;
    hum_act = (double)hum_cal / 1024.0;
    index = 0.81*temp_act+0.01*(0.99*temp_act-14.3)+46.3;
//    Serial.print("TEMP : ");
//    Serial.print(temp_act);
//    Serial.print(" DegC  PRESS : ");
//    Serial.print(press_act);
//    Serial.print(" hPa  HUM : ");
//    Serial.print(index);
//    Serial.println(" %"); 
  oled.fillRect(0,0,96,64,BLACK);   
  oled.setTextColor(YELLOW);
    oled.setTextScale(2); 
  oled.setCursor(0,0);
  oled.print(press_act,1);
  oled.setCursor(0,20);
  oled.print(temp_act-10);
  oled.setCursor(0,40);
//  oled.print(hum_act);
  oled.print(hum_act);
  
    oled.setTextScale(2); 
  oled.setCursor(60,3);
  oled.print("hPa"); 
  oled.setCursor(60,23);
  oled.print("deg"); 
   oled.setCursor(60,43);
  oled.print("%"); 
 // oled.display();

//  if(index<60){
//    led_blink(blue_led, 1000);
//  } else if(index<75){
//    led_blink(green_led, 1000);
//  } else{
//    led_blink(red_led, 1000);   
//  }

}


void readTrim()
{
    uint8_t data[32],i=0;                      // Fix 2014/04/06
    Wire.beginTransmission(BME280_ADDRESS);
    Wire.write(0x88);
    Wire.endTransmission();
    Wire.requestFrom(BME280_ADDRESS,24);       // Fix 2014/04/06
    while(Wire.available()){
        data[i] = Wire.read();
        i++;
    }
    
    Wire.beginTransmission(BME280_ADDRESS);    // Add 2014/04/06
    Wire.write(0xA1);                          // Add 2014/04/06
    Wire.endTransmission();                    // Add 2014/04/06
    Wire.requestFrom(BME280_ADDRESS,1);        // Add 2014/04/06
    data[i] = Wire.read();                     // Add 2014/04/06
    i++;                                       // Add 2014/04/06
    
    Wire.beginTransmission(BME280_ADDRESS);
    Wire.write(0xE1);
    Wire.endTransmission();
    Wire.requestFrom(BME280_ADDRESS,7);        // Fix 2014/04/06
    while(Wire.available()){
        data[i] = Wire.read();
        i++;    
    }
    dig_T1 = (data[1] << 8) | data[0];
    dig_T2 = (data[3] << 8) | data[2];
    dig_T3 = (data[5] << 8) | data[4];
    dig_P1 = (data[7] << 8) | data[6];
    dig_P2 = (data[9] << 8) | data[8];
    dig_P3 = (data[11]<< 8) | data[10];
    dig_P4 = (data[13]<< 8) | data[12];
    dig_P5 = (data[15]<< 8) | data[14];
    dig_P6 = (data[17]<< 8) | data[16];
    dig_P7 = (data[19]<< 8) | data[18];
    dig_P8 = (data[21]<< 8) | data[20];
    dig_P9 = (data[23]<< 8) | data[22];
    dig_H1 = data[24];
    dig_H2 = (data[26]<< 8) | data[25];
    dig_H3 = data[27];
    dig_H4 = (data[28]<< 4) | (0x0F & data[29]);
    dig_H5 = (data[30] << 4) | ((data[29] >> 4) & 0x0F); // Fix 2014/04/06
    dig_H6 = data[31];                                   // Fix 2014/04/06
}
void writeReg(uint8_t reg_address, uint8_t data)
{
    Wire.beginTransmission(BME280_ADDRESS);
    Wire.write(reg_address);
    Wire.write(data);
    Wire.endTransmission();    
}


void readData()
{
    int i = 0;
    uint32_t data[8];
    Wire.beginTransmission(BME280_ADDRESS);
    Wire.write(0xF7);
    Wire.endTransmission();
    Wire.requestFrom(BME280_ADDRESS,8);
    while(Wire.available()){
        data[i] = Wire.read();
        i++;
    }
    pres_raw = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    temp_raw = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
    hum_raw  = (data[6] << 8) | data[7];
}


signed long int calibration_T(signed long int adc_T)
{
    
    signed long int var1, var2, T;
    var1 = ((((adc_T >> 3) - ((signed long int)dig_T1<<1))) * ((signed long int)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((signed long int)dig_T1)) * ((adc_T>>4) - ((signed long int)dig_T1))) >> 12) * ((signed long int)dig_T3)) >> 14;
    
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T; 
}

unsigned long int calibration_P(signed long int adc_P)
{
    signed long int var1, var2;
    unsigned long int P;
    var1 = (((signed long int)t_fine)>>1) - (signed long int)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11) * ((signed long int)dig_P6);
    var2 = var2 + ((var1*((signed long int)dig_P5))<<1);
    var2 = (var2>>2)+(((signed long int)dig_P4)<<16);
    var1 = (((dig_P3 * (((var1>>2)*(var1>>2)) >> 13)) >>3) + ((((signed long int)dig_P2) * var1)>>1))>>18;
    var1 = ((((32768+var1))*((signed long int)dig_P1))>>15);
    if (var1 == 0)
    {
        return 0;
    }    
    P = (((unsigned long int)(((signed long int)1048576)-adc_P)-(var2>>12)))*3125;
    if(P<0x80000000)
    {
       P = (P << 1) / ((unsigned long int) var1);   
    }
    else
    {
        P = (P / (unsigned long int)var1) * 2;    
    }
    var1 = (((signed long int)dig_P9) * ((signed long int)(((P>>3) * (P>>3))>>13)))>>12;
    var2 = (((signed long int)(P>>2)) * ((signed long int)dig_P8))>>13;
    P = (unsigned long int)((signed long int)P + ((var1 + var2 + dig_P7) >> 4));
    return P;
}

unsigned long int calibration_H(signed long int adc_H)
{
    signed long int v_x1;
    
    v_x1 = (t_fine - ((signed long int)76800));
    v_x1 = (((((adc_H << 14) -(((signed long int)dig_H4) << 20) - (((signed long int)dig_H5) * v_x1)) + 
              ((signed long int)16384)) >> 15) * (((((((v_x1 * ((signed long int)dig_H6)) >> 10) * 
              (((v_x1 * ((signed long int)dig_H3)) >> 11) + ((signed long int) 32768))) >> 10) + (( signed long int)2097152)) * 
              ((signed long int) dig_H2) + 8192) >> 14));
   v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * ((signed long int)dig_H1)) >> 4));
   v_x1 = (v_x1 < 0 ? 0 : v_x1);
   v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);
   return (unsigned long int)(v_x1 >> 12);   
}
