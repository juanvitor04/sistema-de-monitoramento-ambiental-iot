# 🌡️ Estação de Monitoramento Ambiental com ESP32, LoRa e Grafana

Este projeto tem como objetivo criar uma **estação de monitoramento ambiental inteligente** utilizando o microcontrolador **ESP32**, sensores de qualidade do ar (**MQ-135**) e de temperatura/umidade (**DHT11**), comunicação via **LoRa** e integração com banco de dados **InfluxDB** e o painel de visualização **Grafana**.

## 📦 Funcionalidades

- 🧠 Leitura de sensores:
  - **Temperatura e Umidade** com DHT11
  - **Qualidade do ar (CO₂ e fumaça)** com MQ135

- 📡 Transmissão de dados:
  - Envio via **LoRa** (para longas distâncias, sem necessidade de Wi-Fi)
  - Envio via **Wi-Fi** diretamente para um servidor InfluxDB

- ☁️ Armazenamento:
  - Dados registrados em um banco de dados **InfluxDB**

- 📈 Visualização:
  - Dashboards interativos em **Grafana**, com gráficos em tempo real

- 💤 Economia de energia:
  - O ESP32 entra em modo **Deep Sleep** entre cada leitura para otimizar o consumo energético

## 🛠️ Tecnologias Utilizadas

| Componente          | Função                                   |
|---------------------|-------------------------------------------|
| **ESP32**           | Microcontrolador principal                |
| **DHT11**           | Sensor de temperatura e umidade           |
| **MQ135**           | Sensor de qualidade do ar (CO₂, fumaça)   |
| **LoRa SX1276**     | Comunicação sem fio de longo alcance      |
| **InfluxDB**        | Banco de dados de séries temporais        |
| **Grafana**         | Visualização e painéis de dados           |
| **Docker Compose**  | Facilita o deploy dos containers          |

## 🚀 Como usar

### Pré-requisitos

-ESP32 DevKit (pelo menos 2 unidades) caso use LoRa, com somente wi-fi é necessario um ESP32 com Suporte a wi-fi
- Arduino IDE configurado com suporte ao ESP32
- Docker instalado na máquina (para InfluxDB e Grafana)
- Sensores conectados aos pinos indicados nos códigos

