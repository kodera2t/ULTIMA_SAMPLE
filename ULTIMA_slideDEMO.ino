#include <SPI.h>
#include <SSD_13XX.h>

#include "_images/nature2.c"
#include "_images/nature3.c"
#include "_images/nature4.c"
#include "_images/nature5.c"
#include "_images/nature6.c"
#include "_images/nature7.c"
#include "_images/sw1.c"





#define __CS1 	8
#define __DC 	10
#define __RST 9


SSD_13XX tft1 = SSD_13XX(__CS1, __DC,__RST);


void setup()
{
	tft1.begin();
	tft1.setRotation(0);

}

void loop()
{
    tft1.drawImage(0, 0, &nature2);
    delay(5000);
    tft1.drawImage(0, 0, &nature3);   
    delay(5000);
    tft1.drawImage(0, 0, &nature4);   
    delay(5000);
    tft1.drawImage(0, 0, &nature5);   
    delay(5000);
    tft1.drawImage(0, 0, &nature6);   
    delay(5000);
    tft1.drawImage(0, 0, &nature7);   
    delay(5000);

}
