#include <avrpins.h>
#include <max3421e.h>
#include <usbhost.h>
#include <usb_ch9.h>
#include <Usb.h>
#include <usbhub.h>
#include <avr/pgmspace.h>
#include <address.h>

#include <adk.h>

#include <printhex.h>
#include <message.h>
#include <hexdump.h>
#include <parsetools.h>
#include <Keypad.h>

USB Usb;
USBHub hub0(&Usb);
USBHub hub1(&Usb);
ADK adk(&Usb,"Google, Inc.",
"DemoKit",
"DemoKit Arduino Board",
"1.0",
"http://www.android.com",
"0000000012345678");


uint8_t  b, b1;

const byte ROWS = 2; //two rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {
    '1','2','3'                  }
  ,
  {
    '4','5','6'                  }
};


/*
 keypad 0 => io 2 =>  col 1
 keypad 1 => io 3 =>  row 0
 keypad 2 => io 4 =>  col 0
 keypad 4 => io 5 =>  col 2
 keypad 6 => io 6 =>  row 1
 */

byte rowPins[ROWS] = {
  3, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  4, 2, 5}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

boolean isWaitingForAndroid = false;

void setup();
void loop();




void setup()
{
  Serial.begin(115200);
  Serial.println("\r\nADK demo start");

  if (Usb.Init() == -1) {
    Serial.println("OSCOKIRQ failed to assert");
    while(1); //halt
  }//if (Usb.Init() == -1...

}

void loop()
{
  uint8_t rcode;
  uint8_t msg[3] = { 
    0x00             };
  Usb.Task();

  char key = keypad.getKey();

  if (key != NO_KEY){
    Serial.println(key);
  }

  if( adk.isReady() == true ) {

    isWaitingForAndroid = false;
    uint16_t len = sizeof(msg);

    rcode = adk.RcvData(&len, msg);

    if(len > 0) {
      USBTRACE("\r\nData Packet.");
      // assumes only one command per packet
      if (msg[0] == 0x2) {
        switch( msg[1] ) {
        case 0:
          USBTRACE2("\r\nLED ",msg[2]);
          break;
        }  
      }
    }

    msg[0] = 0x1;
    if (key != NO_KEY){

      switch (key)
      {
      case '1':
        msg[1] = 0;
        msg[2] = 1;
        rcode = adk.SndData( 3, msg );
        break;

      case '4':
        msg[1] = 0;
        msg[2] = 0;
        rcode = adk.SndData( 3, msg );
        break;

      case '2':
        msg[1] = 1;
        msg[2] = 1;
        rcode = adk.SndData( 3, msg );
        break;

      case '5':
        msg[1] = 1;
        msg[2] = 0;
        rcode = adk.SndData( 3, msg );
        break;

      case '3':
        msg[1] = 2;
        msg[2] = 1;
        rcode = adk.SndData( 3, msg );
        break;

      case '6':
        msg[1] = 2;
        msg[2] = 0;
        rcode = adk.SndData( 3, msg );
        break;

      }

    }
  } 
  else {
    if ( !isWaitingForAndroid) {
      Serial.println("Waiting for android connection...");
      isWaitingForAndroid = true;
    }
  }


  delay(10);       
}






