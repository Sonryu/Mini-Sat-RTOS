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

typedef struct { 
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;
  float temp;
  float pressure;
  float altitude;

} SatelliteData_t ;

QueueHandle_t sensor_queue;
Adafruit_BMP085 bmp;
Adafruit_MPU6050 mpu;

void task_acquisition(void *pvParameters);
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


void task_acquisition( void *pvParameters) {
  // ainda a avaliar
  continue;
}

void task_telemetry( void *pvParameters) {
  // ainda a avaliar
  continue;
}