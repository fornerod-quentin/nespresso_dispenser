/*
Auteur: Quentin Fornerod
Date: 015.04.2020
Description: Afficher les données du port sur le traceur série
*/
void setup() {
  Serial.begin(9600);
}
void loop() {
int sensorValue = analogRead(A0);
  // Convert analog value (0 - 1023)  -> (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  Serial.println(voltage);
}
