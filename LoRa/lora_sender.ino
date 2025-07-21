#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "DHT.h"

#define RF_FREQUENCY 915000000 // Frequência do LoRa (Brasil: 915MHz)
#define TX_OUTPUT_POWER 5      // Potência de transmissão
#define LORA_BANDWIDTH 0       // 125 kHz
#define LORA_SPREADING_FACTOR 7 // SF7
#define LORA_CODINGRATE 1       // Taxa de codificação 4/5
#define LORA_PREAMBLE_LENGTH 8  // Comprimento do preâmbulo
#define LORA_SYMBOL_TIMEOUT 0   // Tempo limite de símbolo
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false

#define BUFFER_SIZE 64 // Tamanho do buffer LoRa

// Configuração dos sensores
#define DHTPIN 4       // Pino do DHT11
#define DHTTYPE DHT11  // Tipo do sensor DHT
#define MQ135PIN 3    // Pino do MQ135

DHT dht(DHTPIN, DHTTYPE);

char txpacket[BUFFER_SIZE];
double txNumber;
bool lora_idle = true;

static RadioEvents_t RadioEvents;
void OnTxDone(void);
void OnTxTimeout(void);

void setup() {
    Serial.begin(115200);
    Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
    dht.begin();
    txNumber = 0;

    // Configura LoRa
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    Radio.Init(&RadioEvents);
    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                      LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                      LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                      true, 0, 0, LORA_IQ_INVERSION_ON, 3000);
}

void loop() {
    if (lora_idle) {
        delay(5000); // Envia a cada 5 segundos

        // Lendo sensores
        float temperatura = dht.readTemperature();
        float umidade = dht.readHumidity();
        float co2 = analogRead(MQ135PIN) * (5.0 / 4095.0) * 1000; // Simulação de PPM de CO2
        float fumaca = co2 / 2; // Simulação de fumaça

        if (isnan(temperatura) || isnan(umidade)) {
            Serial.println("❌ Erro ao ler o DHT11!");
            return;
        }

        txNumber += 0.01;
        sprintf(txpacket, "T:%.2f;H:%.2f;CO2:%.2f;F:%.2f", temperatura, umidade, co2, fumaca);

        Serial.printf("\n📡 Enviando pacote: \"%s\"\n", txpacket);
        Radio.Send((uint8_t *)txpacket, strlen(txpacket));
        lora_idle = false;
    }
    Radio.IrqProcess();
}

void OnTxDone(void) {
    Serial.println("✅ TX done...");
    lora_idle = true;
}

void OnTxTimeout(void) {
    Serial.println("❌ TX Timeout...");
    lora_idle = true;
}
