const int PIN_TRIG = 2;
const int PIN_ECHO = 3;

long duration, distanceInCm;

int minRange = 0;
int maxRange = 0;
double* values;
bool isMeasurementShown = false;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
}

void loop() {
  if (minRange == 0 || maxRange == 0) {
    readRange();
  } else {
    performMeasurement();
  }
}

void readRange() {
  Serial.println("Enter range:");

  minRange = readIntFromSerial();
  maxRange = readIntFromSerial();

  values = new double[maxRange - minRange]();
}

int readIntFromSerial() {
  int result = 0;
  while (Serial.available() > 0) {
    char digit = Serial.read();
    if (isdigit(digit)) {
      result = result * 10 + (digit - '0');
    } else if (digit == '\n') {
      break;
    }
  }
  return result;
}

void performMeasurement() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  duration = pulseIn(PIN_ECHO, HIGH);

  distanceInCm = (duration / 2) / 29.1;

  if (checkIsEnough()) {
    if (!isMeasurementShown) {
      sendMeasurement();
      isMeasurementShown = true;
    }
  } else {
    if (distanceInCm >= minRange && distanceInCm <= maxRange) {
      double val = analogRead(A0);
      val = val * 5.0 / 2046.0;
      values[distanceInCm - minRange] = val;

      Serial.print("Distance to object: ");
      Serial.print(distanceInCm);
      Serial.print(" cm. ");
      Serial.println(val);
    } else {
      Serial.print("Distance outside permissible range: ");
      Serial.print(minRange);
      Serial.print(", ");
      Serial.println(maxRange);
    }
  }
  delay(1000);
}

bool checkIsEnough() {
  int lastNotZero = 0;
  double count = 0.0;
  for (int i = 0; i < maxRange - minRange; i++) {
    if (values[i] != 0) {
      count++;
    } else {
      if (i - lastNotZero >= 2) {
        return false;
      }
      lastNotZero = i;
    }
  }
  return count / (maxRange - minRange) >= 0.9;
}

void sendMeasurement() {
  Serial.println("message");
  Serial.println(maxRange - minRange);
  for (int i = 0; i < maxRange - minRange; i++) {
    String stringValue = String(values[i], 3);
    Serial.println(stringValue);
    Serial.println(i + minRange);
  }
}
