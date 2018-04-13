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


long arraySelected[] = {0, 0};
int modeSelected;

int chosenNumbers[6] = { -1, -1, -1, 1, -1, -1};

int discardedNumbers[9] = { -1, -1, -1, 1, -1, -1, -1, -1, -1};

boolean pressedNumbers[10];
int scheme = 0;
int counter = 0;
int numButtons = 0;
boolean flash = true;
long selectedTime;

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

  fourthModGroup(true);

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



void fourthModGroup(boolean f)
{
  boolean b = true;
  boolean b2 = false;
  flash = f;
  digitalWrite(ledPins[10], HIGH);
  while (b) {
    Serial.println("alla ricerca di @");
    for (int i = 0; i < sizeof(buttonPins) / sizeof(buttonPins[0]); i++) {
      if (b2 == true) {
        for (int i = 1; i < (sizeof(ledPins) / sizeof(ledPins[0])) - 2; i++)
        {
          digitalWrite(ledPins[i], HIGH);
        }
      }
      currentButtonsState[i] = debounce(i);
      if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i] && i == 10) {
        Serial.println("@ trovata");
        b2 = true;
        digitalWrite(ledPins[10], LOW);
      } else if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i] && i >= 1 && i <= 8 && b2 == true) {
        selectedTime = 1000 * i;
        b = false;
      }
    }
  }
  Serial.println("liberta");
  for (int i = 1; i < (sizeof(ledPins) / sizeof(ledPins[0])) - 1; i++)
  {
    digitalWrite(ledPins[i], LOW);
  }
  numButtons = random(1, 7);
  defineScheme(numButtons);
  while (true) {

    for (int i = 0; i < (sizeof(buttonPins) / sizeof(buttonPins[0])) - 1; i++)
    {
      currentButtonsState[i] = debounce(i);
      if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i])
      {
        pressedNumbers[i] = true;
        Serial.print("Pressed button ");
        Serial.println(i);
        counter++;
      }
      lastButtonsState[i] = currentButtonsState[i];
      delay(1);
      Serial.print("counter: ");
      Serial.println(counter);
      Serial.print("nButtons: ");
      Serial.println(getNumButtons());
      if (counter == getNumButtons()) {
        Serial.println("COUNTER e N UGUALI");
        if (isChecked()) {
          score++;
          Serial.println("Correct");
        }
        counter = 0;
        scheme++;
        if (scheme >= 4)
        {
          Serial.println("FINALE");
          scores[0] = score;
          scores[1] = 0;
          //clearVariables1();
          return;
        }
        numButtons = random(1, 7);
        
        defineScheme(numButtons);
      }

    }
  }
}

int getNumButtons() {
  /*Serial.print("flash:  ");
    Serial.println(flash);
    Serial.print("numButtons: ");
    Serial.println(numButtons);*/
  return flash ? numButtons : 10 - numButtons;
}

boolean isChecked() {

  for (int i = 0; i < numButtons; i++) {
    if (pressedNumbers[chosenNumbers[i]] != flash) {
      Serial.println("first check failed");
      Serial.println(chosenNumbers[i]);
      Serial.println("vale");
      Serial.println(pressedNumbers[chosenNumbers[i]]);
      Serial.println();
      for (int i = 0; i < 10; i++) {
        Serial.println(pressedNumbers[chosenNumbers[i]]);
      }
      return false;
    }
  }
  Serial.println("first check succeded");
  Serial.println(isUniqueArray());
  return isUniqueArray();
}

boolean isUniqueArray() {
  int countValues = 0;
  for (int i = 0; i < (sizeof(pressedNumbers) / sizeof(pressedNumbers[0])); i++) {
    Serial.println(pressedNumbers[i]);
    if(pressedNumbers[i] == true){
      countValues++;
    }
  }
  Serial.println("FINE CONTEGGIO");
  Serial.println(countValues);
  if(countValues == getNumButtons()){
     return true;
    }
  return false;
}

void defineScheme(int n) {
  for (int i = 0; i < 10; i++) {
    pressedNumbers[i] = false;
  }
  for(int i = 0; i < 6; i++){
      chosenNumbers[i] = -1;
  }
  for (int i = 0; i < n; i++) {
    int currentNumber = random(0, 10);
    for (int j = 0;  j < i; j++) {
      if (chosenNumbers[j] == currentNumber) {
        currentNumber = random(0, 10);
        j = -1;
      }
    }
    chosenNumbers[i] = currentNumber;
    Serial.print("chosen: ");
    Serial.println(currentNumber);
  }
  int savePoint = chosenNumbers[0];
  int count = 0;
  for (int i = 0; i < (sizeof(ledPins) / sizeof(ledPins[0])) - 2; i++) {
    for (int j = 0; j < n; j++) {
      if (i == chosenNumbers[j]) {
        digitalWrite(ledPins[i], HIGH);
        break;

      }
      if (j + 1 == n) {
        discardedNumbers[count++] = i;
        Serial.print("discarded: ");
        Serial.println(i);
        digitalWrite(ledPins[i], LOW);
      }
    }
  }
  chosenNumbers[0] = savePoint;
  Serial.print("8.0: ");
  Serial.println(chosenNumbers[0]);
  delay(selectedTime - 300);
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(buzzerPin2, HIGH);
  delay(50);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(buzzerPin2, LOW);
  delay(200);
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(buzzerPin2, HIGH);
  delay(50);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(buzzerPin2, LOW);

  for (int i = 0; i < sizeof(ledPins) / sizeof(ledPins[0]); i++) {
    digitalWrite(ledPins[i], LOW);
  }
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


