

boolean debounce(int n)
{
  boolean current = (digitalRead(buttonPins[n]));
  if (lastButtonsState[n] != current)
  {
    delay(1);
    current = digitalRead(buttonPins[n]);
  }
  return current;
}

void lightLed()
{
  while (true)
  {
    currentNumber = random(0, dimensions);

    if (currentNumber != lastNumber)
    {
      break;
    }
  }
  lastNumber = currentNumber;

  for (int i = 0; i < dimensions; i++)
  {
    digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(ledPins[currentNumber], HIGH);
}
