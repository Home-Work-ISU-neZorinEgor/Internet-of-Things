#include <Wire.h>

#define PHOTO_RES_IN A0
#define I2C_ADDRESS 5
#define LED_PIN 13

int maxRecords = 0;
int currentRecord = 0;
int* xValues;
int* yValues;
bool calibrationStarted = false;
bool showData = false;

long lastTime = 0;
bool ledState = HIGH;

int getPhotoResValue(){
  return analogRead(PHOTO_RES_IN);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("");
  Serial.println("freaking Slave");
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(processData);
}

void loop() {
  if (calibrationStarted) {
    blinkLed(lastTime, ledState);
  }
  if (Serial.available()) {
    char cmd = Serial.read();
    switch (cmd) {
      case 'r':
        currentRecord = 0;
        calibrationStarted = false;
        break;
      case 's':
        showData = !showData;
        break;
    }
  }
  if (showData) {
    if (maxRecords != 0) {
      Serial.print("Calibrated value: ");
      Serial.println(linearInterpolation(getPhotoResValue()));
    } else {
      Serial.print("Raw value: ");
      Serial.println(getPhotoResValue());
    }
  }
}

float linearInterpolation(int xNew) {
    int i = 0;
    while (i < currentRecord - 1 && xValues[i + 1] < xNew) {
        i++;
    }
    int x0 = xValues[i];
    int y0 = yValues[i];
    int x1 = xValues[i + 1];
    int y1 = yValues[i + 1];
    return y0 + (y1 - y0) * (xNew - x0) / (x1 - x0);
}

void processData(int length){
  int lastVal = 0;
  while (0 < Wire.available()){
    auto result = Wire.read();
    switch (result) {
      case 'r':
        maxRecords = Wire.read();
        Serial.print("Max records: ");
        Serial.println(maxRecords);
        xValues = new int[maxRecords];
        yValues = new int[maxRecords];
        break;
      case 's':
        Serial.println("Calibration done");
        calibrationStarted = false;
        digitalWrite(LED_PIN, HIGH);
        showData = true;
        return;
      case 'c':
        currentRecord = 0;
        calibrationStarted = true;
        Serial.println("Calibration started");
        break;
      default:
        if (currentRecord < maxRecords && calibrationStarted){
          int data = result * 5;
          int currentResistence = getPhotoResValue();
          xValues[currentRecord] = currentResistence;
          yValues[currentRecord] = data;
          currentRecord += 1;
          Serial.println("Calibrating...");
        }
        break;
    }
  }
}

void blinkLed(long &lastTime, bool &state) {
  long currentTime = millis();
  if (currentTime - lastTime > 100) {
    digitalWrite(LED_PIN, state);
    lastTime = currentTime;
    state = !state;
  }
}
