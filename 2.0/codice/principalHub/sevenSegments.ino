

void reload7Segments(boolean last) {
  Serial.println("7segmentireload");
  digitalWrite(dueP, HIGH);
  for (int dig = 4; dig > 0; dig--) {
    numero(dig);
    Serial.print("dig: ");
    Serial.println(dig);
    if (dig == 4) {
      stampa((int)(timerGame / 1000) % 10, tempo);
      Serial.println();
      stampa(score % 10, punti);
    }
    else if (dig == 3) {
      stampa(((int)(timerGame / 1000) / 10) % 6, tempo);
      Serial.println();
      stampa((score / 10) % 10, punti);
    }
    else if (dig == 2) {
      stampa((int)((timerGame / 1000) / 60) % 10, tempo);
      Serial.println();
      stampa((score / 100) % 10, punti);
    }
    else {
      stampa(((int)((timerGame / 1000 / 60) / 10) % 6), tempo);
      Serial.println();
      stampa((score / 1000) % 10, punti);
    }
    Serial.print("SCORE: ");
    Serial.println(score);
    Serial.print("TEMPO: ");
    Serial.println(timerGame / 1000);
    delayMicroseconds(500);
    if (!last) {
      Serial.println("NO END");
      spegni();
    }

  }
}

void stampa(int nrDisplay, int arrayP[]) {
Serial.println("stampa");
  switch (nrDisplay) {

    case 0:
    Serial.println("Disegno uno 0");
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], HIGH);
      digitalWrite(arrayP[1], HIGH);
      digitalWrite(arrayP[5], HIGH);
      digitalWrite(arrayP[3], LOW);
      break;

    case 1:
      digitalWrite(arrayP[4], LOW);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], LOW);
      digitalWrite(arrayP[1], LOW);
      digitalWrite(arrayP[5], LOW);
      digitalWrite(arrayP[3], LOW);
      break;

    case 2:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], LOW);
      digitalWrite(arrayP[0], HIGH);
      digitalWrite(arrayP[1], HIGH);
      digitalWrite(arrayP[5], LOW);
      digitalWrite(arrayP[3], HIGH);
      break;

    case 3:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], HIGH);
      digitalWrite(arrayP[1], LOW);
      digitalWrite(arrayP[5], LOW);
      digitalWrite(arrayP[3], HIGH);
      break;

    case 4:
      digitalWrite(arrayP[4], LOW);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], LOW);
      digitalWrite(arrayP[1], LOW);
      digitalWrite(arrayP[5], HIGH);
      digitalWrite(arrayP[3], HIGH);
      break;

    case 5:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], LOW);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], HIGH);
      digitalWrite(arrayP[1], LOW);
      digitalWrite(arrayP[5], HIGH);
      digitalWrite(arrayP[3], HIGH);
      break;

    case 6:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], LOW);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], HIGH);
      digitalWrite(arrayP[1], HIGH);
      digitalWrite(arrayP[5], HIGH);
      digitalWrite(arrayP[3], HIGH);
      break;

    case 7:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], LOW);
      digitalWrite(arrayP[1], LOW);
      digitalWrite(arrayP[5], LOW);
      digitalWrite(arrayP[3], LOW);
      break;

    case 8:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], HIGH);
      digitalWrite(arrayP[1], HIGH);
      digitalWrite(arrayP[5], HIGH);
      digitalWrite(arrayP[3], HIGH);
      break;

    case 9:
      digitalWrite(arrayP[4], HIGH);
      digitalWrite(arrayP[6], HIGH);
      digitalWrite(arrayP[2], HIGH);
      digitalWrite(arrayP[0], HIGH);
      digitalWrite(arrayP[1], LOW);
      digitalWrite(arrayP[5], HIGH);
      digitalWrite(arrayP[3], HIGH);
      break;
  }
}

void numero(int digit) {
  switch (digit) {
    case 1:
      digitalWrite(dig1, LOW);
      break;
    case 2:
      digitalWrite(dig2, LOW);
      break;
    case 3:
      digitalWrite(dig3, LOW);
      break;
    case 4:
      digitalWrite(dig4, LOW);
      break;
  }
}
void spegni() {
  Serial.println("SPENGOOO");
  digitalWrite(dig1, HIGH);
  digitalWrite(dig2, HIGH);
  digitalWrite(dig3, HIGH);
  digitalWrite(dig4, HIGH);
}
