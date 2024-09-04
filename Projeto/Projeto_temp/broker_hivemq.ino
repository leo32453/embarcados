#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#define TOPICO_PUBLISH   "sensorTemp_01"  
#define ID_MQTT  "Embarcados_esp32"     

const char* mqtt_user = "Embarcados_esp32";
const char* mqtt_password = "Embarcados2024";

const char* SSID = "Embarcados_esp32"; 
const char* PASSWORD = "embarcados2024"; 
  
const char* BROKER_MQTT = "2df52b92305e401db97b57e6392c7e06.s1.eu.hivemq.cloud"; 
int BROKER_PORT = 8883;

WiFiClientSecure espClient;
PubSubClient MQTT(espClient);

void init_serial(void);
void init_wifi(void);
void init_mqtt(void);
void reconnect_wifi(void); 
void reconnect_mqtt(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void verifica_conexoes_wifi_mqtt(void);

void setup() 
{
    init_serial();
    init_wifi();
    init_mqtt();
}
  
void init_serial() 
{
    Serial.begin(115200);
}
 
void init_wifi(void) 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconnect_wifi();
}
  
void init_mqtt(void) 
{
    espClient.setInsecure();  // Ignora a verificação do certificado
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);            
}

void reconnect_mqtt(void) 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT, mqtt_user, mqtt_password)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Nova tentativa de conexao em 2s");
            delay(2000);
        }
    }
}
  
void reconnect_wifi() 
{
    if (WiFi.status() == WL_CONNECTED)
        return;
         
    WiFi.begin(SSID, PASSWORD);
     
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
   
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}
 
void verifica_conexoes_wifi_mqtt(void)
{
    reconnect_wifi(); 
    if (!MQTT.connected()) 
        reconnect_mqtt(); 
} 
 
void loop() 
{   
    verifica_conexoes_wifi_mqtt();
    MQTT.publish(TOPICO_PUBLISH, "ESP32 se comunicando com MQTT");
    MQTT.loop();
    delay(1000);   
}
