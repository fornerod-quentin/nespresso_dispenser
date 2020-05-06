//--| Informations |-----------------------------------------------------------------------------
//-- HEIG-VD, Haute Ecole d'Ingenierie et de Gestion du canton de Vaud
//-- Projet Multidisciplinaire
//--
//-- Fichier      : projetCafeine3.ino
//--
//-- Description  : Distributeur de capsules nespresso, utilisants des servomoteur pour distribuer 
//--                différents types de capsules. Les servomoteur sont commandé par des Boutons.
//--                Un écran LCD indique l'étape actuelle du processus.
//--
//-- Auteur       : Loïc Fournier
//-- Date         : 15.04.2020 (Version 1)
//-- Version      : 0.2
//-- 
//--| Modifications |----------------------------------------------------------------------------
//-- Version  Date        Auteur      Description
//-- 0.0      22-03-2020  QFD         Version test séparé
//-- 0.1      14-04-2020  LFR         Version test LCD, switch et servomoteur 
//-- 0.2      14-04-2020  LFR         Version test avec FreeRTOS
//-- 1.0      15-04-2020  LFR         Version  1 avec if-else
//-- 1.1      15-04-2020  LFR         Version  1 avec machine d'état
//-- 2.0      25-04-2020  LFR         Version  2 avec interruption, delai et modification de 
//                                               la machine d'état
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


//----|Include|----------------------------------------------------------------------------------
#include <Adafruit_LiquidCrystal.h>  // Bilbliothèque pour l'écran LCD
#include <Servo.h>          // Bibliothèque pour les servomoteurs
//-----------------------------------------------------------------------------------------------

//----|Define|-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------

//----|Variables Constantes|---------------------------------------------------------------------
//Arduino
const uint8_t pinD2 = 2;
const uint8_t pinD3 = 3;
const uint8_t pinD4 = 4;
const uint8_t pinD5 = 5;
const uint8_t pinD6 = 6;
const uint8_t pinD7 = 7;
const uint8_t pinD8 = 8;
const uint8_t pinD9 = 9;
const uint8_t pinD10 = 10;
const uint8_t pinD11 = 11;
const uint8_t pinD12 = 12;
const uint8_t pinD13 = 13;
const uint8_t pinD14 = 14;
const uint8_t pinD15 = 15;
const uint8_t pinD16 = 16;
const uint8_t pinD17 = 17;
const uint8_t pinD18 = 18;
const uint8_t pinD19 = 19;
const uint8_t pinD20 = 20;
const uint8_t pinD21 = 21;

//Switch
const uint8_t switch1Pin = pinD8;
const uint8_t switch2Pin = pinD3;
const uint8_t switch3Pin = pinD2;
const uint8_t switch4Pin = pinD14;
const uint8_t switch5Pin = pinD15;
const uint8_t switch6Pin = pinD16;

//Servomoteur
const uint16_t servo1PosRepos = 90;
const uint16_t servo2PosRepos = 90;
const uint16_t servo1PinCtrl = pinD9;
const uint16_t servo2PinCtrl = pinD10;

//LCD
const uint8_t lcdNbColonnes = 16;
const uint8_t lcdNbLignes = 2;
const uint8_t lcdPinRs = pinD12;
const uint8_t lcdPinEn = pinD11;
const uint8_t lcdPinD4 = pinD4;
const uint8_t lcdPinD5 = pinD5;
const uint8_t lcdPinD6 = pinD6;
const uint8_t lcdPinD7 = pinD7;

//Capsules
const uint8_t posCapsule1 = 180;
const uint8_t posCapsule2 = 0;
const uint8_t posCapsule3 = 180;
const uint8_t posCapsule4 = 0;
//-----------------------------------------------------------------------------------------------

//----|Enumeré|-------------------------------------------------------------------------------
typedef enum{
  CHOIX_CAPSULE = 0,
  VERIF_CAPSULE_DISPONIBLE,
  CAPSULE_INDISPONIBLE,
  LECTURE_RFID,
  VERIF_ARGENT_SUFFISANT,
  ARGENT_SUFFISANT,
  ARGENT_INSUFFISANT,
  CONFIRMATION_PAYEMENT,
  PAYEMENT,
  ATTRAPAGE_CAPSULE,
  CAPSULE_ATTRAPER,
  LARGUAGE_CAPSULE,
  CAPSULE_LARGUER  
}eEtat;
//-----------------------------------------------------------------------------------------------

