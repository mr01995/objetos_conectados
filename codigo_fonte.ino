#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

// Configurações Wi-Fi e MQTT
const char* ssid = "NOME_REDE";
const char* password = "SENHA_REDE";
const char* mqtt_server = "test.mosquitto.org";

// Inicialização do cliente Wi-Fi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
float R0 = 10.0;  // Calibração do sensor de gás
const int MQ135_PIN = 34;  // Pino analógico do sensor MQ-135

// Variáveis para medir tempos
unsigned long sensorDetectionTime = 0;  // Momento em que o sensor detecta

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(MQ135_PIN, INPUT);
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  // Callback para mensagens recebidas (não usada aqui)
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client-Unique")) {  // Identificador único
      client.subscribe("Gases/Leituras");
    } else {
      delay(3000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 3000) {  // Envia a cada 3 segundos
    lastMsg = now;

    // Momento da detecção do sensor (usando micros para alta precisão)
    sensorDetectionTime = micros();

    int mq135Value = analogRead(MQ135_PIN);

    // Estimativas de concentração de cada gás
    float co_ppm = calculateGasPPM(mq135Value, 1.1);       // Monóxido de carbono
    float alcohol_ppm = calculateGasPPM(mq135Value, 0.7);  // Álcool
    float co2_ppm = calculateGasPPM(mq135Value, 1.2);      // Dióxido de carbono
    float toluene_ppm = calculateGasPPM(mq135Value, 0.8);  // Tolueno
    float nh4_ppm = calculateGasPPM(mq135Value, 0.9);      // Amônia
    float acetone_ppm = calculateGasPPM(mq135Value, 0.6);  // Acetona
    float smoke_ppm = calculateGasPPM(mq135Value, 1.3);    // Fumaça

    // Calcula o tempo entre a detecção e o envio (em microssegundos)
    unsigned long deteccaoTempoEnvio = micros() - sensorDetectionTime;

    // Publicar os dados de cada gás no broker MQTT
    publishGasData("gases/alcohol", alcohol_ppm, deteccaoTempoEnvio);
    publishGasData("gases/acetone", acetone_ppm, deteccaoTempoEnvio);
    publishGasData("gases/co", co_ppm, deteccaoTempoEnvio);
    publishGasData("gases/co2", co2_ppm, deteccaoTempoEnvio);
    publishGasData("gases/toluene", toluene_ppm, deteccaoTempoEnvio);
    publishGasData("gases/nh4", nh4_ppm, deteccaoTempoEnvio);
    publishGasData("gases/smoke", smoke_ppm, deteccaoTempoEnvio);
  }
}

float calculateGasPPM(int value, float factor) {
  // Converte o valor do sensor em ppm
  return (value / 4095.0) * R0 * factor;
}

void publishGasData(const char* topic, float value, unsigned long timeToSend) {
  char msg[150];
  char valueStr[20];
  dtostrf(value, 1, 2, valueStr);  // Converte o valor float para string

  // Formata a mensagem com o tempo em microssegundos
  snprintf(msg, sizeof(msg), "{\"valor\": %s, \"tempo_deteccao_envio\": %lu, \"unidade\": \"microssegundos\"}", 
           valueStr, timeToSend);

  client.publish(topic, msg);  // Publica a mensagem no tópico MQTT
}
