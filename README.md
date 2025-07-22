# 🌡️ Estação de Monitoramento Ambiental com ESP32, LoRa e Grafana

Este projeto tem como objetivo criar uma **estação de monitoramento ambiental inteligente** utilizando o microcontrolador **ESP32**, sensores de qualidade do ar (**MQ-135**) e de temperatura/umidade (**DHT11**), comunicação via **LoRa** e integração com banco de dados **InfluxDB** e o painel de visualização **Grafana**.

---

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

---

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

---

## 🚀 Como Usar

### ✅ Pré-requisitos

- ESP32 DevKit (pelo menos 2 unidades para uso com LoRa; apenas 1 se usar Wi-Fi)
- Arduino IDE configurado com suporte ao ESP32
- Docker instalado na máquina (para InfluxDB e Grafana)
- Sensores conectados aos pinos indicados nos códigos

---

###  🐋  Instalação do Docker e Docker Compose (Ubuntu)
```bash
sudo apt update
sudo apt install -y docker.io docker-compose-plugin
```

As configurações abaixo foram feitas no arquivo "docker-compose.yml" então baixe o arquivo que está disponivel e o edite.

As variáveis de ambiente podem ser mudadas no arquivo docker-compose.yml lembre-se que essas informações vão ser colocados no arquivo "detection_wifi.ino" ou no "lora_receiver.ino"

Para realizar a conexão com o banco de dados.
```ỳml

INFLUXDB_USER=admin

INFLUXDB_PASSWORD=senha123

INFLUXDB_ORG=my-org

INFLUXDB_BUCKET=my-bucket

INFLUXDB_TOKEN=meu-token-secreto
```
## Grafana usuário e senha
No arquivo docker-compose.yml edite as variveis de ambiente do grafana
```yml
GF_SECURITY_ADMIN_USER=GRAFANA_USER
GF_SECURITY_ADMIN_PASSWORD=GRAFANA_PASSWORD
```
## Inicie os contairners
```bash
docker compose up -d
```
## 🛠️ Configuração do Sistema

As configurações devem ser feitas no arquivo lora_receiver.ino (para comunicação via LoRa).

Caso opte por Wi-Fi, utilize o arquivo detection_wifi.ino.

## ⚠️ Importante
O dispositivo receptor (gateway) precisa ter suporte a Wi-Fi, pois ele funciona como uma ponte entre LoRa e Wi-Fi.

Ele é responsável por:

📡 Receber dados via LoRa de sensores remotos.

🌐 Conectar-se a uma rede Wi-Fi local.

📤 Enviar os dados recebidos para o InfluxDB via HTTP.

Você deve alterar as variáveis de ambiente no seu código para refletirem os dados reais da sua instância do InfluxDB. Essas informações são geradas quando você faz a configuração inicial do banco.

## Configuração do Wi-Fi (Obrigatório para envio ao InfluxDB)
🔁 Substitua pelos dados reais da sua rede Wi-Fi.
```C++
#define WIFI_SSID "SSID_WIFI"
#define WIFI_PASSWORD "PASSWORD_WIFI"
```
Exemplo
```Exemplo
#define WIFI_SSID "MinhaRedeWiFi"
#define WIFI_PASSWORD "minhaSenha123"
```
Configuração do influxDB
⚠️ O ESP32 precisa estar conectado na mesma rede local do computador ou servidor que está rodando o InfluxDB.
🔁 Substitua 127.0.0.1 pelo IP real da máquina onde o InfluxDB está rodando.
```C++
#define INFLUXDB\_URL "http://127.0.0.1:8086"
```
Exemplo
```Exemplo
#define INFLUXDB\_URL "http://192.168.1.100:8086"
```
## 🔐 Token de Acesso
```C++
#define INFLUXDB_TOKEN "SEU_TOKEN"
```
🔁 Gere um token no InfluxDB em

Load Data > API Tokens > Generate Token

Cole no código
```C++
#define INFLUXDB\_TOKEN "meu_token_gerado"
```
🏢 3. Organização (Org)

#define INFLUXDB\_ORG "SUA\_ORG"

🔁 Substitua pelo nome da organização criada no setup inicial do InfluxDB:

#define INFLUXDB\_ORG "minha\_empresa"

🪣 4. Bucket

#define INFLUXDB\_BUCKET "SEU\_BUCKET"

🔁 Substitua pelo nome do bucket onde deseja armazenar os dados:

#define INFLUXDB\_BUCKET "dados\_lora"

🔗 Integração do Grafana com o InfluxDB

Passos básicos para integrar:

1. Configurar o InfluxDB no Grafana

No painel do Grafana, vá em Configuration > Data Sources.

Clique em Add data source e escolha InfluxDB.

Preencha as configurações:

URL: http://<IP\_do\_InfluxDB>:8086

Auth: marque para usar autenticação e insira o Token do InfluxDB.

Organization: nome da organização configurada no InfluxDB.

Default Bucket: o bucket onde os dados estão sendo armazenados.

1. Testar conexão

Clique em Save & Test para garantir que o Grafana consiga se conectar ao InfluxDB.

1. Criar dashboards

Use a interface do Grafana para criar gráficos e painéis com as métricas do seu bucket.

Utilize a linguagem de consulta Flux (linguagem padrão do InfluxDB 2.x) para montar consultas avançadas.