//----|Structures|-------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------

//----|Variables Global|-------------------------------------------------------------------------
//Machine d'état
eEtat etatMachine;

//Switch
uint8_t capsuleChoisit;
uint8_t payementValider;
uint8_t resetMachine;

//Servo
uint8_t servo;
uint8_t servo1PosActuelle;
uint8_t servo2PosActuelle;
uint16_t posDebut;
uint16_t posFin;
uint16_t posCapsule;

//Capteur
uint8_t capteurCapsuleDispo;
uint8_t capsuleLarguer;

//RFID
uint8_t payement;
uint8_t rfid;
uint8_t argentSuffisant;

//Delay
uint32_t attente;
//-----------------------------------------------------------------------------------------------

//----|Objets|-----------------------------------------------------------------------------------
Adafruit_LiquidCrystal monLcd(lcdPinRs, lcdPinEn, lcdPinD4, lcdPinD5, lcdPinD6, lcdPinD7);
Servo monServo1;
Servo monServo2;
//-----------------------------------------------------------------------------------------------

//----|Prototype|--------------------------------------------------------------------------------
void commandeServo(uint8_t servo, uint16_t posDebut, uint16_t posFin);
void affichage(eEtat etatMachine);
//capteurCapsuleDisponible();
//lectureRFID();
//verificationSolde();
//decompteArgent();
//capteurCapsuleLarguer();
//-----------------------------------------------------------------------------------------------

//----|Fonctions|--------------------------------------------------------------------------------
void commandeServo(uint8_t servo, uint16_t posDebut, uint16_t posFin)
{
  if(posFin > posDebut)
  {
    if(servo == 1)
    {
      for(servo1PosActuelle = posDebut; servo1PosActuelle < posFin; servo1PosActuelle++)
      {
        monServo1.write(servo1PosActuelle);
        delay(10);   
      }  
      
      delay(3000);     
    }
    else if(servo == 2)
    {
      for(servo2PosActuelle = posDebut; servo2PosActuelle < posFin; servo2PosActuelle++)
      {
        monServo2.write(servo2PosActuelle);
        delay(10);   
      }  
      
      delay(3000);     
    }     
  }
  else if(posFin < posDebut)
  {
    if(servo == 1)
    {
      for(servo1PosActuelle = posDebut; servo1PosActuelle > posFin; servo1PosActuelle--)
      {
        monServo1.write(servo1PosActuelle);
        delay(10);   
      }  
      
      delay(3000);     
    }
    else if(servo == 2)
    {
      for(servo2PosActuelle = posDebut; servo2PosActuelle > posFin; servo2PosActuelle--)
      {
        monServo2.write(servo2PosActuelle);
        delay(10);   
      }  
      
      delay(3000);     
    }     
  }
}

void affichage(eEtat etatMachine)
{
  switch(etatMachine)
  {
    case  CHOIX_CAPSULE : 
      monLcd.setCursor(0, 0);
      monLcd.print("Bienvenue");      
      monLcd.setCursor(0, 1);
      monLcd.print("Choisissez cafe");            
    break;
    case  CAPSULE_INDISPONIBLE : 
      monLcd.clear();
      monLcd.setCursor(0, 0);
      monLcd.print("Capsule");
      monLcd.setCursor(0, 1);
      monLcd.print("Indisponible");                
    break;
    case  LECTURE_RFID : 
      monLcd.setCursor(0, 0);
      monLcd.print("Prix : 0.5 CHF");
      monLcd.setCursor(0, 1);
      monLcd.print("Carte SVP");                
    break;
    case  ARGENT_SUFFISANT : 
      monLcd.clear();
      monLcd.setCursor(0, 0);
      monLcd.print("Prix : 0.5 CHF");
      monLcd.setCursor(0, 1);
      monLcd.print("Solde : 1 CHF");                
    break;
    case  ARGENT_INSUFFISANT : 
      monLcd.clear();
      monLcd.setCursor(0, 0);
      monLcd.print("Solde");
      monLcd.setCursor(0, 1);
      monLcd.print("Insuffisante");                
    break;
    case  CONFIRMATION_PAYEMENT : 
      monLcd.setCursor(0, 0);
      monLcd.print("Presser bouton 1");
      monLcd.setCursor(0, 1);
      monLcd.print("pour valider");                
    break;
    case  PAYEMENT : 
      monLcd.clear();
      monLcd.setCursor(0, 0);
      monLcd.print("Payement");
      monLcd.setCursor(0, 1);
      monLcd.print("Effectuer");                
    break;
    case  ATTRAPAGE_CAPSULE : 
      monLcd.clear();
      monLcd.setCursor(0, 0);
      monLcd.print("Distribution");
      monLcd.setCursor(0, 1);
      monLcd.print("En cours");                
    break;
    case  CAPSULE_LARGUER : 
      monLcd.clear();
      monLcd.setCursor(0, 0);
      monLcd.print("Ramassez");
      monLcd.setCursor(0, 1);
      monLcd.print("Votre capsule");              
    break;
    default :
    break;
  }
}
//-----------------------------------------------------------------------------------------------

