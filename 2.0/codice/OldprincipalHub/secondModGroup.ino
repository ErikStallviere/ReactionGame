
void secondModGroup()
{
  if (firstCicle)
  {
    timer = millis();
    lightLed();
    firstCicle = false;
  }

  for (int i = 0; i < sizeof(buttonPins) / sizeof(buttonPins[0]); i++)
  {
    currentButtonsState[i] = debounce(i);
    digitalRead(buttonPins[currentNumber]);

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
    Serial.println(millis() - timer);
    delay(1);

    if (millis() - timer >= countdown || score == 50)
    {
      Serial.println("Completed");
      for (int i = 0; i < sizeof(ledPins) / sizeof(ledPins[0]); i++)
      {
        digitalWrite(ledPins[i], LOW);
      }
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(buzzerPin2, HIGH);
      delay(25 * delayValue);
      digitalWrite(buzzerPin, LOW);
      digitalWrite(buzzerPin2, LOW);
      return;
    }
  }
}
