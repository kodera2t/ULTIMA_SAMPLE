#include <SD.h>
#include <SPI.h>
#include <AudioZero.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 7
Adafruit_SSD1306 display(OLED_RESET);

///// Prepare your favorite audio source by 8bit 
///// this source is modified version of https://www.arduino.cc/en/Tutorial/SimpleAudioPlayerZero
//// Detailed buffer amplifier information can be found in the above site.
///// Please install "AudioZero" library of https://github.com/arduino-libraries/AudioZero

void setup()
{
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  
  Serial.begin(115200);

  
  display.display();



  // setup SD-card
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(32,32);
  display.print("Audio");  
  display.setTextSize(1);  
  display.setCursor(32,48);
  display.print("SDcard init.");
  display.display();
  if (!SD.begin(10)) {
  display.setCursor(32,56);   
    display.print(" failed!");
    display.display();
    while(true);
  }
  display.setCursor(32,56); 
  display.print("now playing");
  display.display();

  // 44100kHz stereo => 88200 sample rate
  AudioZero.begin(2*44100);
}

void loop()
{
  int count = 0;

  // open wave file from sdcard
  File myFile = SD.open("test.wav");
  if (!myFile) {
    // if the file didn't open, print an error and stop
  display.setCursor(32,48);
  display.print("wav file err");  
  display.display();  
 //   Serial.println("error opening test.wav");
    while (true);
  }

  Serial.print("Playing");
  
  // until the file is not finished  
  AudioZero.play(myFile);
  display.clearDisplay();         
  display.setCursor(32,48);
  display.print("FILE END");
  display.setCursor(32,56);
  display.print("THANK YOU");    
  display.display(); 
  
  //Serial.println("End of file. Thank you for listening!");
  while (true) ;
}
