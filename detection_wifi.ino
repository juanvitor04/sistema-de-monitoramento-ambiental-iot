#if defined(ESP32)
  #include <WiFiMulti.h>
  #include <esp_sleep.h>
  WiFiMulti wifiMulti;
  #define DEVICE "ESP32"
#elif defined(ESP8266)
  #include <ESP8266WiFiMulti.h>
  ESP8266WiFiMulti wifiMulti;
  #define DEVICE "ESP8266"
#endif

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <DHT.h>
#include <MQ135.h>

// WiFi AP SSID
#define WIFI_SSID "SSID_WIFI"
// WiFi password
#define WIFI_PASSWORD "PASSWORD_WIFI"

#define INFLUXDB_URL "http://127.0.0.1:8086"
#define INFLUXDB_TOKEN "INFLUXDB_TOKEN"
#define INFLUXDB_ORG "INFLUXDB_ORG"
#define INFLUXDB_BUCKET "INFLUXDB_BUCKET"

// Time zone info
#define TZ_INFO "UTC-3"

// Configuração do sensor DHT11
#define DHTPIN 4      // Pino do sensor
#define DHTTYPE DHT11 // Tipo do sensor
DHT dht(DHTPIN, DHTTYPE);

// Configuração do sensor MQ-135
#define MQ135_PIN 34 // Pino analógico do sensor MQ-135
MQ135 mq135_sensor(MQ135_PIN);

// Declaração do cliente InfluxDB
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Declaração dos pontos de dados
Point sensorDHT("dht11");
Point sensorMQ135("mq135");

// Tempo de deep sleep (150 segundos = 2.5 minutos)
#define SLEEP_TIME 150

void setup() {
    Serial.begin(115200);
    dht.begin();

    // Configuração do WiFi
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Conectando-se ao Wi-Fi");
    while (wifiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println(" Conectado!");

    // Sincronização de horário
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

    // Verificar conexão com o InfluxDB
    if (client.validateConnection()) {
        Serial.print("Conectado ao InfluxDB: ");
        Serial.println(client.getServerUrl());
    } else {
        Serial.print("Falha na conexão com InfluxDB: ");
        Serial.println(client.getLastErrorMessage());
    }

    // Leitura do sensor DHT11
    float umidade = dht.readHumidity();
    float temperatura = dht.readTemperature();
    
    if (!isnan(umidade) && !isnan(temperatura)) {
        Serial.print("Umidade: ");
        Serial.print(umidade);
        Serial.print("%  Temperatura: ");
        Serial.print(temperatura);
        Serial.println("C");

        // Registrar os valores no InfluxDB
        sensorDHT.clearFields();
        sensorDHT.addField("umidade", umidade);
        sensorDHT.addField("temperatura", temperatura);
        client.writePoint(sensorDHT);
    } else {
        Serial.println("Erro ao ler o sensor DHT11!");
    }

    // Leitura do sensor MQ-135 com correção de temperatura e umidade
    float co2 = mq135_sensor.getCorrectedPPM(temperatura, umidade);
    float fumaca = mq135_sensor.getCorrectedPPM(temperatura, umidade);

    Serial.print("CO2 (PPM): ");
    Serial.println(co2);
    Serial.print("Fumaça (PPM): ");
    Serial.println(fumaca);
    
    // Registrar os valores no InfluxDB
    sensorMQ135.clearFields();
    sensorMQ135.addField("co2", co2);
    sensorMQ135.addField("fumaca", fumaca);
    client.writePoint(sensorMQ135);

    Serial.println("Entrando em modo Deep Sleep...");
    esp_sleep_enable_timer_wakeup(SLEEP_TIME * 1000000ULL); // Configura o deep sleep
    esp_deep_sleep_start();
}

void loop() {
    // Não é necessário código no loop, pois o ESP32 será reiniciado após o deep sleep
}
