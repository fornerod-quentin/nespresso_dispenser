/* Test pour vérifier le bonne fonctionnement des LEDs
 * Allumage d'une partie des LEDs avec la couleur choisite
 * 
 * Configuration Arduino:
 * Board: Arduino Nano Every
 * Registers emulation: None (ATMEGA809)
 * Arduino 1.8.12
 */
 
#include <Adafruit_NeoPixel.h>
#define PIN        2
#define NUMPIXELS 40

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();

  pixels.clear();
}

void loop() {
  pixels.clear();

  // Test de couleur pour LED spécifique
  // Chaque couleur possible est testé
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));       // NOIR

  pixels.setPixelColor(1, pixels.Color(0, 0, 150));     // BLEU

  pixels.setPixelColor(2, pixels.Color(0, 150, 0));     // VERT

  pixels.setPixelColor(3, pixels.Color(0, 150, 150));   // CYAN

  pixels.setPixelColor(4, pixels.Color(150, 0, 0));     // ROUGE

  pixels.setPixelColor(5, pixels.Color(150, 0, 150));   // VIOLET

  pixels.setPixelColor(6, pixels.Color(150, 150, 0));   // JAUNE
  
  pixels.setPixelColor(7, pixels.Color(150, 150, 150)); // BLANC 
  
  pixels.show();

}
