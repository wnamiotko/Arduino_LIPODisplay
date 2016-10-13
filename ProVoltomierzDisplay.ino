
#include "TM1651.h"
#define CLK 3//pins definitions for TM1651 and can be changed to other ports       
#define DIO 2
#define pinRelay 4 
#define inputPin A1
#define startScaling 736 //dla bateri 4S 0% 3,6V to ADC 766, 4,2V 100% to 859
#define stopScaling 859 //dla bateri 4S 0% to ADC 532, 100% to 696, dzialnik napięcia 12KOhm i 47KOhm
TM1651 batteryDisplay(CLK,DIO);
int odczytanaWartosc = 1000;//Odczytana wartość z ADC

//float napiecie = 5;//Wartość przeliczona na napięcie w V
int levelDisp = 5; //Wartość przeliczona na poziom wyświetlacza
int levelProc = 100;//Wartość przeliczona na napięcie w %
int x = 0; // Blink display
int lp = 0;// Zmianna pomocnicza do obliczania wartości levelProc
int ld = 0;// Zmianna pomocnicza do obliczania wartości levelDisp
 
void setup() {
  Serial.begin(9600);//Uruchomienie komunikacji przez USART
  batteryDisplay.init();
  batteryDisplay.set(7);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  batteryDisplay.frame(FRAME_ON);//light the frame of the battery display
  pinMode(pinRelay, OUTPUT);
  digitalWrite(pinRelay, HIGH);
  analogReference(EXTERNAL); //napięcie odniesienia na externall
}


void loop() {
  odczytanaWartosc = analogRead(inputPin);//Odczytujemy wartość napięcia
  levelPROC();
  relay();
  bateryDISP();
  displayBlink();
  delay(250);//Czekamy, aby wygodniej odczytywać wyniki
  
}

void relay() {    //relay
  if (levelProc>5)
  digitalWrite(pinRelay, HIGH);
  else
  digitalWrite(pinRelay, LOW);

}

void bateryDISP() { // battery display 
    ld = map (odczytanaWartosc, startScaling, stopScaling, 0, 5); //Przeliczenie wartości na napięcie (dla bateri 4S 0% to ADC 532, 100% to 696, dzialnik napięcia 12KOhm i 47KOhm)
      if (ld>=5)
    levelDisp=5;
      else if (ld<=0)
    levelDisp=0;
  // displayBlink();
     else
    levelDisp=ld;
    batteryDisplay.displayLevel(levelDisp); //Display wyświetlanie
  //  Serial.println(levelDisp);//Wysyłamy zmierzone napięcie
    
}

void levelPROC()  { //level in % over RS232 COM Port
     lp = map (odczytanaWartosc, startScaling, stopScaling, 0, 100); //Przeliczenie wartości na % (dla bateri 4S 0% to ADC 532, 100% to 696, dzialnik napięcia 12KOhm i 47KOhm)
   if (lp>=100)
       levelProc = 100;
    else if (lp<=0)
       levelProc = 0;
       else
       levelProc = lp;
  Serial.println(levelProc);//Wysyłamy zmierzone napięcie
  
}
void displayBlink(){

  if (levelDisp<1)
    {  
     batteryDisplay.displayLevel(x);
      x=!x;
    }
}
