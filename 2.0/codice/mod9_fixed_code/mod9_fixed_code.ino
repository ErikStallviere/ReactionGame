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

int score;



int buzzerPin = 6;
int buzzerPin2 = 7;
int delayValue = 20;


boolean pressed;
int response[] = {0, 0};
int solution = 0;
int addend[] = {0, 0};
int scheme = 0;
boolean error;
long timeReflection;
void setup() {
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
}

void loop()
{
  Serial.println("START");
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
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(buzzerPin2, HIGH);
  delay(delayValue * 2);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(buzzerPin2, LOW);
  lcd.clear();

  mod9();

  digitalWrite(buzzerPin, HIGH);
  digitalWrite(buzzerPin2, HIGH);
  delay(25 * delayValue);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(buzzerPin2, LOW);
  Serial.print("punteggio");
  Serial.println(scores[0]);
  Serial.print("tempo");
  Serial.println(scores[1]);
  scores[0] = 0;
  scores[1] = 0;
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


void mod9() {
  boolean b = true;
  boolean b2 = false;
  digitalWrite(ledPins[10], HIGH);
  while (b) {
    Serial.println("alla ricerca di @");
    for (int i = 0; i < sizeof(buttonPins) / sizeof(buttonPins[0]); i++) {
      if (b2 == true) {
        for (int j = 2; j < (sizeof(ledPins) / sizeof(ledPins[0])) - 2; j++)
        {
          digitalWrite(ledPins[j], HIGH);
        }
      }
      currentButtonsState[i] = debounce(i);
      if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i] && i == 10) {
        Serial.println("@ trovata");
        b2 = true;
        digitalWrite(ledPins[10], LOW);
      } else if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i] && i >= 2 && i <= 9 && b2 == true) {
        timeReflection = 1000 * i;
        b = false;
      }
    }
  }
  for (int j = 2; j < (sizeof(ledPins) / sizeof(ledPins[0])) - 2; j++)
  {
    digitalWrite(ledPins[j], LOW);
  }
  defineSum();
  delay(1000);
  while (true) {
    int buttonPressed = 0;
    response[0] = 0;
    response[1] = 0;
    while (!pressed && !error && timeElapsed() <= timeReflection) {

      for (int i = 0; i < 10; i++) {
        currentButtonsState[i] = debounce(i);
        if (currentButtonsState[i] == true && lastButtonsState[i] != currentButtonsState[i]) {
          response[0] = response[1];
          response[1] = i;
          buttonPressed++;
          Serial.print("bottoni: ");
          Serial.println(buttonPressed);
          Serial.print(response[0]);
          Serial.println(response[1]);
        }
        if (clickAllButtonsAllows(buttonPressed)) {
          Serial.println("BOTTONI PREMUTI");
          Serial.println(solution);
          if (checkResults())
          {
            pressed = true;
            digitalWrite(buzzerPin, HIGH);
            delay(delayValue);
            digitalWrite(buzzerPin, LOW);
            score++;
            Serial.println("RIGHT--------");
          } else {
            error = true;
            Serial.println("WRONG--------");
          }
          break;
        }
        lastButtonsState[i] = currentButtonsState[i];
      }
    }
    if (error) {
      lcd.setCursor(0, 1);
      lcd.print("SBAGLIATO");
      lcd.setCursor(0, 1);
      lcd.print("Risultato corretto: ");
      lcd.setCursor(0, 1);
      lcd.print(solution);
      lcd.setCursor(0, 1);
      lcd.print("Sua risposta: ");
      lcd.setCursor(0, 1);
      lcd.print(response[0]);
      lcd.print(response[1]);

    }
    pressed = false;
    error = false;
    scheme++;
    delay(100);
    if (scheme >= 8) {
      scores[0] = score;
      scores[1] = 0;
      scheme = 0;
      //clearVariables();
      return;
    }
    Serial.println("cambio schema");
    defineSum();
    delay(1000);
  }
}
long startTime;
void defineSum() {
  addend[0] = random(1, 10);
  addend[1] = random(1, 10);
  Serial.println(); Serial.println(); Serial.println(); Serial.println();
  Serial.print(addend[0]);
  Serial.print("  +  ");
  Serial.println(addend[1]);
  Serial.println(); Serial.println(); Serial.println(); Serial.println();
  solution = addend[0] + addend[1];
  startTime = millis();
}


float timeElapsed() {
  float elT = (millis() - startTime);
  return elT;
}

boolean clickAllButtonsAllows(int n) {
  return ((solution / 10) + 1) <= n;
}

boolean checkResults() {
  return ((response[0]*10 + response[1]) == solution);
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

