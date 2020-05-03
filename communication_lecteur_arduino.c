#include <Arduino.h>
#include <SoftwareSerial.h>


SoftwareSerial serial_twn(10, 11,true); //true car liaison série "inversée" 

void setup()  
{
  Serial.begin(57600);	//liaison avec l'ordinateur
  serial_twn.begin(9600);	//liaison avec le lecteur
}

void loop() 
{

  if (serial_twn.available()){
	  //---VARIANTES ESSAYEES---
	//Serial.write(serial_twn.read());
	//Serial.println(serial_twn.read(),HEX);
	//Serial.println(serial_twn.read(),DEC);
  }
}
