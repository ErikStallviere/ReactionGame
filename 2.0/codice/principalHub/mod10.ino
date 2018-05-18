
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
  //Serial.print("num scelto: ");
  //Serial.println(numChosen);
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
          //Serial.print("bottoni: ");
          //Serial.println(buttonPressed);
          //Serial.print(response[0]);
          //Serial.println(response[1]);
        }
        if (clickAllButtonsAllows(buttonPressed)) {
          //Serial.println("BOTTONI PREMUTI");
          //Serial.println(solution);
          if (checkResults())
          {
            pressed = true;
            digitalWrite(buzzerPin, HIGH);
            delay(delayValue);
            digitalWrite(buzzerPin, LOW);
            score++;
            incrementScore();
            //Serial.println("RIGHT--------");
          } else {
            error = true;
            //Serial.println("WRONG--------");
          }
          break;
        }
        lastButtonsState[i] = currentButtonsState[i];
      }
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
      clearVariables();
      return;
    }
  }
}

void defineProduct() {
  setNextButton(false);
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print(numChosen);
  lcd.setCursor(4, 2);
  lcd.print(" * ");
  lcd.setCursor(7, 2);
  lcd.print(currentNumber);
  solution = numChosen * currentNumber;
  startTime = millis();
}




