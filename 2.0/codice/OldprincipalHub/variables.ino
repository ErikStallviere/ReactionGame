
//Bottoni comuni a tutte le mod
int buttonPins[] = {22, 24, 38, 40};
//Led comuni a tutte le mod
int ledPins[] = {23, 25, 39, 41};
long arraySelected[2];
int arrayFinal[2];
long val;
int modSelected;

/**
   Array contenente il vecchio stato dei bottoni.
*/
boolean lastButtonsState[12];

/**
   Array contenente lo stato attuale dei bottoni.
*/
boolean currentButtonsState[12];


/**
   Array contenente i pins dei bottoni.
*/
int buttonPins[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44};
/**
   Array contenente i pins dei led
*/

int buzzerPin = 7;
int buzzerPin2 = 6;
int delayValue = 20;

int lastNumber = -1;
int currentNumber = -1;
float timer = 0;
int dimensions = 0;
int score = 0;

long buttons[] = {16738455, 16724175, 16718055, 16743045, 16716015, 16726215, 16734885, 16728765, 16730805, 16732845, 16748655};

int scores[] = {0, 0};


/**
   Il numero di millisecondi mancanti alla fine del gioco.
*/
long countdown = 0;


int countdownLoop = 0;



double timerGame = 0;

/**
   Booleano che verifica se si è presenti nel primo ciclo.
*/
boolean firstCicle = true;
boolean pressed = false;
float startTime;
float elapsedTime;
float timeReflection = 1000;
int score = 0;
int shot = 100;
boolean error = false;
int buzzerPin = 6;
int buzzerPin2 = 7;
int delayValue = 20;


long selectedTime = 1000;

int chosenNumbers[6] = { -1, -1, -1, 1, -1, -1};

int discardedNumbers[9] = { -1, -1, -1, 1, -1, -1, -1, -1, -1};

boolean pressedNumbers[10];
int scheme = 0;
int counter = 0;
int numButtons = 0;
boolean flash = true;

