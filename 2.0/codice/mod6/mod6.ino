#include <Wire.h>

#include <FastIO.h>
#include <I2CIO.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal_SR2W.h>
#include <LiquidCrystal_SR3W.h>

#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

#include <Ethernet2.h>
#include <SPI.h>

const byte RECV_PIN = 50;
IRrecv irrecv(RECV_PIN);
decode_results results;
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


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
float timeReflection = 1000;
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
  Wire.begin();
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
  mod6();
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

void setNextButton()
{
  while (true)
  {
    currentNumber = random(0, 10);

    if (currentNumber != lastNumber)
    {
      break;
    }
  }
  lastNumber = currentNumber;

  for (int i = 0; i < 10; i++)
  {
    digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(ledPins[currentNumber], HIGH);
  Serial.println(currentNumber);
  startTime = millis();
  return;
}

float timeElapsed() {
  elapsedTime = (millis() - startTime);
  return elapsedTime;
}

int errorCounter = 0;
int buttonCounter = 0;
void mod6() {
  setNextButton();
  while (true) {
    timeReflection = 1400 - scheme * 100;
    buttonCounter = 0;
    scheme++;
    Serial.print("timeReflection: ");
    Serial.println(timeReflection);
    while (buttonCounter <= 30) {
      Serial.println(buttonCounter);
      while (!pressed && !error) {
        for (int i = 0; i < 10; i++) {
          currentButtonsState[i] = debounce(i);
          if (timeElapsed() >= timeReflection) {
            Serial.println("TEMPO SCADUTO----------------------");
            error = true;
            break;
          }
          if (currentButtonsState[i] == true && lastButtonsState[i] != currentButtonsState[i]) {
            buttonCounter++;
            if (i == currentNumber) {
              pressed = true;
              Serial.println("RIGHT----------------------");
            } else {
              error = true;
              Serial.println("WRONG----------------------");
            }
          }
          lastButtonsState[i] = currentButtonsState[i];
        }
      }
      Serial.println("ESC");
      if (error) {
        errorCounter++;
      } else if (pressed) {
        errorCounter = 0;
      }
      delay(150);
      setNextButton();
      pressed = false;
      error = false;
      if (errorCounter >= 3) {
        Serial.print("errori: ");
        Serial.println(errorCounter);
        scores[0] = scheme;
        scores[1] = 0;
        scheme = 0;
        //clearVariables();
        return;
      }
    }
    Serial.println(); Serial.println(); Serial.println(); Serial.println();
    Serial.println("NUOVO SCHEMA"); Serial.println(); Serial.println(); Serial.println(); Serial.println();
    delay(100);
    if (scheme >= 10) {
      Serial.print("schema: ");
      Serial.println(scheme);
      scores[0] = scheme;
      scores[1] = 0;
      scheme = 0;
      //clearVariables();
      return;
    }
    delay(1000);
  }
}

