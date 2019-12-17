//////////test code for esp8266 wifi
#include <SoftwareSerial.h>
SoftwareSerial espSerial(2, 3); // RX, TX
char OK[2]="";

/*
 * function to check if the config of the esp8266 succeeded 
 * OO: i am a little unsure how this is supposed to work though
 */
void OK_config()
{  
 while(1)
  {
   OK[0]=espSerial.read();
   if(OK[0]=='O')
     {
      OK[1]=espSerial.read();
      if(OK[1]=='K')
        {
        break;
        }
     }
   delay(20);
   }  
}
/*
 * set up the wifi module (esp8266)
 */
void esp8266_config()
{
  Serial.println("AT+RST");   
  delay(100);
  espSerial.println("AT+RST");   //This will reset the wifi module. Its good practice to reset it before or after it has been programmed.
  OK_config();
  Serial.println("level 1:");
  //this should mention the firmware version installed on the ESP8266. (Optional) 
  //@oomp: not sure this actually works at this point, maybe I should move it to the loop
  //espSerial.println("AT+GMR");   
  //OK_config();
  Serial.println("2:");
  //mySerial.println("AT+CWLAP");   //This will detect the Access points and their signal strengths available in the area.
  //OK_config();
  Serial.println("3:");
  //espSerial.println("AT+CWJAP=\"Yellow Submarine\",\"@chterhoek!\"");   //This connects the ESP8266 to the specified SSID in the AT command mentioned in the previous code.
  espSerial.println("AT+CWJAP=\"Ip0ne\",\"Da!5h0gu\"");   //This connects the ESP8266 to the specified SSID in the AT command mentioned in the previous code.
  //OK_config();
  Serial.println("4:");
  //OK_config();
  Serial.println("5:");
  //espSerial.println("AT+CWMODE=1");   //set the mode
  //OK_config();
  Serial.println("6:");
  //espSerial.println("AT+RST");   //reset the esp 
  //Serial.println("AT+RST");   //reset the esp
  //OK_config();
  //mySerial.println("AT+CWMODE_DEF=3");   //set the current Wi-Fi mode and save to Flash
  //OK_config();
 // espSerial.println("AT+CIPMUX=1");       //multiple connection   default port=333
  //OK_config();
  //espSerial.println("AT+CIPSERVER=1");    //build TCP Server
  
  //OK_config();
  
  espSerial.println("AT+CIFSR");   //This will display the ESP8266’s obtained IP address.
  Serial.println("Initialized the server!");
  
  digitalWrite(13,HIGH);
  //OK_config();                          //wait Client to send OK; access to Client
}
///////////////////////////////////////////////////////////////////////
void setup() 
{ 
  Serial.begin(115200);
  espSerial.begin(115200);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  /*
  * looks like this setup function is called only once, correctly 
  */
  esp8266_config();
}

void loop()
{
    //http://www.martyncurrey.com/arduino-to-esp8266-serial-commincation/
   // listen for communication from the ESP8266 and then write it to the serial monitor
    if ( espSerial.available() )   {  Serial.write( espSerial.read() );  }

    // listen for user input and send it to the ESP8266
    if ( Serial.available() )       {  espSerial.write( Serial.read() );  }
  
 //mySerial.println("AT+CIPSEND=0,9"); 
 //espSerial.println("AT+CIFSR");
 //Serial.println("AT+CIFSR");
 //Serial.println(mySerial.println("AT+CIFSR"));
 delay(100);
 //espSerial.println("arduino");
 //Serial.println("arduino"); 
 digitalWrite(13,LOW);
 delay(100);
 digitalWrite(13,HIGH);
 delay(100);
}
