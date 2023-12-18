#include Wire.h

#define PHOTO_RES_IN A0
#define I2C_ADDRES 9
#define LED_PIN 13

int max_records = 0;
int currentRecord = 0;
int x;
int y;
bool calibrationStarted = false;
bool showData = false;

long lastTime = 0;
bool state = HIGH;

int getMyPhotoResValue(){
  return analogRead(PHOTO_RES_IN);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println();
  Serial.println(fuckng Slave);
  Wire.begin(I2C_ADDRES);
  Wire.onReceive(processData);
}

void loop() {
  if (calibrationStarted) {
      blinkLed(lastTime, state);
    }
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'r') {
      int currentRecord = 0;
      calibrationStarted = false;
    }
    if (cmd == 's') {
      showData = !showData;
    }
  }
  if (showData) {
    if (max_records != 0) {
      Serial.print(Calibrated value );
      Serial.println(linearInterpolation(getMyPhotoResValue()));
    } else {
      Serial.print(Raw value );
      Serial.println(getMyPhotoResValue());
    }
  }
}

float linearInterpolation(int x_new) {
    int i = 0;
    while (i  currentRecord - 1 && x[i + 1]  x_new) {
        i++;
    }
    int x0 = x[i];
    int y0 = y[i];
    int x1 = x[i + 1];
    int y1 = y[i + 1];
    return y0 + (y1 - y0)  (x_new - x0)  (x1 - x0);
}

void processData(int length){
  int last_val = 0;
  while (0  Wire.available()){
    auto result = Wire.read();
    if (result == 'r') {
      max_records = Wire.read();
      Serial.print(Max records );
      Serial.println(max_records);
      x = new int[max_records];
      y = new int[max_records];
    } else if (result == 's') {
      Serial.println(Calibration done);
      calibrationStarted = false;
      digitalWrite(LED_PIN, HIGH);
      showData = true;
      return;
    } else if (result == 'c') {
      currentRecord = 0;
      calibrationStarted = true;
      Serial.println(Calibration started);
    } else {
      if (currentRecord  max_records && calibrationStarted){
        int data = result  5;
        int currentResistence = getMyPhotoResValue();
        x[currentRecord] = currentResistence;
        y[currentRecord] = data;
        currentRecord+=1;
        Serial.println(Calibrating...);
       }
    }
  }
}

void blinkLed(long &lastTime, bool &state) {
  long currentTime = millis();
  if (currentTime - lastTime  100) {
    digitalWrite(LED_PIN, state);
    lastTime = currentTime;
    state = !state;
  }
}