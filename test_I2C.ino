// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.
/*

//  Auteur: Quentin Fornerod || Date: 10.05.2020 || Création de la fonction "ishex"
//  Auteur: Quentin Fornerod || Date: 10.05.2020 || Implémentation des GPIOs
*/

#define GPIO_LED_GREEN 2
#define GPIO_LED_RED 3

#include <Wire.h>

bool ishex(char c){
if(isdigit(c) or (c >= 'A' and c <= 'F'))
  return true;
else
  return false;
}

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  pinMode(GPIO_LED_GREEN, INPUT);
  pinMode(GPIO_LED_RED, INPUT);
}

void loop()
{
  //Obtention et impression de l'ID
  Wire.requestFrom(0x30, 4);    // request 6 bytes from slave device #8

   while (Wire.available())   // slave may send less than requested
  {
    char c = Wire.read(); // receive a byte as character
    if(ishex(c))
      Serial.print(c);         // print the character
  
  //Impression du message selon l'état des GPIOs du lecteur
  if(digitalRead(GPIO_LED_GREEN)
	Serial.println("GREEN ON");
  }
  if(digitalRead(GPIO_LED_RED))
	Serial.println("RED ON");  
}
