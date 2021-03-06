
#include <Wire.h>

#include <FastIO.h>
#include <I2CIO.h>
#include <lcd.h>
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

const byte RECV_PIN = 52;
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
int buzzerPin2 = 5;
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

//variavbili mod6

int errorCounter = 0;
int buttonCounter = 0;


//variabili mod23
int chosenOnesNumbers[11];

boolean chosenOnes[11];


//variabili mod10
int numChosen = 0;


//variabili per mod20
int maxCounter = 4;
int simonRecord[20];

int startPin = 8;
int scorePin = 7;
int finishPin = 9;

//variabili mod 5

boolean blinkButton;
boolean currentPedana = false;
boolean lastPedana = false;
int pedanaPin = buttonPins[10];


int tempo[] = {2, 51, 50, 49, 48, 47, 46};
//int punti[] = {3, 4, 5, 6, 7, 8, 9};


//variabili per collegare Arduino a DB SQL

byte mac[] = {0x90, 0xA2, 0xDA, 0x11, 0x1D, 0x55};
IPAddress ip(192, 168, 5, 16);
IPAddress server(192, 168, 5, 17);
EthernetClient client;
int conn;

void setup() {
  //Funzionamento dei pin
  Wire.begin();
  // inizializzazione dell'oggetto //lcd_I2C
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
  pinMode(startPin, OUTPUT);
  pinMode(finishPin, OUTPUT);
  pinMode(scorePin, OUTPUT);


  //connessione
  Serial.begin(9600);
  Ethernet.begin(mac, ip);

}

void loop()
{
  Serial.println("Inizio");
  for (int i = 0; i < sizeof(ledPins) / sizeof(ledPins[0]); i++) {
    digitalWrite(ledPins[i], LOW);
  }
  lcd.clear();
  digitalWrite(ledPins[11], HIGH);
  while (!debounce(11)) {
    Serial.print("@");
    lcd.setCursor(0, 0);
    lcd.print("Premi @ per ");
    lcd.setCursor(0, 1);
    lcd.print("scegliere");
    lcd.setCursor(0, 2);
    lcd.print("la modalita");
  }
  //Serial.println("@ premuta");
  delay(100);
  modeSelected = 0;
  arraySelected[0] = 0;
  arraySelected[1] = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mod disponibili:");
  lcd.setCursor(0, 1);
  lcd.print("1 - 23");
  lcd.setCursor(0, 2);
  lcd.print("Premi i bottoni");
  lcd.setCursor(0, 3);
  lcd.print("per scegliere");
  delay(200);
  do {
    for (int i = 0; i < sizeof(buttonPins) / sizeof(buttonPins[0]); i++) {
      currentButtonsState[i] = debounce(i);
      if (currentButtonsState[i] == true && lastButtonsState[i] == false && i < 10) {
        arraySelected[0] = arraySelected[1];
        arraySelected[1] = i;
        modeSelected = arraySelected[1] + arraySelected[0] * 10;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Mod disponibili:");
        lcd.setCursor(0, 1);
        lcd.print("1 - 23");
        lcd.setCursor(0, 2);
        lcd.print("modalita");
        lcd.setCursor(9, 2);
        lcd.print(modeSelected);
        Serial.print("mod: ");
        Serial.println(modeSelected);
        if  (modeSelected < 1 || modeSelected > nMod) {
          lcd.setCursor(0, 4);
          lcd.print("mod non valida");
        }
      }
      lastButtonsState[i] = currentButtonsState[i];
    }
  } while (!currentButtonsState[11] || !(modeSelected > 0 && modeSelected <= nMod));
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
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(buzzerPin2, HIGH);
  delay(delayValue * 2);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(buzzerPin2, LOW);
  lcd.clear();

  Serial.print("connessione: ");
  Serial.println(conn);
  startTimer();
  switch (modeSelected) {

    case 1:
      firstModGroup(10, 60000, false);
      break;
    case 2:
      firstModGroup(10, 300000, false);
      break;
    case 3:
      secondModGroup(10, 50);
      break;
    case 4:
      thirdModGroup(4, 100, true);
      break;
    case 5:
      thirdModGroup(11, 100, false);
      break;
    case 6:
      mod6();
      break;
    case 7:
      thirdModGroup(10, 50, false);
      break;
    case 8:
      firstModGroup(10, 30000, true);
      break;
    case 9:
      mod9();
      break;
    case 10:
      mod10();
      break;
    case 11:
      thirdModGroup(4, 25, true);
      break;
    case 12:
      thirdModGroup(4, 50, true);
      break;
    case 13:
      thirdModGroup(6, 25, false);
      break;
    case 14:
      thirdModGroup(6, 50, false);
      break;
    case 15:
      firstModGroup(6, 30000, false);
      break;
    case 16:
      firstModGroup(6, 60000, false);
      break;
    case 17:
      secondModGroup(6, 25);
      break;
    case 18:
      secondModGroup(6, 50);
      break;
    case 19:
      firstModGroup(6, 180000, false);
      break;
    case 20:
      mod20();
      break;
    case 21:
      fourthModGroup(true);
      break;
    case 22:
      fourthModGroup(false);
      break;
    case 23:
      mod23();
      break;
    default:
      //Serial.println(); Serial.println(); Serial.println(); Serial.println(); Serial.println(); Serial.println(); Serial.println(); Serial.println();
      //Serial.println("MOD NON ESISTENTE");
      break;
  }
  stopTimer();
  client.connect(server, 80);
  client.println("GET /reactiongame/updateAll.php?score=" + String(scores[0]) + "&modality=" + String(modeSelected) + " HTTP/1.1");
  client.println("Host: 192.168.5.17");
  client.println("Connection: close");
  client.println();
  client.stop();
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(buzzerPin2, HIGH);
  delay(25 * delayValue);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(buzzerPin2, LOW);
  //Serial.print("punteggio");
  //Serial.println(scores[0]);
  //Serial.print("tempo");
  //Serial.println(scores[1]);
  scores[0] = 0;
  scores[1] = 0;
  modeSelected = 0;
  //Serial.println("fine modalita");
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
  endTimer();
  //Serial.println("fine loop");

  score = 0;
  timerGame = 0;
}


