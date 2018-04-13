
void mod9() {
  boolean b = true;
  boolean b2 = false;
  digitalWrite(ledPins[10], HIGH);
  while (b) {
    Serial.println("alla ricerca di @");
    for (int i = 0; i < sizeof(buttonPins) / sizeof(buttonPins[0]); i++) {
      if (b2 == true) {
        for (int j = 2; j < (sizeof(ledPins) / sizeof(ledPins[0])) - 2; j++)
        {
          digitalWrite(ledPins[j], HIGH);
        }
      }
      currentButtonsState[i] = debounce(i);
      if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i] && i == 10) {
        Serial.println("@ trovata");
        b2 = true;
        digitalWrite(ledPins[10], LOW);
      } else if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i] && i >= 2 && i <= 9 && b2 == true) {
        timeReflection = 1000 * i;
        b = false;
      }
    }
  }
  for (int j = 2; j < (sizeof(ledPins) / sizeof(ledPins[0])) - 2; j++)
  {
    digitalWrite(ledPins[j], LOW);
  }
  defineSum();
  delay(1000);
  while (true) {
    int buttonPressed = 0;
    response[0] = 0;
    response[1] = 0;
    while (!pressed && !error && timeElapsed() <= timeReflection) {

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
    scheme++;
    delay(100);
    if (scheme >= 8) {
      scores[0] = score;
      scores[1] = 0;
      scheme = 0;
      //clearVariables();
      return;
    }
    Serial.println("cambio schema");
    defineSum();
    delay(1000);
  }
}
void defineSum() {
  addend[0] = random(1, 10);
  addend[1] = random(1, 10);
  Serial.println(); Serial.println(); Serial.println(); Serial.println();
  Serial.print(addend[0]);
  Serial.print("  +  ");
  Serial.println(addend[1]);
  Serial.println(); Serial.println(); Serial.println(); Serial.println();
  solution = addend[0] + addend[1];
  startTime = millis();
}


boolean clickAllButtonsAllows(int n) {
  return ((solution / 10) + 1) <= n;
}

boolean checkResults() {
  return ((response[0]*10 + response[1]) == solution);
}

