#include <Wire.h>

#define PHOTO_RES A0
#define SLAVE_ADDRESS 9

bool isCalibration = false;
bool isReportSent = false;
bool dataSendAborted = false;
bool showData = false;
bool minRangeChecked = false;
bool maxRangeChecked = false;
int minRange = -1;
int maxRange = -1;

int previousValue = -1;
int counter = 0;
int recordsToDo = 0;

const int STEP = 10;
const int DIFF = 10;
const int ARCHIVE_FACTOR = 5;

int getPhotoResValue() {
  return analogRead(PHOTO_RES);
}

void setup() {
  pinMode(PHOTO_RES, INPUT);
  Serial.begin(9600);
  Serial.println("dungeon master");
  Wire.begin();
}

void toggleShowData() {
  showData = !showData;
}

void resetCalibration() {
  isCalibration = false;
  isReportSent = false;
  dataSendAborted = false;
  minRange = -1;
  maxRange = -1;
  maxRangeChecked = false;
  minRangeChecked = false;
  previousValue = -1;
  counter = 0;
  recordsToDo = 0;
  Serial.println("Resetted");
}

void startCalibration() {
  if (minRange == -1) {
    minRange = getPhotoResValue() + DIFF;
    Serial.println("'o' to confirm. LIGHT ON");
  } else if (maxRange == -1) {
    maxRange = getPhotoResValue() - DIFF;
    isCalibration = true;

    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write('r');
    recordsToDo = static_cast<int>((maxRange - minRange + 1) / STEP);
    Wire.write(recordsToDo);
    Serial.print("awaiting records: ");
    Serial.println(recordsToDo);
    Wire.endTransmission(SLAVE_ADDRESS);
    Serial.println(minRange);
    Serial.println(maxRange);
    Serial.println("WATCH OUT!!! DANGEROUS CALIBRATION");
    delay(5000);
  }
}

void processCommand(char cmd) {
  switch (cmd) {
    case 's':
      toggleShowData();
      break;
    case 'c':
      Serial.println("'o' to confirm. LIGHT OFF");
      break;
    case 'r':
      resetCalibration();
      break;
    case 'o':
      startCalibration();
      break;
  }
}

void loop() {
  if (showData) {
    Serial.print("value: ");
    Serial.println(getPhotoResValue());
  }

  if (Serial.available()) {
    char cmd = Serial.read();
    processCommand(cmd);
  }

  if (isCalibration) {
    Wire.beginTransmission(SLAVE_ADDRESS);

    if (!isReportSent) {
      Wire.write("c");
      isReportSent = true;
    }

    if (isReportSent && !dataSendAborted) {
      int res = getPhotoResValue();

      if (res <= minRange && !minRangeChecked) {
        minRangeChecked = true;
      }
      if (res >= minRange && !maxRangeChecked) {
        maxRangeChecked = true;
      }

      if (res - previousValue >= STEP) {
        Serial.print("Value for calibration: ");
        Serial.println(getPhotoResValue());
        Wire.write(static_cast<int>(res / ARCHIVE_FACTOR));
        counter += 1;
        previousValue = res;
      }

      Wire.endTransmission(SLAVE_ADDRESS);

      if (previousValue == -1) {
        previousValue = res;
      }
    }

    if (minRangeChecked && maxRangeChecked && counter >= recordsToDo) {
      isCalibration = false;
      isReportSent = false;
      dataSendAborted = false;

      minRange = -1;
      maxRange = -1;

      Wire.beginTransmission(SLAVE_ADDRESS);
      Wire.write('s');
      Wire.endTransmission(SLAVE_ADDRESS);
      Serial.println("finished... (dies)");
      showData = true;
    }
  }
}
