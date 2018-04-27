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
  mod10();
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


int numChosen = 0;


void mod10()
{
  dimensions = 10;
  boolean b = true;
  boolean b2 = false;
  digitalWrite(ledPins[10], HIGH);
  while (b) {
    for (int i = 0; i < sizeof(buttonPins) / sizeof(buttonPins[0]); i++) {
      if (b2 == true) {
        for (int j = 1; j < (sizeof(ledPins) / sizeof(ledPins[0])) - 2; j++)
        {
          digitalWrite(ledPins[j], HIGH);
        }
      }
      currentButtonsState[i] = debounce(i);
      if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i] && i == 10) {
        b2 = true;
        digitalWrite(ledPins[10], LOW);
      } else if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i] && i >= 1 && i <= 9 && b2 == true) {
        numChosen = i;
        b = false;
      }
    }
  }
  Serial.print("num scelto: ");
  Serial.println(numChosen);
  for (int i = 1; i < (sizeof(ledPins) / sizeof(ledPins[0])) - 1; i++)
  {
    digitalWrite(ledPins[i], LOW);
  }

  while (true) {
    int buttonPressed = 0;
    response[0] = 0;
    response[1] = 0;
    delay(100);
    defineProduct();
    while (!pressed && !error) {
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
    delay(100);
    scheme++;
    Serial.println("cambio schema");
    delay(1000);
    timerGame = millis() - timer;
    Serial.print("schema:  ");
    Serial.println(scheme);
    if (scheme >= 12)
    {
      scores[0] = score;
      scores[1] = timerGame;
      //clearVariables();
      return;
    }
  }
}

void defineProduct() {
  setNextButton(false);
  Serial.println(); Serial.println(); Serial.println(); Serial.println();
  Serial.print(numChosen);
  Serial.print("  *  ");
  Serial.println(currentNumber);
  Serial.println(); Serial.println(); Serial.println(); Serial.println();
  solution = numChosen * currentNumber;
  startTime = millis();
}

void setNextButton(boolean light)
{
  while (true)
  {
    currentNumber = random(0, dimensions);

    if (currentNumber != lastNumber)
    {
      break;
    }
  }
  lastNumber = currentNumber;

  for (int i = 0; i < dimensions; i++)
  {
    digitalWrite(ledPins[i], LOW);
  }
  if (light) {
    digitalWrite(ledPins[currentNumber], HIGH);
  }
  return;
}
boolean clickAllButtonsAllows(int n) {
  if (((solution / 10) + 1) <= 1) {
    return 1 <= n;
  }
  return 2 <= n;

}

boolean checkResults() {
  return ((response[0] * 10 + response[1]) == solution);
}

