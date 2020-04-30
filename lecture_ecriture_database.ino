/*
Auteur: Quentin Fornerod
Date: 28.04.2020
Description: Ecriture et lecture de l'ID dans un fichier texte
*/
#include <SD.h> 
#include <SPI.h>
#include <Arduino.h>
int CS = 4; //chip select 
#define FILENAME "database.txt"
#define NBR_PERS 40
String NB_ID = "BB";
File file; 

typedef struct USER{
  char solde[6] = {"EMPTY"};
  char id[8] = {"NO_USER"};
  unsigned int emplacement = 0;
};
USER personne[NBR_PERS];
USER utilisateur_actif;

char data_tab[2];
void setup() {
  Serial.begin(9600); 
  pinMode(CS, OUTPUT); // chip select pin must be set to OUTPUT mode
  if (!SD.begin(CS)) { // Initialize SD card
    Serial.println("Could not initialize SD card."); // if return value is false, something went wrong.
  }
  
  if (SD.exists(FILENAME)) { // if "file.txt" exists, fill will be deleted
    Serial.println("File exists.");
    SD.remove(FILENAME);
  }
  
  file = SD.open(FILENAME, FILE_WRITE);
	//écriture de données sous la forme ID;Solde;
  file.print("AA");
  file.print(';');
  file.print("22");
  file.print(';');
  file.print('\n');
  file.print("BB");
  file.print(';');
  file.print("250");
  file.print(';');
  file.print('\n');
  file.print("CC");
  file.print(';');
  file.print("24");
  file.print(';');
  file.print('\n');
  file.print("DD");
  file.print(';');
  file.print("6665");
  file.print(';');
  file.print('\n');
  file.close();

}

void loop() {
  unsigned int file_size = 0;
  file = SD.open(FILENAME, FILE_READ); // open "file.txt" to read data
  file.seek(0);
  unsigned int cnt_ligne = 0;
  unsigned int cnt_lettre = 0;
  bool lire_id = false;
  bool lire_solde = false;
  if (file) {
    file_size = file.size(); //2 bytes par ligne
    Serial.println("il y a ce nombre de bytes:");
    Serial.println(file_size);
    
   while (file.available()) {
      //on commence par lire l'ID dans le fichier
      if(!lire_id and !lire_solde){
        lire_id = true;
      }
      char c = file.read();
      //ràz du compteur de lettre lors d'un ';'
     if(c == ';'){
      if(lire_id){
        personne[cnt_ligne].id[++cnt_lettre] = '\0';
        lire_id = false;
        lire_solde = true;
      }
      else{
        personne[cnt_ligne].solde[++cnt_lettre] = '\0';
        lire_id = true;
        lire_solde = false;
      }
      cnt_lettre = 0; 
     }
     if(c != '\r' and c != '\n' and c != ';'){
        //on détermine si on est en train de récupérer l'ID ou le solde
        if(lire_id){
          personne[cnt_ligne].id[cnt_lettre] = c;
        }
        else{
          personne[cnt_ligne].solde[cnt_lettre] = c;
        }
        cnt_lettre++;
     }
     else{
       if(c == '\n'){
        Serial.println(personne[cnt_ligne].id);
        Serial.println(personne[cnt_ligne].solde);
        Serial.println("en ligne:");
        Serial.println(cnt_ligne);
        cnt_ligne++;
       }
     }    
   }
    file.close();
    Serial.println("lecture terminee");
  } else {
    Serial.println("Could not open file (reading).");
  }
  //recherche de l'identifiant
for(int i = 0; i<cnt_ligne;i++){
  
  if(NB_ID.equals(personne[i].id)){
    Serial.println("Personne trouvee en position: ");
    Serial.println(i);  
    Serial.println("Son solde est de ");
    Serial.println(personne[i].id);  
}
}
  delay(5000); // wait for 5000ms
}
