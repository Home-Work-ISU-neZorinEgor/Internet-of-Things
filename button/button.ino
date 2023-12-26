const int PIN = 3;
const int PRESSED_STATE = LOW;
volatile bool btnPressed = false; //по умолчанию
const int BTN_BOUNCE = 50; //задержка
volatile int count = 0;

void setupBtn() {
  pinMode(PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN), []() { btnPressed = true; }, FALLING);
}

void setupTimer() {
  TCCR1B = (1 << WGM12) | (1 << CS12);
  OCR1A = 3125;
  TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
  if (btnPressed && digitalRead(PIN) == PRESSED_STATE) {
    count++;
    btnPressed = false;
  }
}

void printCount() {
  if (count > 0) {
    Serial.println(count);
    count = 0;
  }
}

void setup() {
  Serial.begin(9600);
  setupBtn();
  setupTimer();
}

void loop() {
  delay(1000);
  printCount();
}
