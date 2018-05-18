boolean reload = true;
void firstModGroup(int d, long t, boolean octave)
{
  dimensions = d;
  countdown = t;
  boolean led = HIGH;
  timer = millis();
  setNextButton(true);
  while (true) {
    for (int i = 0; i < dimensions; i++)
    {
      currentButtonsState[i] = debounce(i);

      if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i] && i == currentNumber)
      {
        score++;
        digitalWrite(scorePin, HIGH);
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(buzzerPin2, HIGH);
        delay(delayValue);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(buzzerPin2, LOW);
        setNextButton(true);
      }

      lastButtonsState[i] = currentButtonsState[i];
      delay(1);
      timerGame = millis() - timer;
      if (timerGame >= countdown)
      {
        for (int i = 0; i < dimensions; i++)
        {
          digitalWrite(ledPins[i], LOW);
        }
        if (octave) {
          digitalWrite(buzzerPin, HIGH);
          digitalWrite(buzzerPin2, HIGH);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("SCAMBIATI CON UN ");
          lcd.setCursor(0, 1);
          lcd.print("ALTRO GIOCATORE");
          delay(3000);
          digitalWrite(buzzerPin, LOW);
          digitalWrite(buzzerPin2, LOW);
          countdownLoop++;

          setNextButton(true);
          if (countdownLoop >= 4) {
            for (int i = 0; i < dimensions; i++)
            {
              digitalWrite(ledPins[i], LOW);
            }
            scores[0] = score;
            scores[1] = timerGame;
            delay(100);
            clearVariables();

            return;
          }
          hz = 0;
          timer = millis();
        } else {
          scores[0] = score;
          scores[1] = timerGame;
          delay(100);
          clearVariables();

          return;
        }
      }
    }
  }
}

