#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include "HX711.h"

#define BUTTON_PIN 2
#define MOTOR_PIN  3

#define XSHUT_1 4
#define XSHUT_2 5

#define PIEZO_PIN A0

#define HX_DT  A1
#define HX_SCK A2

Adafruit_VL53L0X tof1 = Adafruit_VL53L0X();
Adafruit_VL53L0X tof2 = Adafruit_VL53L0X();
HX711 scale;

long weight;
int piezoValue;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);

  pinMode(XSHUT_1, OUTPUT);
  pinMode(XSHUT_2, OUTPUT);

  digitalWrite(XSHUT_1, LOW);
  digitalWrite(XSHUT_2, LOW);
  delay(10);

  digitalWrite(XSHUT_1, HIGH);
  delay(10);
  if (!tof1.begin(0x30)) {
    Serial.println("TOF 1 failed");
    while (1);
  }

  digitalWrite(XSHUT_2, HIGH);
  delay(10);
  if (!tof2.begin(0x31)) {
    Serial.println("TOF 2 failed");
    while (1);
  }

  scale.begin(HX_DT, HX_SCK);
  scale.set_scale();   
  scale.tare();

  Serial.println("System ready");
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button pressed");

    VL53L0X_RangingMeasurementData_t measure1;
    VL53L0X_RangingMeasurementData_t measure2;

    tof1.rangingTest(&measure1, false);
    tof2.rangingTest(&measure2, false);

    if (measure1.RangeStatus == 0)
      Serial.print("TOF1 (mm): "), Serial.println(measure1.RangeMilliMeter);
    else
      Serial.println("TOF1 out of range");

    if (measure2.RangeStatus == 0)
      Serial.print("TOF2 (mm): "), Serial.println(measure2.RangeMilliMeter);
    else
      Serial.println("TOF2 out of range");

    digitalWrite(MOTOR_PIN, HIGH);
    delay(200); =

    piezoValue = analogRead(PIEZO_PIN);
    Serial.print("Piezo value: ");
    Serial.println(piezoValue);

    digitalWrite(MOTOR_PIN, LOW);

    if (scale.is_ready()) {
      weight = scale.get_units(5);
      Serial.print("Weight (raw units): ");
      Serial.println(weight);
    } else {
      Serial.println("HX711 not ready");
    }

    Serial.println("-------------------------");
    delay(500); 
  }
}
