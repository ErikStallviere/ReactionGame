
void incrementScore() {
  analogWrite(scorePin, 1023);
  delay(5);
  analogWrite(scorePin, 0);
}

void startTimer() {
  analogWrite(startPin, 1023);
  analogWrite(finishPin, 0);
}

void stopTimer() {
  analogWrite(startPin, 1023);
  analogWrite(finishPin, 1023);
}

void endTimer() {
  analogWrite(startPin, 0);
  analogWrite(finishPin, 1023);
}

boolean debounce(int n)
{
  boolean current = !(digitalRead(buttonPins[n]));
  if (lastButtonsState[n] != current)
  {
    delay(1);
    current = !digitalRead(buttonPins[n]);
  }
  return current;
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

void stampThirdLCD(int colpi) {

  lcd.clear();
  Serial.print("Punteggio: ");
  Serial.println(score);
  Serial.print("Colpi: ");
  Serial.print(colpi - shot);
  Serial.print(" / ");
  Serial.println(colpi);
  lcd.setCursor(0, 0);
  lcd.print("Tiri: ");
  lcd.setCursor(0, 3);
  lcd.print(score);
  lcd.setCursor(0, 2);
  lcd.print("Punteggio: ");
  lcd.setCursor(0, 1);
  lcd.print(colpi - shot);
  lcd.setCursor(3, 1);
  lcd.print(" / ");
  lcd.setCursor(7, 1);
  lcd.print(colpi);

}



float timeElapsed() {
  elapsedTime = (millis() - startTime);
  return elapsedTime;
}

void clearVariables() {
  timer = 0;
  dimensions = 0;
  countdown = 0;

  countdownLoop = 0;


  hz = 0;

  pressed = false;

  timeReflection = 1000;
  shot = 100;
  error = false;

  scheme = 0;
  counter = 0;
  numButtons = 0;
  flash = true;
}

