/* SIMPLE TEST
   Test photorésistance
   Sans laser

   Configuration Arduino:
   Board: Arduino Nano Every
   Registers emulation: None (ATMEGA809)
   Arduino 1.8.12
*/

#define PIN_PHOTORES  A7                    //Definition pins photoresistance
#define WAIT_50_ms    50                    //Definition du temps d'attente

float val_photores;

void setup() {
  Serial.begin(9600);

  //Initialisation pin photoresistance
  pinMode(PIN_PHOTORES, INPUT);
  //Initialisation val_photores
  val_photores = 0.00;
}

void loop() {
  //Lecture valeur de la photoresistance
  val_photores = analogRead(PIN_PHOTORES);

  //Affichage valeur de la photoresistance
  Serial.print("Valeur photoresistance: ");
  Serial.println(val_photores);

  //Attente de 50ms
  delay(WAIT_50_ms);

}
