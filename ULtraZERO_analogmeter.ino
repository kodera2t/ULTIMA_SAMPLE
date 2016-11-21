/// Analogue/Digital meter sample
/// for ULTRA-Zero
/// Default input:A0
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

  testscrolltext();
  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(32,32);

//////draw axis and tics///////
  display.drawCircle(64,64,31,WHITE);
  for(int j=30;j<180;j=j+30){
    for(int i=28;i<31;i++){
    xpos=64-i*cos(j*3.1415/180);
    ypos=64-i*sin(j*3.1415/180);
    display.drawPixel(xpos,ypos,WHITE);
    }
  }
////////////////////////////////q
  display.display();
}


void loop() {
  int readvalue;
  float voltage;
  readvalue=analogRead(A5);
  voltage=3.3*readvalue/1024.0;
  drawneedle(readvalue);
  eraseneedle(readvalue);
    display.fillRect(32,32,32,8,BLACK); 
    display.setTextColor(WHITE);
    display.setCursor(32,32);
    display.print(voltage); 
    display.print("V");
}

void drawneedle(int i){
  int xpos2,ypos2;
  xpos2=64-25*cos(i/1024.0*3.1415);
  ypos2=64-25*sin(i/1024.0*3.1415);
  display.drawLine(xpos2,ypos2,64,64,WHITE);
  display.display();  
}

void eraseneedle(int i){
  int xpos3,ypos3;
  xpos3=64-25*cos(i/1024.0*3.1415);
  ypos3=64-25*sin(i/1024.0*3.1415);
  display.drawLine(xpos3,ypos3,64,64,BLACK);
//  display.display();  
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(32,32);
  display.clearDisplay();
  display.println("voltage");
  display.setCursor(32,48); 
  display.println("meter"); 
  display.display();
 
  display.startscrollright(0x00, 0x0F);
  delay(8000);
  display.stopscroll();
  delay(500);
}

