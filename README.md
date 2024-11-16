# objetos_conectados
Sistema de Monitoramento de Emissões para Áreas Industriais

Descrição do Funcionamento e Uso
O Sistema de Monitoramento de Emissões utiliza um ESP32 conectado a um sensor de gás MQ-135 para monitorar a qualidade do ar em tempo real. Os dados coletados são processados e enviados para um broker MQTT por meio de conectividade Wi-Fi, permitindo o monitoramento remoto. Este projeto é voltado para indústrias e municípios que desejam monitorar poluentes atmosféricos de forma eficiente.

Software Desenvolvido:
Código-fonte do ESP32 implementado em Arduino IDE.
Configuração para conexão Wi-Fi e envio de dados via protocolo MQTT.
Script para visualização dos dados coletados em tempo real.
Descrição do Hardware Utilizado:

ESP32: Microcontrolador responsável pela leitura do sensor, processamento dos dados e envio ao servidor MQTT.
MQ-135: Sensor de gás utilizado para detectar poluentes como amônia, benzeno e outros gases nocivos.

Protocolo MQTT: Comunicação eficiente e leve, ideal para IoT. A implementação utiliza tópicos para enviar os dados do sensor.
Interfaces: Configuração para brokers MQTT como Mosquitto ou serviços baseados em nuvem (ex.: HiveMQ).

Configuração de rede Wi-Fi no código:
const char* ssid = "Sua_Rede_WiFi";
const char* password = "Senha_WiFi";

Configuração do broker MQTT:
const char* mqtt_server = "endereco_do_broker";
const int mqtt_port = 1883;
