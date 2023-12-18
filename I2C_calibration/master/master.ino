#include <Wire.h>


const int PHOTO_RES = A0;
const int SLAVE_ADDRESS = 9;


const int STEP = 10;
const int DIFF = 10;
const int ARCHIVE_FACTOR = 5;

// Calibration state enum
enum CalibrationState {
  NOT_CALIBRATING,
  CALIBRATING_MIN,
  CALIBRATING_MAX
};


int photoResValue = -1;
int minRangeThreshold = -1;
int maxRangeThreshold = -1;
bool minRangeChecked = false;
bool maxRangeChecked = false;
int previous = -1;
int cntr = 0;
int recordsToDo = 0;
bool isCalibration = false;
bool isReportSent = false;
bool dataSendAborted = false;
bool showData = false;

void setup() {
  pinMode(PHOTO_RES, INPUT);
  Serial.begin(9600);
  Serial.println("Dungeon Master");
  Wire.begin();
}

void switchShowData() {
  showData = !showData;
}

void handleSerialInput() {
  if (Serial.available()) {
    char cmd = Serial.read();

    switch (cmd) {
      case 's':
        switchShowData();
        break;
      case 'c':
        Serial.println("'o' to confirm. LIGHT OFF");
        break;
      case 'r':
        // Reset all calibration variables
        isCalibration = false;
        isReportSent = false;
        dataSendAborted = false;
        minRangeThreshold = -1;
        maxRangeThreshold = -1;
        maxRangeChecked = false;
        minRangeChecked = false;
        previous = -1;
        cntr = 0;
        recordsToDo = 0;
        Serial.println("Resetted");
        break;
      case 'o':
        if (minRangeThreshold == -1) {
          minRangeThreshold = photoResValue + DIFF;
          Serial.println("'o' to confirm. LIGHT ON");
        } else if (maxRangeThreshold == -1) {
          maxRangeThreshold = photoResValue - DIFF;
          isCalibration = true;

          Wire.beginTransmission(SLAVE_ADDRESS);
          Wire.write('r');
          recordsToDo = static_cast<int>((maxRangeThreshold - minRangeThreshold + 1) / STEP);
          Wire.write(recordsToDo);
          Serial.print("Awaiting records: ");
          Serial.println(recordsToDo);
          Wire.endTransmission(SLAVE_ADDRESS);

          Serial.println(minRangeThreshold);
          Serial.println(maxRangeThreshold);
          Serial.println("WATCH OUT!!! DANGEROUS CALIBRATION");
          delay(5000);
        }
        break;
    }
  }
}

void loop() {
  if (showData) {
    Serial.print("Value: ");
    Serial.println(photoResValue);
  }

  handleSerialInput();

  if (isCalibration) {
    Wire.beginTransmission(SLAVE_ADDRESS);

    if (!isReportSent) {
      Wire.write("c");
      isReportSent = true;
    }

    if (isReportSent && !dataSendAborted) {
      int res = photoResValue;

      if (res <= minRangeThreshold && !minRangeChecked) {
        minRangeChecked = true;
      }

      if (res >= minRangeThreshold && !maxRangeChecked) {
        maxRangeChecked = true;
      }

      if (res - previous >= STEP) {
        Serial.print("Value for calibration: ");
        Serial.println(photoResValue);
        Wire.write(static_cast<int>(res / ARCHIVE_FACTOR));
        cntr += 1;
        previous = res;
      }

      Wire.endTransmission(SLAVE_ADDRESS);

      if (previous == -1) {
        previous = res;
      }
    }

    if (minRangeChecked && maxRangeChecked && cntr >= recordsToDo) {
      isCalibration = false;
      isReportSent = false;
      dataSendAborted = false;

      minRangeThreshold = -1;
      maxRangeThreshold = -1;

      Wire.beginTransmission(SLAVE_ADDRESS);
      Wire.write('s');
      Wire.endTransmission(SLAVE_ADDRESS);
      Serial.println("Finished... (Dies)");
      showData = true;
    }
  }
}
