#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

const int emgPin = A0;
const int NUM_DEDOS = 5;

// Canal da PCA9685 de cada dedo
//                          Polegar Indicador Medio Anelar Minimo
const int canal[NUM_DEDOS] = {0,     1,       2,    3,     4};

// Pulso (microssegundos) de mão ABERTA e FECHADA para cada dedo
int abertoUs[NUM_DEDOS]  = {1000, 1000, 1000, 1000, 1000};
int fechadoUs[NUM_DEDOS] = {1800, 1800, 1800, 1800, 1800};

// Calculados automaticamente na calibração
float limiteFechar = 0;
float limiteAbrir  = 0;

float media = 0;
bool fechado = false;
unsigned long ultimaTroca = 0;
const unsigned long tempoMinimo = 400;   // ms mínimo em cada posição

// Move todos os dedos para aberto ou fechado
void moverMao(bool fechar) {
  for (int i = 0; i < NUM_DEDOS; i++) {
    pwm.writeMicroseconds(canal[i], fechar ? fechadoUs[i] : abertoUs[i]);
  }
}

// Lê o sensor durante 2 ciclos da rede (33 ms) e tira a média.
// Isso cancela boa parte do ruído de 60 Hz da tomada.
void atualizarMedia() {
  long soma = 0;
  int n = 0;
  unsigned long inicio = micros();
  while (micros() - inicio < 33333) {
    soma += analogRead(emgPin);
    n++;
  }
  float leitura = (float)soma / n;
  media = 0.8 * media + 0.2 * leitura;   // suavização extra
}

// Mede a média do sinal durante 'ms' milissegundos
float medirDurante(unsigned long ms) {
  unsigned long inicio = millis();
  float soma = 0;
  long n = 0;
  while (millis() - inicio < ms) {
    atualizarMedia();
    soma += media;
    n++;
  }
  return soma / n;
}

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  delay(500);
  moverMao(false);   // começa com a mão aberta

  // ---------- CALIBRAÇÃO ----------
  Serial.println();
  Serial.println("=== CALIBRACAO ===");
  Serial.println("RELAXE o braco (mao mole, apoiada na mesa)...");
  medirDurante(1500);                     // tempo para estabilizar
  float relaxado = medirDurante(3000);

  Serial.println("Agora FECHE a mao com FORCA e segure...");
  medirDurante(1000);
  float contraido = medirDurante(3000);

  Serial.println("Pode relaxar.");

  float diferenca = contraido - relaxado;
  limiteAbrir  = relaxado + diferenca * 0.35;
  limiteFechar = relaxado + diferenca * 0.65;

  Serial.print("Relaxado: ");      Serial.println(relaxado);
  Serial.print("Contraido: ");     Serial.println(contraido);
  Serial.print("Diferenca: ");     Serial.println(diferenca);
  Serial.print("Limite abrir: ");  Serial.println(limiteAbrir);
  Serial.print("Limite fechar: "); Serial.println(limiteFechar);

  if (diferenca < 50) {
    Serial.println("ATENCAO: pouca diferenca entre relaxado e contraido.");
    Serial.println("Confira eletrodos, ganho e ruido, e aperte RESET.");
  }

  Serial.println("=== FUNCIONANDO ===");
  delay(2000);
}

void loop() {
  atualizarMedia();   // já demora ~33 ms

  Serial.print("Media: ");
  Serial.print(media);
  Serial.print(" | Estado: ");
  Serial.println(fechado ? "FECHADA" : "ABERTA");

  unsigned long agora = millis();
  if (agora - ultimaTroca > tempoMinimo) {
    if (!fechado && media > limiteFechar) {
      Serial.println(">>> FECHAR MAO");
      moverMao(true);
      fechado = true;
      ultimaTroca = agora;
    }
    else if (fechado && media < limiteAbrir) {
      Serial.println(">>> ABRIR MAO");
      moverMao(false);
      fechado = false;
      ultimaTroca = agora;
    }
  }

  delay(1);
}