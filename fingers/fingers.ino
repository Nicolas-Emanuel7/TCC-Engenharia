
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pca9685(0x40);

// Canais dos servos no PCA9685
const int THUMB  = 0;  // Polegar
const int INDEX  = 1;  // Indicador
const int MIDDLE = 2;  // Médio
const int RING   = 3;  // Anelar
const int PINKY  = 4;  // Mínimo
const int WRIST  = 8;  // Pulso


const int SERVOMIN = 250;
const int SERVOMAX = 410;


const int ABERTO  = 0;
const int FECHADO = 90;

const int TEMPO_MOVIMENTO = 1000;
const int TEMPO_PAUSA = 1000;

void servoWrite(int canal, int angulo)
{
  angulo = constrain(angulo, 0, 180);

  int pulse = map(angulo, 0, 180,
                  SERVOMIN, SERVOMAX);

  pca9685.setPWM(canal, 0, pulse);
}

void testarDedo(int canal, const char* nome)
{
  Serial.print("Movimentando: ");
  Serial.println(nome);

  // Fecha somente o dedo selecionado
  servoWrite(canal, FECHADO);
  delay(TEMPO_MOVIMENTO);

  // Mantém a posição por um instante
  delay(TEMPO_PAUSA);

  // Abre somente o dedo selecionado
  servoWrite(canal, ABERTO);
  delay(TEMPO_MOVIMENTO);

  delay(TEMPO_PAUSA);

  Serial.print("Concluido: ");
  Serial.println(nome);
  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  pca9685.begin();
  pca9685.setPWMFreq(50);

  delay(1000);

  Serial.println("Teste individual dos dedos - InMoov");

  // Coloca os dedos na posição inicial
  servoWrite(THUMB, ABERTO);
  servoWrite(INDEX, ABERTO);
  servoWrite(MIDDLE, ABERTO);
  servoWrite(RING, ABERTO);
  servoWrite(PINKY, ABERTO);

  // Mantem o pulso sem movimentacao
  // Nao envia comando ao canal 8.

  delay(2000);
}

void loop()
{
  testarDedo(THUMB,  "Polegar - CH0");
  testarDedo(INDEX,  "Indicador - CH1");
  testarDedo(MIDDLE, "Medio - CH2");
  testarDedo(RING,   "Anelar - CH3");
  testarDedo(PINKY,  "Minimo - CH4");

  Serial.println("Ciclo completo. Reiniciando...");
  delay(3000);
}
