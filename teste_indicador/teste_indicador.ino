
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pca9685(0x40);

// Indicador conectado ao canal 2
const int INDICADOR = 15;

// Limites PWM
const int SERVOMIN = 250;
const int SERVOMAX = 410;

// Ângulos de teste
const int ABERTO = 0;
const int FECHADO = 270;

void servoWrite(int canal, int angulo)
{
  int pulse = map(angulo, 0, 270,
                  SERVOMIN, SERVOMAX);

  pca9685.setPWM(canal, 0, pulse);
}

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  pca9685.begin();
  pca9685.setPWMFreq(50);

  delay(1000);

  Serial.println("Teste do indicador - Canal CH3");

  // Posiciona somente o indicador
  servoWrite(INDICADOR, ABERTO);

  delay(2000);
}

void loop()
{
  // Serial.println("Fechando indicador");
  // servoWrite(INDICADOR, FECHADO);
  // delay(2000);

  Serial.println("Abrindo indicador");
  servoWrite(INDICADOR, ABERTO);
  delay(2000);
}
