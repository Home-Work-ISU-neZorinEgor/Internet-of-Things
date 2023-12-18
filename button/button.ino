const int BUTTON_PIN = 2;
const int BOUNCE_TIME = 50;
const int PRESSED = LOW;

volatile bool buttonPressed = false;
volatile int counter = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  attachInterrupt(0, handleButtonClick, FALLING);

  // Timer setup
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 3125;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12); 
  TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
  if (buttonPressed && digitalRead(BUTTON_PIN) == PRESSED) {
    counter++;
    buttonPressed = false;
  }
}

void loop() {
  delay(1000);
  if (counter > 0) {
    Serial.println(counter);
    counter = 0;
  }
}

void handleButtonClick() {
  if (!buttonPressed) {
    buttonPressed = true;
  }
}
