/*
Auteur: Quentin Fornerod
Date: 18.03.2020
Description: Pilotage d'un servomoteur SG90
*/
#include <Servo.h>
 
Servo monservo;  // crée l’objet pour contrôler le servomoteur
 #define v1 0    //position réservoir 1
 #define v2 180	 //posotion réservoir 2
void setup()
{
  monservo.attach(9);  // utilise la broche 9 pour le contrôle du servomoteur
  monservo.write(0); // positionne le servomoteur à 0°
}
 
 //réalise des rotations entre le réservoir 1 et 2
void loop()
{
   for(int i=0;i < v2; i++){
    monservo.write(i);
    delay(10);
  }
  delay(3000);
  for(int j=v2;j > 1; j--){
    monservo.write(j);
    delay(10);
  }
  delay(3000);
}
