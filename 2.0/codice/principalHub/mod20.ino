
void mod20()
{
  dimensions = 10;
  defineSimonGame();
  while (true) {
    for (int i = 0; i < 12; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    for (int i = 0; i < maxCounter; i++) {
      digitalWrite(ledPins[simonRecord[i]], HIGH);
      delay(500);
      digitalWrite(ledPins[simonRecord[i]], LOW);
    }
    while (counter < maxCounter && !error) {
      for (int i = 0; i < 10; i++) {
        
        currentButtonsState[i] = debounce(i);
        if (currentButtonsState[i] == true && lastButtonsState[i] != currentButtonsState[i]) {
          if (i == simonRecord[counter]) {
            //Serial.println("RIGHT---------------");
            counter++;
            
          }
          else {
            //Serial.println("WRONG---------------");
            error = true;
            
          }
          //Serial.println("FINE IF");
        }
        lastButtonsState[i] = currentButtonsState[i];
      }
      timerGame = millis() - timer;
    }

    if (maxCounter >= 20 || error)
    {
      scores[0] = maxCounter - 4;
      scores[1] = timerGame;
      //clearVariables();
      return;
    }
    error = false;
    delay(100);
    maxCounter++;
    counter = 0;
  }
}

void defineSimonGame() {
  for (int i = 0; i < 20; i++) {
    setNextButton(false);
    simonRecord[i] = currentNumber;
    Serial.print("Lista: ");
    Serial.println(simonRecord[i]);
  }
}

