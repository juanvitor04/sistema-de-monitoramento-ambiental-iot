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

## 🐋 Docker Compose – Instalação e Configuração

### 📦 Instalação do Docker e Docker Compose (Ubuntu)
```bash
sudo apt update
sudo apt install -y docker.io docker-compose-plugin
```

As configurações abaixo foram feitas no arquivo "docker-composer.yml" então baixe o arquivo que está disponivel e o edite.

As variáveis de ambiente podem ser mudadas no arquivo docker-composer.yml lembre-se que esses dados vão ser colocados no arquivo "detection\_wifi.ino" ou no "lora\_receiver.ino"

Para realizar a conexão com o banco de dados.

INFLUXDB\_USER=admin

INFLUXDB\_PASSWORD=senha123

INFLUXDB\_ORG=my-org

INFLUXDB\_BUCKET=my-bucket

INFLUXDB\_TOKEN=meu-token-secreto

GRAFANA\_USER=admin

GRAFANA\_PASSWORD=grafana123

após isso suba os contairners

docker compose up -d

🛠️ Configurações Necessárias para o Sistema Funcionar(Observação essas alterações deve ser feita  no arquivo nomeado "Lora\_receiver.ino"

se for usar comunicação LoRA se for wifi utilize o arquivo "detection\_wifi.ino").

⚠️ Observação Importante sobre o Dispositivo Receptor (Gateway)

O dispositivo receptor (este código) deve obrigatoriamente ter suporte a Wi-Fi, pois ele atua como um gateway LoRa-WiFi.

Ele é responsável por:

📡 Receber dados via LoRa de sensores remotos.

🌐 Conectar-se a uma rede Wi-Fi local.

📤 Enviar os dados recebidos para o InfluxDB via HTTP.

Você deve alterar as variáveis de ambiente no seu código para refletirem os dados reais da sua instância do InfluxDB. Essas informações são geradas quando você faz a configuração inicial do banco.

🌐 0. Wi-Fi (Obrigatório para envio ao InfluxDB)

#define WIFI\_SSID "SSD\_WIFI"

#define WIFI\_PASSWORD "PASSWORD\_WIFI"

🔁 Substitua pelos dados reais da sua rede Wi-Fi:

#define WIFI\_SSID "MinhaRedeWiFi"

#define WIFI\_PASSWORD "minhaSenha123"

⚠️ O ESP32 precisa estar conectado na mesma rede local do computador ou servidor que está rodando o InfluxDB.

📍1. URL do InfluxDB

#define INFLUXDB\_URL "http://127.0.0.1:8086"

🔁 Substitua 127.0.0.1 pelo IP real da máquina onde o InfluxDB está rodando.

Exemplo:

#define INFLUXDB\_URL "http://192.168.1.100:8086"

🔐 2. Token de Acesso

#define INFLUXDB\_TOKEN "SEU\_TOKEN"

🔁 Gere um token no InfluxDB em:

Load Data > API Tokens > Generate Token

Cole no código:

#define INFLUXDB\_TOKEN "meu\_token\_gerado"

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
