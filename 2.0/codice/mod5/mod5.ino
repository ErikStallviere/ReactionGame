#include <Wire.h>

//#include <FastIO.h>
//#include <I2CIO.h>
//#include <LCD.h>
//#include <LiquidCrystal.h>
//#include <LiquidCrystal_I2C.h>
//#include <LiquidCrystal_SR.h>
//#include <LiquidCrystal_SR2W.h>
//#include <LiquidCrystal_SR3W.h>

#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

#include <Ethernet2.h>
#include <SPI.h>

const byte RECV_PIN = 50;
IRrecv irrecv(RECV_PIN);
decode_results results;
//LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


/**
   Array contenente il vecchio stato dei bottoni.
*/
boolean lastButtonsState[12];

/**
   Array contenente lo stato attuale dei bottoni.
*/
boolean currentButtonsState[12];


/**
   Array contenente i pins dei bottoni.
*/
int buttonPins[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44};
/**
   Array contenente i pins dei led
*/
int ledPins[] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45};

float scores[] = {0, 0};


int buzzerPin = 6;
int buzzerPin2 = 7;
int delayValue = 20;


long arraySelected[] = {0, 0};
int modeSelected;


int lastNumber = -1;
int currentNumber = -1;
float timer = 0;
int dimensions = 0;
int score = 0;

/**
   Il numero di millisecondi mancanti alla fine del gioco.
*/
long countdown = 0;


int countdownLoop = 0;


double timerGame = 0;
boolean corner;
boolean pressed = false;
float startTime;
float elapsedTime;
float timeReflection = 3000;
int shot = 100;
boolean error = false;


int hz = 0;

int nMod = 23;

//variabili per mod 21-22

int chosenNumbers[6] = { -1, -1, -1, 1, -1, -1};

int discardedNumbers[9] = { -1, -1, -1, 1, -1, -1, -1, -1, -1};

boolean pressedNumbers[10];
int scheme = 0;
int counter = 0;
int numButtons = 0;
boolean flash = true;
long selectedTime;


// variabili per mod 9
int response[] = {0, 0};
int solution = 0;
int addend[] = {0, 0};


//variabili per collegare Arduino a DB SQL
byte mac[] = {0x90, 0xA2, 0xDA, 0x11, 0x1D, 0x55};
IPAddress ip(192, 168, 5, 16);
IPAddress server(192, 168, 5, 17);
EthernetClient client;
int conn;

void setup() {
  //Funzionamento dei pin
  //Wire.begin();
  // inizializzazione dell'oggetto lcd_I2C
  lcd.begin(20, 4);
  lcd.clear();
  //accensione retro illuminazione
  lcd.backlight();
  randomSeed(analogRead(0));
  pinMode(buzzerPin, OUTPUT);
  pinMode(buzzerPin2, OUTPUT);
  for (int i = 0; i < sizeof(buttonPins) / sizeof(buttonPins[0]); i++)
  {
    //Assegno i pin ai relativi bottoni e led.
    pinMode(buttonPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);

    //Riempio di false i due array.
    lastButtonsState[i] = false;
    currentButtonsState[i] = false;
  }
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  conn = client.connect(server, 3305);
}

void loop()
{

  digitalWrite(ledPins[11], LOW);
  boolean led = true;
  for (int i = 0; i < 6; i++) {

    for (int j = 0; j < sizeof(buttonPins) / sizeof(buttonPins[0]); j++)
    {
      digitalWrite(ledPins[j], led);
    }
    delay(500);
    led = !led;
    if (i % 2 == 0) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(3 - i / 2);
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(buzzerPin2, HIGH);
      delay(delayValue);
      digitalWrite(buzzerPin, LOW);
      digitalWrite(buzzerPin2, LOW);
    }
  }
  thirdModGroup(11, 20, false);
  Serial.print("punteggio");
  Serial.println(scores[0]);
  Serial.print("tempo");
  Serial.println(scores[1]);
  scores[0] = 0;
  scores[1] = 0;
  modeSelected = 0;
  Serial.println("fine modalita");
  delay(1000);
  for (int i = 0; i < (sizeof(ledPins) / sizeof(ledPins[0])); i++) {
    digitalWrite(ledPins[i], HIGH);
  }
  bool b = true;
  while (b) {
    for (int i = 0; i < (sizeof(buttonPins) / sizeof(buttonPins[0])); i++) {
      if (debounce(i)) {
        b = false;
        break;
      }
    }
  }
  for (int i = 0; i < (sizeof(ledPins) / sizeof(ledPins[0])); i++) {
    digitalWrite(ledPins[i], LOW);
  }
  delay(5000);
  Serial.println("fine loop");
}


