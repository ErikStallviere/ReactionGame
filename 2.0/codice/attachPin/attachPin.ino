
const byte interruptPin = 2;
volatile byte state = LOW;
const int led = 23;
const int ledPin = 25;
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(led, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
  pinMode(interruptPin, INPUT_PULLUP);
}

void loop() {
  //digitalWrite(led, HIGH);
  digitalWrite(ledPin, state);
//digitalWrite(interruptedPin, LOW);
delay(1000);
}

void blink() {
  //digitalWrite(ledPin, state);
  state = !state;
}
