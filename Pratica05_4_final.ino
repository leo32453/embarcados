#include <WiFi.h>
#include <math.h>

// Temperatura
const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = 34;     // Grove - Temperature Sensor connected to D34 (ESP32)
                                  // D15 tem a mesma conexão do Wifi

// Wifi
const char* ssid = "Embarcados_esp32";
const char* password = "embarcados2024";
int LED = 4; //Pino D4
 WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectada.");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  float T_soma = 0;
  int ADC = analogRead(pinTempSensor);

  // Mede 30 temperaturas
  for(int i=0; i<30; i++){
    ADC = analogRead(pinTempSensor);
    // Convert to resistance
    float R = 4095.0 / ADC - 1.0;    // 4095 is the maximum ADC value for ESP32
    R = R0 * R;
    // Calculate temperature
    float T = 1.0 / (log(R / R0) / B + 1 / 298.15) - 273.15;

    //Serial.println(T);
    T_soma = T_soma + T;

    delay(100);
  }

  // Media
  float T_media = T_soma/30;
  Serial.print("Media: ");
  Serial.println(T_media);

  if (T_media > 27){
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  // Wifi
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<h1>Sistemas Computacionais embarcado - ESP32 wifi</h1>");
            client.println();
            client.print("<p>Temperatura Media: ");
            client.print(T_media);
            client.println("</p>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
  delay(1000);
}