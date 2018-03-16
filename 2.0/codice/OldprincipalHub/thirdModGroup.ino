
void thirdModGroup()
{

  if (firstCicle)
  {
    start();
    firstCicle = false;
  }
  while (pressed == false && error == false && timeElapsed() <= timeReflection) {
    for (int i = 0; i < sizeof(buttonPins) / sizeof(buttonPins[i]); i++) {
      currentButtonsState[i] = debounce(i);
      if (lastButtonsState[i] == LOW && currentButtonsState[i] == HIGH && i == currentNumber)
      {
        pressed = true;
      }
      if (lastButtonsState[i] == LOW &&  currentButtonsState[i] == HIGH && i != currentNumber) {
        error = true;
      }
      lastButtonsState[i] = currentButtonsState[i];
    }
  }
  if (pressed == true) {
    digitalWrite(buzzerPin, HIGH);
    delay(delayValue);
    digitalWrite(buzzerPin, LOW);
    score++;
    Serial.println(score);
  }
  if (error == true) {
    timeReflection -= 50;
  }
  pressed = false;
  error = false;
  shot--;
  start();
  if (shot <= 0) {
    for (int i = 0; i < sizeof(ledPins) / sizeof(ledPins[0]); i++)
    {
      digitalWrite(ledPins[i], LOW);
    }
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(buzzerPin2, HIGH);
    delay(25 * delayValue);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(buzzerPin2, LOW);
    exit(0);
  }
}



void start() {

  boolean a = true;
  while (a)
  {
    currentNumber = random(0, 4);

    if (currentNumber != lastNumber)
    {
      a = false;
    }
  }
  lastNumber = currentNumber;
  for (int i = 0; i < sizeof(ledPins) / sizeof(ledPins[0]); i++)
  {
    digitalWrite(ledPins[i], LOW);
  }

  digitalWrite(ledPins[currentNumber], HIGH);
  //delay(1000);
  startTime = millis();
  Serial.println(startTime);

}

float timeElapsed() {
  elapsedTime = (millis() - startTime);
  elapsedTime = elapsedTime;
  return elapsedTime;
}

