
void firstModGroup(int d, long t, boolean octave)
{
  while (true) {
    if (firstCicle)
    {
      dimensions = d;
      countdown = t;
      boolean led = HIGH;
      timer = millis();
      lightLed();
      firstCicle = false;
    }

    for (int i = 0; i < dimensions; i++)
    {
      currentButtonsState[i] = debounce(i);
      if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i] && i == currentNumber)
      {
        score++;

        digitalWrite(buzzerPin, HIGH);
        digitalWrite(buzzerPin2, HIGH);
        delay(delayValue);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(buzzerPin2, LOW);
        lightLed();
      }

      lastButtonsState[i] = currentButtonsState[i];
      delay(1);
      timerGame = (millis() - timer) / 1000;
      if ((millis() - timer) >= countdown)
      {
        for (int i = 0; i < dimensions; i++)
        {
          digitalWrite(ledPins[i], LOW);
        }
        if (octave) {
          digitalWrite(buzzerPin, HIGH);
          digitalWrite(buzzerPin2, HIGH);
          delay(2000);
          digitalWrite(buzzerPin, LOW);
          digitalWrite(buzzerPin2, LOW);
          countdownLoop++;

          lightLed();
          if (countdownLoop >= 4) {
            for (int i = 0; i < dimensions; i++)
            {
              digitalWrite(ledPins[i], LOW);
            }
            scores[0] = score;
            scores[1] = timerGame;
            clearVariables();
            return;
          }
          timer = millis();
        } else {
          scores[0] = score;
          scores[1] = timerGame;
          clearVariables();
          return;
        }
      }
    }
    
    if (true) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Tempo: ");

      lcd.setCursor(0, 2);
      lcd.print("Punteggio: ");

      lcd.setCursor(0, 1);
      lcd.print(timerGame);

      lcd.setCursor(0, 3);
      lcd.print(score);
    }
  }
}
