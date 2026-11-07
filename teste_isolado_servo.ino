#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pca9685(0x40);

const int SERVO_PULSO = 5;

const int SERVOMIN = 250;
const int SERVOMAX = 410;


void moverPulso(int angulo) {

  int pulse = map(angulo, 0, 180, SERVOMIN, SERVOMAX);

  pca9685.setPWM(SERVO_PULSO, 0, pulse);

  Serial.print("MG995 - Pulso | Angulo: ");
  Serial.print(angulo);
  Serial.print(" graus | PWM: ");
  Serial.println(pulse);
}

void setup() {

  Serial.begin(115200);

  pca9685.begin();
  pca9685.setPWMFreq(50);

  delay(1000);

  moverPulso(90);

  delay(2000);
}

void loop() {

  Serial.println("Movendo 90 -> 150 graus");

  for (int angulo = 90; angulo <= 150; angulo += 5) {
    moverPulso(angulo);
    delay(300);
  }

  delay(1000);

  Serial.println("Movendo 150 -> 90 graus");

  for (int angulo = 150; angulo >= 90; angulo -= 5) {
    moverPulso(angulo);
    delay(300);
  }

  delay(1000);
}