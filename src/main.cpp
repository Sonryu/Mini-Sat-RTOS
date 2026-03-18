/* 
Código-base para o Projeto 2 - Mini Satélite em Tempo Real.
Usem ele para transformar de um código sequencial (super loop) para uma arquitetura multitarefas 
utilizando FreeRTOS no ESP32.
*/
#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

// Criação dos objetos do BMP e MPU
Adafruit_BMP085 bmp;
Adafruit_MPU6050 mpu;

void setup(void) {
  // Inicialização do Monitor Serial
  Serial.begin(115200);

  // Inicialização do MPU
  while (!mpu.begin()) {
    Serial.println("MPU6050 não conectado!");
    delay(1000);
  }
  Serial.println("MPU6050 Pronto!");

  // Inicialização do BMP
  while (!bmp.begin()) {
    Serial.println("BMP180 não conectado!");
    delay(1000);
  }
  Serial.println("BMP180 Pronto!");
}

void loop() {
  /* Criação das variáveis para armazenar os dados do MPU
   Se quiserem verificar a documentação da biblioteca, vocês podem conferir aqui:
   [link da biblioteca] */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Criação das variáveis para armazenar os dados do BMP
   Se quiserem verificar a documentação da biblioteca, vocês podem conferir aqui:
   [link da biblioteca] */
  float temperature = bmp.readTemperature();
  int32_t pressure = bmp.readPressure();
  float altitude = bmp.readAltitude();

  /* struct dessa variável de MPUT: Objeto (a) -> Categoria (.acceleration) -> Eixo (.x)
    Caso fosse para dado do Giroscópio em Y seria: `Serial.println(g.gyro.y);` */
  Serial.print("Aceleração X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" g");

  Serial.println("");

  // O acesso da struct do BMP é bem mais simples:
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Pressão: ");
  Serial.print(pressure);
  Serial.println(" Pa");

  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");

  Serial.println();

  delay(1000);
}
