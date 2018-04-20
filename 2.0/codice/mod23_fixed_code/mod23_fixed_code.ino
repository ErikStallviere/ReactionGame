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

int chosenOnesNumbers[11];

boolean chosenOnes[11];

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
    chosenOnes[i] = false;
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
  mod23();
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
  //Serial.println("LISTA:");
  for (int i = 0; i < 11; i++) {
    //Serial.println(chosenOnes[i]);
    //Serial.println();
    //Serial.println(chosenOnesNumbers[i]);
  }
  while (true)
  {
    int n = random(0, dimensions);
    //Serial.print("num casuale: ");
    //Serial.println(n);
    currentNumber = chosenOnesNumbers[n];
    //Serial.print("num corrispondente: ");
    //Serial.println(currentNumber);
    if (currentNumber >= 0 && currentNumber <= 11) {
      //Serial.println(chosenOnes[currentNumber]);
      if (!chosenOnes[currentNumber])
      {
        Serial.println("CHECKED------------------");
        chosenOnes[currentNumber] = true;
        break;
      }
    }
  }

  for (int i = 0; i < 12; i++)
  {
    digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(ledPins[currentNumber], HIGH);
  //Serial.println(currentNumber);
  startTime = millis();
  return;
}



int countChosenOnes() {
  int c = 0;
  for (int i = 0; i < 11; i++) {
    if (chosenOnes[i]) {
      c++;
    }
  }
  return c;
}

void mod23()
{
  timer = millis();
  digitalWrite(ledPins[11], HIGH);
  while (!debounce(11)) {
    Serial.println("Prema #");
  }
  digitalWrite(ledPins[11], LOW);
  delay(1000);
  boolean finished = false;
  while (!finished) {
    for (int i = 0; i < 12; i++) {
      currentButtonsState[i] = debounce(i);

      if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i] && i != 11)
      {
        chosenOnes[i] = !chosenOnes[i];
        //Serial.println(i);
        //Serial.println(chosenOnes[i]);
        digitalWrite(ledPins[i], chosenOnes[i]);
      }
      if (countChosenOnes() >= 1 && debounce(11)) {
        finished = true;
      }
      lastButtonsState[i] = currentButtonsState[i];
    }
  }
  dimensions = countChosenOnes();
  for (int i = 0; i < dimensions; i++) {
    for (int j = 0; j < 11; j++) {
      if (chosenOnes[j]) {
        chosenOnesNumbers[i] = j;
        Serial.print("chosen: ");
        Serial.println(j);
        chosenOnes[j] = false;
        break;
      }

    }
  }

  for (int i = 0; i < 11; i++) {
    chosenOnes[i] = false;
  }
  setNextButton();

  while (true) {
    Serial.println("rewind-----------------");
    for (int i = 0; i < 11; i++) {
      chosenOnes[i] = false;
    }
    finished = false;
    while (!finished) {
      for (int i = 0; i < 11; i++)
      {
        currentButtonsState[i] = debounce(i);
        //Serial.print("current: ");
        //Serial.println(i);
        //Serial.print(": ");
        //Serial.println(currentButtonsState[i]);
        if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i] && i == currentNumber)
        {
          score++;
          Serial.println(i);
          Serial.println("CORRETTO---------------");
          setNextButton();
          digitalWrite(buzzerPin, HIGH);
          digitalWrite(buzzerPin2, HIGH);
          delay(delayValue);
          delay(1);
        }
        lastButtonsState[i] = currentButtonsState[i];
      }
      if (score % dimensions == 0 && score != 0) {
        Serial.println("finished");
        finished = true;
      }
    }
    timerGame = millis() - timer;

    scheme++;
    Serial.print("schema:  ");
    Serial.println(scheme);
    if (scheme >= 10)
    {
      for (int i = 0; i < 12; i++)
      {
        digitalWrite(ledPins[i], LOW);
      }
      scores[0] = score;
      scores[1] = timerGame;
      //clearVariables();
      return;
    }
  }
}