//----|Initialisation|---------------------------------------------------------------------------
void setup() 
{
  //Machine d'état
  etatMachine = CHOIX_CAPSULE;
  
  //Switch 
  pinMode(switch1Pin,INPUT);  
  pinMode(switch2Pin,INPUT);
  pinMode(switch3Pin,INPUT);
  pinMode(switch4Pin,INPUT);
  pinMode(switch5Pin,INPUT);
  pinMode(switch6Pin,INPUT);
  capsuleChoisit = 0;
  resetMachine = 0;
  payementValider = 0;

  //Servomoteur  
  monServo1.attach(servo1PinCtrl);    // utilise la broche 9 pour le contrôle du servomoteur
  monServo1.write(servo1PosRepos);     // positionne le servomoteur à 0°
  
  monServo2.attach(servo2PinCtrl);    // utilise la broche 9 pour le contrôle du servomoteur
  monServo2.write(servo2PosRepos);     // positionne le servomoteur à 0°
  
  servo = 0;
  servo1PosActuelle = 0;
  servo2PosActuelle = 0;
  posDebut = 0;
  posFin = 0;
  posCapsule = 0;

  //LCD
  monLcd.begin(lcdNbColonnes, lcdNbLignes);    // set up the LCD's number of columns and rows:
  monLcd.clear();

  //Capteur
  capteurCapsuleDispo = 0;
  capsuleLarguer = 0;

  //RFID
  payement = 0;
  rfid = 0;
  argentSuffisant = 0;

  //Delay
  attente = 0;
  
  //Interruptions
  attachInterrupt(digitalPinToInterrupt(switch1Pin), choixCapsule1, FALLING);
  attachInterrupt(digitalPinToInterrupt(switch2Pin), choixCapsule2, FALLING);
  attachInterrupt(digitalPinToInterrupt(switch3Pin), choixCapsule3, FALLING);
  attachInterrupt(digitalPinToInterrupt(switch4Pin), choixCapsule4, FALLING);
  attachInterrupt(digitalPinToInterrupt(switch5Pin), validationPayement, FALLING);
  attachInterrupt(digitalPinToInterrupt(switch6Pin), reset, FALLING);
}
//-----------------------------------------------------------------------------------------------

