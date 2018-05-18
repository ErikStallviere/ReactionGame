void thirdModGroup(int dim, int colpi, boolean angolo)
{
  corner = angolo;
  dimensions = dim;
  shot = colpi;
  start();
  boolean led = true;
  int giro = 0;
  while (true) {
    int counter = 0;
    while (!pressed && !error && (timeElapsed() <= timeReflection || !corner)) {
      for (int i = 0; i < dimensions; i++) {
        timerGame = millis() - startTime;
        counter = i;
        if (corner) {
          switch (currentNumber) {
            case 2:
              counter = 8;
              break;
            case 3:
              counter = 9;
              break;
          }
        }

        currentButtonsState[i] = debounce(counter);

        if (currentNumber == 10) {
          currentPedana = digitalRead(buttonPins[10]); //mancanza poi diventa pedanaPin
          if (currentPedana) {
            Serial.println("premutooooo pedana");
            pressed = true;
            break;
          }
          if (timeElapsed() >= timeReflection || (lastButtonsState[i] == LOW && currentButtonsState[i] == HIGH)) {
            Serial.println("errore pedana");
            error = true;
            break;
          }
          lastPedana = currentPedana;

        } else {
          if (currentNumber < 10 && blinkButton) {

            giro++;
            if (giro % 25 == 0) {
              led = !led;
            }
            digitalWrite(ledPins[currentNumber], led);

            delay(5);
          }
          if ((dimensions >= 10 && !blinkButton && lastButtonsState[i] == LOW && currentButtonsState[i] == HIGH && counter == currentNumber) || (blinkButton && timeElapsed() >= timeReflection))
          {

            //Serial.println("premutooooo");

            pressed = true;
            break;
          }
          if ((dimensions >= 10 && (lastButtonsState[i] == LOW && currentButtonsState[i] == HIGH && counter != currentNumber)) || (!corner && timeElapsed() >= timeReflection) || (blinkButton && (lastButtonsState[i] == LOW && currentButtonsState[i] == HIGH))) {

            //Serial.println("erroreeeeeee");

            error = true;
            break;
          }
        }
        lastButtonsState[i] = currentButtonsState[i];
      }
    }
    //stampThirdLCD(colpi);
    if (pressed) {
      digitalWrite(buzzerPin, HIGH);
      delay(delayValue);
      digitalWrite(buzzerPin, LOW);
      score++;
      incrementScore();
    }
    if (error) {
      timeReflection -= 50;
    }
    if (timeReflection < 200) {
      lcd.setCursor(0, 1);

      stampThirdLCD(colpi);
      lcd.print("GAME OVER");
      clearVariables();
      return;
    }
    pressed = false;
    error = false;
    shot--;
    start();
    if (shot <= 0) {
      int n = 0;
      for (int i = 0; i < dimensions; i++)
      {
        n = i;
        if (corner) {
          switch (i) {
            case 2:
              n = 8;
              break;
            case 3:
              n = 9;
              break;
          }
        }
        digitalWrite(ledPins[n], LOW);
      }
      scores[0] = score;
      scores[1] = timerGame;

      //Serial.println("fine");

      stampThirdLCD(colpi);
      clearVariables();
      return;
    }
  }
}
void start() {

  while (currentNumber == lastNumber)
  {


    currentNumber = random(0, dimensions);
    if (corner) {
      switch (currentNumber) {
        case 2:
          currentNumber = 8;
          break;
        case 3:
          currentNumber = 9;
          break;
      }
    }
  }
  int n = 0;
  lastNumber = currentNumber;
  for (int i = 0; i < dimensions; i++)
  {
    n = i;
    if (corner) {
      switch (i) {
        case 2:
          n = 8;
          break;
        case 3:
          n = 9;
          break;
      }
    }
    digitalWrite(ledPins[n], LOW);
  }
  digitalWrite(ledPins[11], LOW);
  if (dimensions == 11) {
    if (currentNumber == 10) {
      digitalWrite(ledPins[2], HIGH);
      digitalWrite(ledPins[3], HIGH);
      digitalWrite(ledPins[11], HIGH);
    } else {
      int blinkPercentual = random(1, 101);
      if (blinkPercentual <= 20) {
        blinkButton = true;
      } else {
        blinkButton = false;
        digitalWrite(ledPins[currentNumber], HIGH);
      }
    }
  } else {
    digitalWrite(ledPins[currentNumber], HIGH);
  }

  delay(200);
  startTime = millis();

}


