#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

// 1. Estrutura de Dados (O Pacote do Satélite)
typedef struct {
    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;
    float temp;
    float pressure;
    float altitude;
} SatelliteData_t;

// 2. Recursos do FreeRTOS
QueueHandle_t sensorQueue;
Adafruit_BMP085 bmp;
Adafruit_MPU6050 mpu;

// Protótipos das Tarefas
void taskAcquisition(void *pvParameters);
void taskTelemetry(void *pvParameters);

void setup() {
    Serial.begin(115200);

    // Inicialização dos Sensores
    if (!mpu.begin()) {
        while (1) { Serial.println("Erro MPU6050"); delay(1000); }
    }
    if (!bmp.begin()) {
        while (1) { Serial.println("Erro BMP180"); delay(1000); }
    }

    // 3. Criação da Fila (Capacidade para 10 pacotes)
    sensorQueue = xQueueCreate(10, sizeof(SatelliteData_t));

    if (sensorQueue != NULL) {
        // Tarefa de Aquisição: Alta Prioridade, Core 1 (100Hz)
        xTaskCreatePinnedToCore(
            taskAcquisition, "Acq", 4096, NULL, 3, NULL, 1
        );

        // Tarefa de Telemetria: Baixa Prioridade, Core 0 (10Hz)
        xTaskCreatePinnedToCore(
            taskTelemetry, "Telem", 4096, NULL, 1, NULL, 0
        );
    }
}

void loop() {
    // No FreeRTOS, o loop fica vazio ou deletado para economizar recursos
    vTaskDelete(NULL);
}

// --- Implementação das Tarefas ---

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
        xQueueSend(sensorQueue, &currentData, 0);

        // Espera exata para manter os 100Hz
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void taskTelemetry(void *pvParameters) {
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