//----|Boucle Infinie|---------------------------------------------------------------------------
void loop() 
{
  switch(etatMachine)
  {
    case CHOIX_CAPSULE : 
      affichage(etatMachine);
      if(capsuleChoisit == 1)
      {
        etatMachine = VERIF_CAPSULE_DISPONIBLE;
        capsuleChoisit = 0;
      }
      else
      {
        etatMachine = CHOIX_CAPSULE;
      }
    break;
    case VERIF_CAPSULE_DISPONIBLE : 
    //capteurCapsuleDisponible();
      capteurCapsuleDispo = 1;
      if(capteurCapsuleDispo == 1)
      {
        etatMachine = LECTURE_RFID;   
        attente = millis(); 
        monLcd.clear(); 
      }  
      else
      {
        etatMachine = CAPSULE_INDISPONIBLE;    
      } 
    break;
    case CAPSULE_INDISPONIBLE : 
      affichage(etatMachine);
      delay(3000);
      etatMachine = CHOIX_CAPSULE;        
      monLcd.clear();
    break;
    case LECTURE_RFID : 
      affichage(etatMachine);
      //lectureRFID();
      rfid = 1;
      if((resetMachine == 1)||((millis() - attente) >= 90000))
      {
        resetMachine = 0;
        etatMachine = CHOIX_CAPSULE;
        monLcd.clear();
      }
      else if(rfid == 1)
      {
        etatMachine = VERIF_ARGENT_SUFFISANT;;        
      }
      else
      {
        etatMachine = LECTURE_RFID;      
      }
    break;
    case VERIF_ARGENT_SUFFISANT : 
      //verificationSolde();
      argentSuffisant = 1;
      if(argentSuffisant == 1)
      {
        etatMachine = ARGENT_SUFFISANT;
      }
      else
      {
        etatMachine = ARGENT_INSUFFISANT;   
      }     
    break;
    case ARGENT_SUFFISANT : 
       affichage(etatMachine); 
       delay(3000);
       etatMachine = CONFIRMATION_PAYEMENT;
       attente = millis();  
       monLcd.clear();
    break;
    case ARGENT_INSUFFISANT : 
       affichage(etatMachine); 
       delay(3000);
       etatMachine = CHOIX_CAPSULE;
       monLcd.clear();
    break;
    case CONFIRMATION_PAYEMENT :
      affichage(etatMachine); 
      if(payementValider == 1)
      {
        etatMachine = PAYEMENT;  
        payementValider = 0;  
      }
      else if((resetMachine == 1)||((millis() - attente) >= 90000))
      { 
        resetMachine = 0;
        etatMachine = CHOIX_CAPSULE; 
        monLcd.clear();
      }
      else
      { 
        etatMachine = CONFIRMATION_PAYEMENT; 
      }
    break;
    case PAYEMENT :
      affichage(etatMachine);
      //decompteArgent();
      delay(3000);
      etatMachine = ATTRAPAGE_CAPSULE;
    break;
    case ATTRAPAGE_CAPSULE :
      affichage(etatMachine);
      commandeServo(servo, posDebut, posFin);
      etatMachine = LARGUAGE_CAPSULE ;  
    break;
    case LARGUAGE_CAPSULE :
      posFin = posDebut;
      if(servo == 1)
      {
         posDebut = servo1PosActuelle;
      }
      else if(servo == 2)
      {
         posDebut = servo2PosActuelle;
      }       
      commandeServo(servo, posDebut, posFin);
      //capteurCapsuleLarguer();
      capsuleLarguer = 1;
      if(capsuleLarguer == 1)
      {
        etatMachine = CAPSULE_LARGUER;
      }
      else
      {
        etatMachine = ATTRAPAGE_CAPSULE;
      }
    break;
    case  CAPSULE_LARGUER :
      affichage(etatMachine);  
      delay(3000);   
      etatMachine = CHOIX_CAPSULE;
      monLcd.clear();
    break;
    default :
    break;
  }
}
//-----------------------------------------------------------------------------------------------

//----|Interrupts|-------------------------------------------------------------------------------
void choixCapsule1()
{
  if(etatMachine==CHOIX_CAPSULE)
  {
    capsuleChoisit = 1;
    posDebut = servo1PosRepos;
    posFin = posCapsule1;
    servo = 1;    
  } 
}

void choixCapsule2()
{
  if(etatMachine==CHOIX_CAPSULE)
  {
    capsuleChoisit = 1;
    posDebut = servo1PosRepos;
    posFin = posCapsule2;
    servo = 1;    
  } 
}

void choixCapsule3()
{
  if(etatMachine==CHOIX_CAPSULE)
  {
    capsuleChoisit = 1;
    posDebut = servo2PosRepos;
    posFin = posCapsule3;
    servo = 2;    
  } 
}

void choixCapsule4()
{
  if(etatMachine==CHOIX_CAPSULE)
  {
    capsuleChoisit = 1;
    posDebut = servo2PosRepos;
    posFin = posCapsule4;
    servo = 2;    
  } 
}

void validationPayement()
{
  if(etatMachine==CONFIRMATION_PAYEMENT)
  {
    payementValider = 1;
  } 
}

void reset()
{
  if((etatMachine==LECTURE_RFID)||(etatMachine==CONFIRMATION_PAYEMENT))
  {
    resetMachine = 1;
  } 
}
//-----------------------------------------------------------------------------------------------
