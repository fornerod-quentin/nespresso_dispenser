/* SIMPLE TEST
   Test bouton tactile
   Sans cylindres

   Configuration Arduino:
   Board: Arduino Mega 2560
   Registers emulation: ATMega 2560
   Arduino 1.8.12
*/

#define PIN_BOUTON     8                  //Definition pin bouton
#define WAIT_500_ms    500                //Definition du temps d'attente

int etat_bouton;

void setup() {
  Serial.begin(9600);

  //Initialisation pin photoresistance
  pinMode(PIN_BOUTON, INPUT);
  //Initialisation etat_bouton
  etat_bouton = 0;
}

void loop() {
  //Lecture etat du bouton
  etat_bouton = digitalRead(PIN_BOUTON);

  //Affichage valeur de la photoresistance
  Serial.print("Etat bouton: ");
  if(etat_bouton == HIGH){
    Serial.println("High");
  } else {
    Serial.println("Low");
  }

  //Attente de 500ms
  delay(WAIT_500_ms);

}