boolean debounce(int n)
{
  boolean current = (digitalRead(buttonPins[n]));
  if (lastButtonsState[n] != current)
  {
    delay(1);
    current = digitalRead(buttonPins[n]);
  }
  return current;
}
boolean blinkButton;
boolean currentPedana = false;
boolean lastPedana = false;
int pedanaPin = A1;
void thirdModGroup(int dim, int colpi, boolean angolo)
{
  corner = angolo;
  dimensions = dim;
  shot = colpi;
  start();
  boolean led = true;
  int giro = 0;
  while (true) {
    int counter = 0;
    while (!pressed && !error && (timeElapsed() <= timeReflection || !corner)) {
      for (int i = 0; i < dimensions; i++) {
        timerGame = millis() - startTime;
        counter = i;
        if (corner) {
          switch (currentNumber) {
            case 2:
              counter = 8;
              break;
            case 3:
              counter = 9;
              break;
          }
        }

        currentButtonsState[i] = debounce(counter);

        if (currentNumber == 10) {
          currentPedana = digitalRead(buttonPins[10]); //mancanza poi diventa pedanaPin
          if (currentPedana) {
            Serial.println("premutooooo pedana");
            pressed = true;
            break;
          }
          if (timeElapsed() >= timeReflection || (lastButtonsState[i] == LOW && currentButtonsState[i] == HIGH)) {
            Serial.println("errore pedana");
            error = true;
            break;
          }
          lastPedana = currentPedana;

        } else {
          if (currentNumber < 10 && blinkButton) {

            giro++;
            if (giro % 25 == 0) {
              led = !led;
            }
            digitalWrite(ledPins[currentNumber], led);

            delay(5);
          }
          if ((dimensions >= 10 && !blinkButton && lastButtonsState[i] == LOW && currentButtonsState[i] == HIGH && counter == currentNumber) || (blinkButton && timeElapsed() >= timeReflection))
          {

            Serial.println("premutooooo");

            pressed = true;
            break;
          }
          if ((dimensions >= 10 && (lastButtonsState[i] == LOW && currentButtonsState[i] == HIGH && counter != currentNumber)) || (!corner && timeElapsed() >= timeReflection) || (blinkButton && (lastButtonsState[i] == LOW && currentButtonsState[i] == HIGH))) {

            Serial.println("erroreeeeeee");

            error = true;
            break;
          }
        }
        lastButtonsState[i] = currentButtonsState[i];
      }
    }
    //stampThirdLCD(colpi);
    if (pressed) {
      digitalWrite(buzzerPin, HIGH);
      delay(delayValue);
      digitalWrite(buzzerPin, LOW);
      score++;
    }
    if (error) {
      timeReflection -= 50;
    }
    if (timeReflection < 200) {
      lcd.setCursor(0, 1);

      //        stampThirdLCD(colpi);
      lcd.print("GAME OVER");

      //        clearVariables();
      return;
    }
    pressed = false;
    error = false;
    shot--;
    start();
    if (shot <= 0) {
      int n = 0;
      for (int i = 0; i < dimensions; i++)
      {
        n = i;
        if (corner) {
          switch (i) {
            case 2:
              n = 8;
              break;
            case 3:
              n = 9;
              break;
          }
        }
        digitalWrite(ledPins[n], LOW);
      }
      scores[0] = score;
      scores[1] = timerGame;

      Serial.println("fine");

      //stampThirdLCD(colpi);
      //        clearVariables();
      return;
    }
  }
}
void start() {

  while (currentNumber == lastNumber)
  {


    currentNumber = random(0, dimensions);
    if (corner) {
      switch (currentNumber) {
        case 2:
          currentNumber = 8;
          break;
        case 3:
          currentNumber = 9;
          break;
      }
    }
  }
  int n = 0;
  lastNumber = currentNumber;
  for (int i = 0; i < dimensions; i++)
  {
    n = i;
    if (corner) {
      switch (i) {
        case 2:
          n = 8;
          break;
        case 3:
          n = 9;
          break;
      }
    }
    digitalWrite(ledPins[n], LOW);
  }
  digitalWrite(ledPins[11], LOW);
  if (dimensions == 11) {
    if (currentNumber == 10) {
      digitalWrite(ledPins[2], HIGH);
      digitalWrite(ledPins[3], HIGH);
      digitalWrite(ledPins[11], HIGH);
    } else {
      int blinkPercentual = random(1, 101);
      if (blinkPercentual <= 20) {
        blinkButton = true;
      } else {
        blinkButton = false;
        digitalWrite(ledPins[currentNumber], HIGH);
      }
    }
  } else {
    digitalWrite(ledPins[currentNumber], HIGH);
  }

  delay(200);
  startTime = millis();

}

float timeElapsed() {
  elapsedTime = (millis() - startTime);
  return elapsedTime;
}

