#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

int pinFinish = A0;
int pinStart = A1;

int increment = A2;

const int dig4 = 13;
const int dig3 = 12;
const int dig2 = 11;
const int dig1 = 10;

int dueP = 9;//Da cambiare

/*
  const int sDx = 9/8;
  const int sSx = 8/7;
  const int s = 7/6;
  const int c = 6/3;
  const int bDx = 5/2;
  const int bSx = 4/4;
  const int b = 3/5;
*/

int tempo[] = {5, 4, 2, 3, 6, 7, 8}; //{52,51,50,49,48,47,46};
//int punti[] = {5,4,2,3,6,7,8};

int punteggio = 0;

int start;
int ora;

boolean currentIncrement = false;
boolean lastIncrement = false;
void setup() {
  pinMode(dig4, OUTPUT);
  pinMode(dig3, OUTPUT);
  pinMode(dig2, OUTPUT);
  pinMode(dig1, OUTPUT);

  for (int i = 0; i < sizeof(tempo); i++) {
    pinMode(tempo[i], OUTPUT);

  }
  pinMode(dueP, OUTPUT);

  digitalWrite(dig4, HIGH);
  digitalWrite(dig3, HIGH);
  digitalWrite(dig2, HIGH);
  digitalWrite(dig1, HIGH);

  pinMode(pinStart, INPUT);
  pinMode(pinFinish, INPUT);
  pinMode(increment, INPUT);

#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
#endif
  matrix.begin(0x70);


  Serial.begin(9600);
}

void loop() {
  punteggio = 0;

  if (analogRead(pinStart) > 1000 && analogRead(pinFinish) < 30) {
    start = millis() / 1000;
    while (true) {
      Serial.print("start: ");
      Serial.println(analogRead(pinStart));
      boolean finished = false;
      Serial.print("finish: ");
      Serial.println(analogRead(pinFinish));
      if (analogRead(pinFinish) > 1000) {
        finished = true;
        Serial.println("FINISH");
      }



      timer(start, finished);
      if (analogRead(pinStart) < 30 && analogRead(pinFinish) > 1000) {
        punteggio = 0;
        start = 0;
        ora = 0;
        timer(start, true);
        break;
      }
    }
  }
}

void timer(int inizio, boolean finish) {
  digitalWrite(dueP, HIGH);
  if (!finish) {
    ora = (millis() / 1000);
  }

  for (int dig = 4; dig > 0; dig--) {
    numero(dig);
    matrix.print(punteggio);
    matrix.writeDisplay();

    if (dig == 4) {
      stampa((ora - start) % 10, tempo);
    }
    else if (dig == 3) {
      stampa(((ora - start) / 10) % 6, tempo);
    }
    else if (dig == 2) {
      stampa(((ora - start) / 60) % 10, tempo);
    }
    else {
      stampa((((ora - start) / 60) / 10) % 6, tempo);
    }
    delayMicroseconds(5500);
    currentIncrement = analogRead(increment) > 1000;
    Serial.print("increment: ");
    Serial.println(analogRead(increment));
    if (currentIncrement && !lastIncrement) {
      Serial.println("AUMENTO");
      punteggio++;
    }
    lastIncrement = currentIncrement;
    spegni();
  }
}

void stampa(int nrDisplay, int arrayP[]) {

  switch (nrDisplay) {

    case 0:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], HIGH);
      digitalWrite(arrayP[1], HIGH);
      digitalWrite(arrayP[5], HIGH);
      digitalWrite(arrayP[3], LOW);
      break;

    case 1:
      digitalWrite(arrayP[4], LOW);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], LOW);
      digitalWrite(arrayP[1], LOW);
      digitalWrite(arrayP[5], LOW);
      digitalWrite(arrayP[3], LOW);
      break;

    case 2:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], LOW);
      digitalWrite(arrayP[0], HIGH);
      digitalWrite(arrayP[1], HIGH);
      digitalWrite(arrayP[5], LOW);
      digitalWrite(arrayP[3], HIGH);
      break;

    case 3:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], HIGH);
      digitalWrite(arrayP[1], LOW);
      digitalWrite(arrayP[5], LOW);
      digitalWrite(arrayP[3], HIGH);
      break;

    case 4:
      digitalWrite(arrayP[4], LOW);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], LOW);
      digitalWrite(arrayP[1], LOW);
      digitalWrite(arrayP[5], HIGH);
      digitalWrite(arrayP[3], HIGH);
      break;

    case 5:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], LOW);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], HIGH);
      digitalWrite(arrayP[1], LOW);
      digitalWrite(arrayP[5], HIGH);
      digitalWrite(arrayP[3], HIGH);
      break;

    case 6:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], LOW);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], HIGH);
      digitalWrite(arrayP[1], HIGH);
      digitalWrite(arrayP[5], HIGH);
      digitalWrite(arrayP[3], HIGH);
      break;

    case 7:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], LOW);
      digitalWrite(arrayP[1], LOW);
      digitalWrite(arrayP[5], LOW);
      digitalWrite(arrayP[3], LOW);
      break;

    case 8:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], HIGH);
      digitalWrite(arrayP[1], HIGH);
      digitalWrite(arrayP[5], HIGH);
      digitalWrite(arrayP[3], HIGH);
      break;

    case 9:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], HIGH);
      digitalWrite(arrayP[1], LOW);
      digitalWrite(arrayP[5], HIGH);
      digitalWrite(arrayP[3], HIGH);
      break;
  }
}

void numero(int digit) {
  switch (digit) {
    case 1:
      digitalWrite(dig1, LOW);
      break;
    case 2:
      digitalWrite(dig2, LOW);
      break;
    case 3:
      digitalWrite(dig3, LOW);
      break;
    case 4:
      digitalWrite(dig4, LOW);
      break;
  }
}
void spegni() {
  digitalWrite(dig1, HIGH);
  digitalWrite(dig2, HIGH);
  digitalWrite(dig3, HIGH);
  digitalWrite(dig4, HIGH);
}
