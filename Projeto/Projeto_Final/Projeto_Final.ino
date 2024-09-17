#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// Sensor
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

// MQTT
#define TOPICO_PUBLISH "sensorTemp_01"
#define ID_MQTT "Embarcados_esp32"

const char* mqtt_user = "Embarcados_esp32";
const char* mqtt_password = "Embarcados2024";

const char* SSID = "Embarcados_esp32";
const char* PASSWORD = "embarcados2024";

const char* BROKER_MQTT = "3a58006c3a3140f7a7c1b51027c53e9f.s1.eu.hivemq.cloud";
int BROKER_PORT = 8883;

WiFiClientSecure espClient;
PubSubClient MQTT(espClient);

// Sensor
#define ONE_WIRE_BUS1 15
OneWire oneWire1(ONE_WIRE_BUS1);
DallasTemperature sensor1(&oneWire1);

#define ONE_WIRE_BUS2 13
OneWire oneWire2(ONE_WIRE_BUS2);
DallasTemperature sensor2(&oneWire2);

// float RawHigh = 1;
// float ReferenceHigh = 0;
// float RawLow = -7.56;
// float ReferenceLow = -12;
float RawHigh = 29;
float ReferenceHigh = 26;
float RawLow = 1;
float ReferenceLow = 0;

float RawRange = RawHigh - RawLow;
float ReferenceRange = ReferenceHigh - ReferenceLow;

// MQTT
void init_serial(void);
void init_wifi(void);
void init_mqtt(void);
void reconnect_wifi(void);
void reconnect_mqtt(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void verifica_conexoes_wifi_mqtt(void);

void setup() {
  // MQTT
  init_serial();
  init_wifi();
  init_mqtt();

  // Sensor
  Serial.begin(115200);
  sensor1.begin();
  sensor2.begin();
}

void init_serial() {
  Serial.begin(115200);
}

void init_wifi(void) {
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");
  reconnect_wifi();
}

void init_mqtt(void) {
  espClient.setInsecure();  // Ignora a verificação do certificado
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
}

void reconnect_mqtt(void) {
  while (!MQTT.connected()) {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT, mqtt_user, mqtt_password)) {
      Serial.println("Conectado com sucesso ao broker MQTT!");
    } else {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Nova tentativa de conexao em 2s");
      delay(2000);
    }
  }
}

void reconnect_wifi() {
  if (WiFi.status() == WL_CONNECTED)
    return;

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("IP obtido: ");
  Serial.println(WiFi.localIP());
}

void verifica_conexoes_wifi_mqtt(void) {
  reconnect_wifi();
  if (!MQTT.connected())
    reconnect_mqtt();
}

void loop() {
  // Sensor
  sensor1.requestTemperatures();
  sensor2.requestTemperatures();

  float T1 = sensor1.getTempCByIndex(0);
  float T2 = sensor2.getTempCByIndex(0);
  float CorrectedValue1 = (((T1 - RawLow) * ReferenceRange) / RawRange) + ReferenceLow;
  float CorrectedValue2 = (((T2 - RawLow) * ReferenceRange) / RawRange) + ReferenceLow;
  

  // Crie um objeto JSON
  StaticJsonDocument<200> jsonDocument;
  JsonObject s1 = jsonDocument.createNestedObject("sensor1");
  s1["temperatura"] = String(CorrectedValue1);
  JsonObject s2= jsonDocument.createNestedObject("sensor2");
  s2["temperatura"] = String(CorrectedValue2);
  // jsonDocument["nome"] = "Sensor 01";
  // jsonDocument["status"] = "Ativo";
  // jsonDocument["temperatura"] = String(T1);

  // Converta o JSON para uma string
  char jsonBuffer[512];
  serializeJson(jsonDocument, jsonBuffer);

  // MQTT
  verifica_conexoes_wifi_mqtt();
  // MQTT.publish(TOPICO_PUBLISH, "ESP32 se comunicando com MQTT");
  MQTT.publish(TOPICO_PUBLISH, jsonBuffer);


  Serial.print("Sensor 1(*C): ");
  Serial.print(T1);
  Serial.print(", C: ");
  Serial.print(CorrectedValue1);

  Serial.print(" | Sensor 2(*C): ");
  Serial.print(T2);
  Serial.print(", C: ");
  Serial.println(CorrectedValue2);

  MQTT.loop();


  delay(1000);
}