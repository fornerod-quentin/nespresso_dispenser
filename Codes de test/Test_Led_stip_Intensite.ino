/* Test pour vérifier le bonne fonctionnement des LEDs
 * Allumage des LEDs avec une différent itensité de lumière
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
  // Test inténsité
  // Chaque LED est allumé de la même couleur avec des intensitées différentes
  // Le test est effectué pour des couleurs différentes (Vert, Bleu, Rouge)
  
  pixels.setPixelColor(0, pixels.Color(0, 100, 0));
  pixels.setPixelColor(1, pixels.Color(0, 150, 0));
  pixels.setPixelColor(2, pixels.Color(0, 200, 0));

  pixels.setPixelColor(4, pixels.Color(100, 0, 0));
  pixels.setPixelColor(5, pixels.Color(150, 0, 0));
  pixels.setPixelColor(6, pixels.Color(200, 0, 0));

  pixels.setPixelColor(8, pixels.Color(0, 0, 100));
  pixels.setPixelColor(9, pixels.Color(0, 0, 150));
  pixels.setPixelColor(10, pixels.Color(0, 0, 200));
  
  pixels.show();

}
