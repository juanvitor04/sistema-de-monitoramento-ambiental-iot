#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <WiFi.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

// 📡 Configuração do LoRa
#define RF_FREQUENCY 915000000 // Frequência LoRa (Brasil: 915MHz)
#define LORA_BANDWIDTH 0       // 125 kHz
#define LORA_SPREADING_FACTOR 7 // SF7
#define LORA_CODINGRATE 1       // Taxa de codificação 4/5
#define LORA_PREAMBLE_LENGTH 8  // Comprimento do preâmbulo
#define LORA_SYMBOL_TIMEOUT 0   // Tempo limite de símbolo
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false

#define BUFFER_SIZE 128 // Tamanho do buffer LoRa

char rxpacket[BUFFER_SIZE];
bool receivedFlag = false;

// 🔗 Configuração do Wi-Fi
#define WIFI_SSID "SSID_WIFI"
#define WIFI_PASSWORD "PASSWORD_WIFI"

// ⏰ Fuso horário
#define TZ_INFO "UTC-3"

// 🔍 Configuração do InfluxDB
#define INFLUXDB_URL "http://127.0.0.1:8086"
#define INFLUXDB_TOKEN "INFLUXDB_TOKEN"
#define INFLUXDB_ORG "INFLUXDB_ORG"
#define INFLUXDB_BUCKET "INFLUXDB_BUCKET"

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
Point sensorData("lora_sensor");

static RadioEvents_t RadioEvents;

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
    memcpy(rxpacket, payload, size);
    rxpacket[size] = '\0';
    receivedFlag = true;
    Serial.printf("\n📡 Pacote recebido: \"%s\" | RSSI: %d | SNR: %d\n", rxpacket, rssi, snr);
    Radio.Rx(0);
}

void setup() {
    Serial.begin(115200);
    Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);

    // 🔗 Conectar ao Wi-Fi
    Serial.print("🔗 Conectando ao WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n✅ WiFi conectado!");

    // ⏰ Sincronizar horário
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

    // 🔍 Conectar ao InfluxDB
    if (client.validateConnection()) {
        Serial.print("✅ InfluxDB conectado: ");
        Serial.println(client.getServerUrl());
    } else {
        Serial.println("❌ Erro ao conectar ao InfluxDB: " + client.getLastErrorMessage());
    }

    // 📡 Configurar LoRa
    RadioEvents.RxDone = OnRxDone;
    Radio.Init(&RadioEvents);
    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                      LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                      LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                      0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

    // Iniciar recepção
    Serial.println("📡 Entrando no modo RX...");
    Radio.Rx(0);
}

void loop() {
    Radio.IrqProcess();

    if (receivedFlag) {
        receivedFlag = false;

        // 🔍 Interpretar mensagem recebida: "T:25.30;H:60.00;CO2:400.00;F:350.00"
        float t, h, co2, fuma;
        if (sscanf(rxpacket, "T:%f;H:%f;CO2:%f;F:%f", &t, &h, &co2, &fuma) == 4) {
            Serial.printf("📊 Temperatura: %.2f °C | Umidade: %.2f %% | CO2: %.2f PPM | Fumaça: %.2f\n", t, h, co2, fuma);

            // Enviar para o InfluxDB
            sensorData.clearFields();
            sensorData.addField("temperatura", t);
            sensorData.addField("umidade", h);
            sensorData.addField("co2", co2);
            sensorData.addField("fumaca", fuma);
            client.writePoint(sensorData);
            Serial.println("✅ Dados enviados ao InfluxDB!");
        } else {
            Serial.println("❌ Erro no formato da mensagem recebida!");
        }
    }
}
