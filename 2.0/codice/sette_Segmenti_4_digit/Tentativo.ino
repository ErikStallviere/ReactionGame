const int dig4 = 13;
const int dig3 = 12;
const int dig2 = 11;
const int dig1 = 10;

int dueP = 53;

/*
const int sDx = 9;
const int sSx = 8;
const int s = 7;
const int c = 6;
const int bDx = 5;
const int bSx = 4;
const int b = 3;
*/

int tempo[] = {52,51,50,49,48,47,46};
int punti[] = {3,4,5,6,7,8,9};

int punteggio = 1;

int start;

void setup() {
  pinMode(dig4, OUTPUT);
  pinMode(dig3, OUTPUT);
  pinMode(dig2, OUTPUT);
  pinMode(dig1, OUTPUT);
  
  for(int i = 0; i < sizeof(tempo); i++){
    pinMode(tempo[i], OUTPUT);
    pinMode(punti[i], OUTPUT);
  }  
  pinMode(dueP, OUTPUT);

  digitalWrite(dig4, HIGH);
  digitalWrite(dig3, HIGH);
  digitalWrite(dig2, HIGH);
  digitalWrite(dig1, HIGH);
  
  start = millis()/1000;
}

void loop() {
  timer(start);
}

void timer(int inizio){
  digitalWrite(dueP, HIGH);
  
  while(true){
    int ora = (millis()/1000); 

    for(int dig = 4; dig > 0; dig--){
      numero(dig);    
  
      if(dig == 4){
        stampa((ora -start)%10, tempo);
        stampa(punteggio%10, punti); 
      }
      else if(dig == 3){
        stampa(((ora -start)/10)%6, tempo);
        stampa((punteggio/10)%10, punti);  
      }
      else if(dig == 2){
        stampa(((ora -start)/60)%10, tempo);
        stampa((punteggio/100)%10, punti);  
      }
      else{
        stampa((((ora -start)/60)/10)%6, tempo);
        stampa((punteggio/1000)%10, punti);
      }

      delayMicroseconds(5500);

      spegni();
    }
  }
}

void stampa(int nrDisplay, int arrayP[]){

  switch (nrDisplay){
  
  case 0:
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

void numero(int digit){
  switch(digit) {
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
void spegni(){
  digitalWrite(dig1, HIGH);
  digitalWrite(dig2, HIGH);
  digitalWrite(dig3, HIGH);
  digitalWrite(dig4, HIGH);  
}
