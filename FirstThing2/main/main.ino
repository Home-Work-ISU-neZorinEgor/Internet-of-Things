#include "button_calibrator.h"
#include "timer_setup.h"

void printCount();

void setup() {
  Serial.begin(9600);
  setupBtn();
  setupTimer();
}

void loop() {
  delay(1000);
  printCount();
}

void printCount() {
  if (count > 0) {
    Serial.println(count);
    count = 0;
  }
}
