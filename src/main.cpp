/*
 * Copyright (C) 2026 Ramon Watson de Lima Vilar
 * * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 */

// ||--------||--------||--------||--------||--------||--------||--------||--------||

/* 
Código-base para o Projeto 2 - Mini Satélite em Tempo Real.
Usem ele para transformar de um código sequencial (super loop) para uma arquitetura multitarefas 
utilizando FreeRTOS no ESP32.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

// Criação dos objetos do BMP e MPU

// Estrutura principal que armazena todos os pacotes de dados coletados pelos sensores do satélite
typedef struct { 
  float accX, accY, accZ;     // Dados de aceleração capturados pelo MPU6050 nos eixos X, Y e Z
  float gyroX, gyroY, gyroZ;  // Dados de rotação (giroscópio) capturados pelo MPU6050
  float temp;                 // Temperatura ambiente lida pelo sensor BMP
  float pressure;             // Pressão atmosférica medida em Pascal pelo BMP
  float altitude;             // Altitude calculada com base na pressão atmosférica

} SatelliteData_t ;

// Handle da Fila (Queue): Canal de comunicação sincronizado entre a tarefa de aquisição e a de telemetria
QueueHandle_t sensor_queue;

// Objeto da biblioteca Adafruit para controle e leitura do sensor BMP180
Adafruit_BMP085 bmp;

// Objeto da biblioteca Adafruit para controle e leitura do sensor MPU6050
Adafruit_MPU6050 mpu;

// Protótipo da Task de Aquisição: Responsável por ler os sensores em alta frequência
void task_acquisition(void *pvParameters);

// Protótipo da Task de Telemetria: Responsável por processar e enviar os dados para o monitor serial
void task_telemetry(void *pvParameters);

void setup(void) {
  // Inicialização do Monitor Serial
  Serial.begin(115200);

  if (!mpu.begin()) { 
    while(1) {
      Serial.println("Erro MPU6050");
      delay(1000);
    }
  
  }

  if (!bmp.begin()) { 
    while(1) {
      Serial.println("Erro BMP180");
      delay(1000);
    }
  
  }

  sensor_queue = xQueueCreate(10, sizeof(SatelliteData_t));

}

void loop() {
/*


  // Criação das variáveis para armazenar os dados do MPU
  // Se quiserem verificar a documentação da biblioteca, vocês podem conferir aqui:
  // [link da biblioteca] 
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Criação das variáveis para armazenar os dados do BMP
  // Se quiserem verificar a documentação da biblioteca, vocês podem conferir aqui:
  // [link da biblioteca] 
  float temperature = bmp.readTemperature();
  int32_t pressure = bmp.readPressure();
  float altitude = bmp.readAltitude();

  // struct dessa variável de MPUT: Objeto (a) -> Categoria (.acceleration) -> Eixo (.x)
  //  Caso fosse para dado do Giroscópio em Y seria: `Serial.println(g.gyro.y);` 
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
*/

vTaskDelete(NULL);
}


  void taskAcquisition(void *pvParameters) {
    SatelliteData_t currentData;
    sensors_event_t a, g, t;
    
    // Variáveis para controle de tempo determinístico (100Hz)
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // 10ms = 100Hz

    for (;;) {
        // Leitura dos sensores
        mpu.getEvent(&a, &g, &t);
        
        currentData.accX = a.acceleration.x;
        currentData.accY = a.acceleration.y;
        currentData.accZ = a.acceleration.z;
        currentData.gyroX = g.gyro.x;
        currentData.gyroY = g.gyro.y;
        currentData.gyroZ = g.gyro.z;
        currentData.temp = bmp.readTemperature();
        currentData.pressure = bmp.readPressure();
        currentData.altitude = bmp.readAltitude();

        // Envia para a fila (sobrescreve se estiver cheia para manter dado real-time)
        xQueueSend(sensor_queue, &currentData, 0);

        
       vTaskDelayUntil(&xLastWakeTime, xFrequency);     

    }


void task_telemetry( void *pvParameters) {
  // ainda a avaliar
  
  SatelliteData_t receivedData;

    for (;;) {
        // Tenta tirar o dado da fila (espera até haver um disponível)
        if (xQueueReceive(sensorQueue, &receivedData, portMAX_DELAY)) {
            Serial.println(">>> TELEMETRIA CUBESAT <<<");
            Serial.printf("Acel: [%.2f, %.2f, %.2f] m/s^2\n", 
                          receivedData.accX, receivedData.accY, receivedData.accZ);
            Serial.printf("Giro: [%.2f, %.2f, %.2f] rad/s\n", 
                          receivedData.gyroX, receivedData.gyroY, receivedData.gyroZ);
            Serial.printf("Ambiente: %.1f C | %d Pa | Alt: %.2f m\n", 
                          receivedData.temp, (int)receivedData.pressure, receivedData.altitude);
            Serial.println("---------------------------\n");
        }

        // Taxa de exibição de 10Hz&&langs_count=8langs_count=8 (100ms)
        vTaskDelay(pdMS_TO_TICKS(100));
    }

}