
void setNextButton23()
{
  while (true)
  {
    int n = random(0, dimensions);
    Serial.print("num casuale: ");
    Serial.println(n);
    currentNumber = chosenOnesNumbers[n];
    //Serial.print("num corrispondente: ");
    //Serial.println(currentNumber);
    if (currentNumber >= 0 && currentNumber <= 11) {
      //Serial.println(chosenOnes[currentNumber]);
      if (!chosenOnes[currentNumber])
      {
        Serial.println("CHECKED------------------");
        chosenOnes[currentNumber] = true;
        break;
      }
    }
  }

  for (int i = 0; i < 12; i++)
  {
    digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(ledPins[currentNumber], HIGH);
  //Serial.println(currentNumber);
  startTime = millis();
  return;
}



int countChosenOnes() {
  int c = 0;
  for (int i = 0; i < 11; i++) {
    if (chosenOnes[i]) {
      c++;
    }
  }
  return c;
}

void mod23()
{
  timer = millis();
  digitalWrite(ledPins[11], HIGH);
  while (!debounce(11)) {
    Serial.println("Prema #");
  }
  digitalWrite(ledPins[11], LOW);
  delay(1000);
  boolean finished = false;
  while (!finished) {
    for (int i = 0; i < 12; i++) {
      currentButtonsState[i] = debounce(i);

      if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i] && i != 11)
      {
        chosenOnes[i] = !chosenOnes[i];
        //Serial.println(i);
        //Serial.println(chosenOnes[i]);
        digitalWrite(ledPins[i], chosenOnes[i]);
      }
      if (countChosenOnes() >= 1 && debounce(11)) {
        finished = true;
      }
      lastButtonsState[i] = currentButtonsState[i];
    }
  }
  dimensions = countChosenOnes();
  for (int i = 0; i < dimensions; i++) {
    for (int j = 0; j < 11; j++) {
      if (chosenOnes[j]) {
        chosenOnesNumbers[i] = j;
        Serial.print("chosen: ");
        Serial.println(j);
        chosenOnes[j] = false;
        break;
      }

    }
  }
  Serial.println("START");
  for (int i = 0; i < 12; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  setNextButton23();
  finished = false;
  while (true) {

    Serial.println("Primo ciclo");
    while (!finished) {
      //Serial.println("Secondo ciclo");
      for (int i = 0; i < 11; i++)
      {
        currentButtonsState[i] = debounce(i);
        //Serial.print("current: ");
        //Serial.println(i);
        //Serial.print(": ");
        //Serial.println(currentButtonsState[i]);
        if (currentButtonsState[i] == true && currentButtonsState[i] != lastButtonsState[i] && i == currentNumber)
        {
          score++;
          Serial.println(i);
          Serial.println("CORRETTO---------------");
          if (score % dimensions == 0) {
            Serial.println("finished");
            finished = true;
          } else {
            setNextButton23();
          }
          digitalWrite(buzzerPin, HIGH);
          digitalWrite(buzzerPin2, HIGH);
          delay(delayValue);
          delay(1);
        }
        lastButtonsState[i] = currentButtonsState[i];
      }
      Serial.print("score:");
      Serial.println(score);
      Serial.print("dim:");
      Serial.println(dimensions);

    }
    finished = false;

    timerGame = millis() - timer;
    scheme++;
    Serial.print("schema:  ");
    Serial.println(scheme);
    if (scheme >= 10)
    {
      for (int i = 0; i < 12; i++)
      {
        digitalWrite(ledPins[i], LOW);
      }
      scores[0] = score;
      scores[1] = timerGame;
      clearVariables();
      return;
    }
    Serial.println("rewind-----------------");
    for (int i = 0; i < 11; i++) {
      chosenOnes[i] = false;
    }
    setNextButton23();
  }
}
