
void mod6() {
  dimensions = 10;
  setNextButton(true);
  while (true) {
    timeReflection = 1400 - scheme * 100;
    buttonCounter = 0;
    scheme++;
    Serial.print("timeReflection: ");
    Serial.println(timeReflection);
    while (buttonCounter <= 30) {
      Serial.println(buttonCounter);
      while (!pressed && !error) {
        for (int i = 0; i < 10; i++) {
          currentButtonsState[i] = debounce(i);
          if (timeElapsed() >= timeReflection) {
            Serial.println("TEMPO SCADUTO----------------------");
            error = true;
            break;
          }
          if (currentButtonsState[i] == true && lastButtonsState[i] != currentButtonsState[i]) {
            buttonCounter++;
            if (i == currentNumber) {
              pressed = true;
              Serial.println("RIGHT----------------------");
            } else {
              error = true;
              Serial.println("WRONG----------------------");
            }
          }
          lastButtonsState[i] = currentButtonsState[i];
        }
      }
      Serial.println("ESC");
      if (error) {
        errorCounter++;
      } else if (pressed) {
        errorCounter = 0;
      }
      delay(150);
      setNextButton(true);
      pressed = false;
      error = false;
      if (errorCounter >= 3) {
        Serial.print("errori: ");
        Serial.println(errorCounter);
        scores[0] = scheme;
        scores[1] = 0;
        scheme = 0;
        //clearVariables();
        return;
      }
    }
    Serial.println(); Serial.println(); Serial.println(); Serial.println();
    Serial.println("NUOVO SCHEMA"); Serial.println(); Serial.println(); Serial.println(); Serial.println();
    delay(100);
    if (scheme >= 10) {
      Serial.print("schema: ");
      Serial.println(scheme);
      scores[0] = scheme;
      scores[1] = 0;
      scheme = 0;
      //clearVariables();
      return;
    }
    delay(1000);
  }
}

