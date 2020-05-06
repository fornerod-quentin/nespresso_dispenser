/*
*Auteur: Quentin Fornerod
*Date: 03.04.2020
*Description: Pilotage d'un ruban led de type WS2812
*
* Configuration Arduino:
* Board: Arduino Nano Every
* Registers emulation: None (ATMEGA809)
* Arduino 1.8.12
*/

#include <FastLED.h>    //Librairie de pilotage pour le ruban
#define LED_PIN     7   //Pin Data
#define NUM_LEDS    40  //Nombre de LEDS que contient le ruban (ici 40)

CRGB leds[NUM_LEDS];    //crée une instance de la classe CRGB

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS); //type du ruban, pin data
  //initialisation de toutes les leds, deux variantes possible
  for(int i = 0; i < NUM_LEDS; i++){    
    leds[i] = CRGB::Green;
    //alternative:
    //leds[i] = CRGB(0, 255, 0); //(R,G,B)
    FastLED.show(); //nécessaire après chaque configuration de couleur ou autre
  }
}
void loop() {
delay(3000);  //Attend 3 s
  for(int i = 0; i<5;i++){  //clignote en bleu 5 fois
    leds[0] = CRGB(0, 0, 255);
    leds[1] = CRGB(0, 0, 255);
    leds[2] = CRGB(0, 0, 255);
    leds[3] = CRGB(0, 0, 255);
    leds[4] = CRGB(0, 0, 255);
    leds[5] = CRGB(0, 0, 255);
    FastLED.show();
    
    delay(200);
    
    leds[0] = CRGB(0, 0, 0);
    leds[1] = CRGB(0, 0, 0);
    leds[2] = CRGB(0, 0, 0);
    leds[3] = CRGB(0, 0, 0);
    leds[4] = CRGB(0, 0, 0);
    leds[5] = CRGB(0, 0, 0);
    FastLED.show();
    
    delay(200);
    }
    //tout enclenché en vert
  leds[0] = CRGB(0, 255, 0);
  leds[1] = CRGB(0, 255, 0);
  leds[2] = CRGB(0, 255, 0);
  leds[3] = CRGB(0, 255, 0);
  leds[4] = CRGB(0, 255, 0);
  leds[5] = CRGB(0, 255, 0);
  FastLED.show(); 
}
