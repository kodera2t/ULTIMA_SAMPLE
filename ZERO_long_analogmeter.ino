/// 2ch-Analogue/Digital meter sample
/// for Zero-long
/// Default input:A4 and A5
/// Nov. 2016 kodera2t

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 7
Adafruit_SSD1306 display(OLED_RESET);



void setup()   {
  int xpos,ypos;                
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done

  //testscrolltext();
  // Clear the buffer.
  display.clearDisplay();
//  display.setTextSize(1);
//  display.setTextColor(WHITE);
//  display.setCursor(32,32);

//////draw axis and tics///////
  display.drawCircle(16,32,16,WHITE);
  for(int j=30;j<180;j=j+30){
    for(int i=13;i<16;i++){
    xpos=16-i*cos(j*3.1415/180);
    ypos=32-i*sin(j*3.1415/180);
    display.drawPixel(xpos,ypos,WHITE);
    }
  }
    display.drawCircle(78,32,16,WHITE);
  for(int j=30;j<180;j=j+30){
    for(int i=13;i<16;i++){
    xpos=78-i*cos(j*3.1415/180);
    ypos=32-i*sin(j*3.1415/180);
    display.drawPixel(xpos,ypos,WHITE);
    }
  }
////////////////////////////////q
  display.display();
}


void loop() {
  int readvalue1,readvalue2;
  float voltage;
  readvalue1=analogRead(A4);
  readvalue2=analogRead(A5);
  voltage=3.3*readvalue1/1024.0;
  drawneedle(readvalue1,readvalue2);
  eraseneedle(readvalue1,readvalue2);
  showvalue(readvalue1,readvalue2);

}

void drawneedle(int i,int j){
  int xpos2,ypos2;
  xpos2=16-10*cos(i/1024.0*3.1415);
  ypos2=32-10*sin(i/1024.0*3.1415);
  display.drawLine(xpos2,ypos2,16,32,WHITE);
  xpos2=78-10*cos(j/1024.0*3.1415);
  ypos2=32-10*sin(j/1024.0*3.1415);
  display.drawLine(xpos2,ypos2,78,32,WHITE);
  display.display();  
}

void eraseneedle(int i,int j){
  int xpos3,ypos3;
  xpos3=16-10*cos(i/1024.0*3.1415);
  ypos3=32-10*sin(i/1024.0*3.1415);
  display.drawLine(xpos3,ypos3,16,32,BLACK);
  xpos3=78-10*cos(j/1024.0*3.1415);
  ypos3=32-10*sin(j/1024.0*3.1415);
  display.drawLine(xpos3,ypos3,78,32,BLACK);
//  display.display();  
}

void showvalue(int i,int j){
    float voltage=3.3*i/1024.0;
      display.fillRect(36,25,24,8,BLACK); 
    display.setTextColor(WHITE);
    display.setCursor(36,25);
    display.print(voltage); 
    voltage=3.3*j/1024.0;
    display.fillRect(36,16,24,8,BLACK); 
    display.setTextColor(WHITE);
    display.setCursor(36,16);
    display.print(voltage);     
    //display.print("V");
}



