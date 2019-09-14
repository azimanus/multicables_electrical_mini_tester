/*
 * 
 * **************************************************
 * CODE POUR TESTER PLUSIEURS CABLES SIMULTANEIMENT *
 * **************************************************
 * 
 * les tests a effectuer sont:
 * ---> test de continuite
 * ---> test court-circuit
 * 
 * il verifie aussi si tous les fils sont connectes au testeur
 * 
 * Ce programme est destine a une carte arduino Uno R3,    donc, 
 * le maximum de cables qu'on peut tester simultanement est 9 (18 pins)
 * Mais on va se contenter de 8 cables pour qu'on puisse ajouter une LED
 * qui va s'allumer s'il y a au moins un defaut.
 * pour une carte a capacite plus grande (exemple: Arduino Mega), 
 * on peut par exemple augmenter le nombre de cables a tester et ajouter
 * des LED decrivant l'etat de chaque fil en plus de l'interface graphique
 * 
 */


// on cree nos propres types de donnees (etat cable)
enum {PASS, FAIL_NOTCONNECTED, FAIL_WRONGCONNECTED, FAIL_SHORTENED };

// les pins qui representent les deux extremites de chaque cable (9 cable --> 18 pins)
const byte pinsCableBegin[]= {2, 3, 4, 5, 6, 7, 8, 9};
const byte pinsCableEnd[]  = {10, 11, A0, A1, A2, A3, A4, A5};
const byte NUMCABLES=sizeof(pinsCableBegin);

byte pinLED = 12;


void setup() {
  Serial.begin(9600);
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, LOW)  ;
}


// On fixe l'etat des pins d'entree
void allPinsInput(){ 
  for (byte i=0;i<NUMCABLES;i++)
  {
    pinMode(pinsCableBegin[i],INPUT_PULLUP);
    pinMode(pinsCableEnd[i],INPUT_PULLUP);
  }
}


// on test l'ensemble de cables
void electricalTest()
{
  byte result;
  Serial.println();
  //Serial.println("\n");
  
  for (byte i=0;i<NUMCABLES;i++) // test each pin 
  {
    result= PASS; // initially there is no error found, assume PASS
    allPinsInput();
    
    // TEST -1: Test de continuite (cas 1)
    pinMode(pinsCableBegin[i], OUTPUT);
    if (digitalRead(pinsCableEnd[i])!=HIGH)
    {
        bitSet(result,FAIL_NOTCONNECTED);
        digitalWrite(pinLED, HIGH);
    }

    // TEST -1: Test de continuite (cas 2)
    digitalWrite(pinsCableBegin[i], LOW);
    if (digitalRead(pinsCableEnd[i])!=LOW)
    {
        bitSet(result,FAIL_NOTCONNECTED);
        digitalWrite(pinLED, HIGH);
    }
    
    // TEST -2: on verifie s'il y a une connexion faute entre pins
    for (byte j=0;j<NUMCABLES;j++)
    {
      if (j!=i && digitalRead(pinsCableEnd[j])==LOW)
      {
        bitSet(result, FAIL_WRONGCONNECTED);
        digitalWrite(pinLED, HIGH);
      }  
    }
    
    // TEST -3: test de court-circuit
    for (byte j=0;j<NUMCABLES;j++)
    {
      if (j!=i && digitalRead(pinsCableBegin[j])==LOW)
      {
        bitSet(result, FAIL_SHORTENED);
        digitalWrite(pinLED, HIGH);
      }  
    }

    // Affichage resultat test
    Serial.print("    Line ");
  
    Serial.print(i+1);
      Serial.print("  ->>>  ");
    if (result== PASS) Serial.print(" PASS");
    else Serial.print(" FAIL: ");
    if (bitRead(result,FAIL_NOTCONNECTED)) Serial.print(" NOT CONNECTED | ");
    if (bitRead(result,FAIL_WRONGCONNECTED)) Serial.print(" WRONG CONNECTION | ");
    if (bitRead(result,FAIL_SHORTENED)) Serial.print(" SHORT CIRCUIT | ");
    Serial.println("\n\n");
    Serial.println();
  }
  Serial.println("******* TEST FINISHED *******");
  Serial.println();
  Serial.setTimeout(50);
}


void loop() { 
  if (Serial.available())
  {
    electricalTest();
    delay(20);
    while (Serial.available()) Serial.read(); 
  }
}
