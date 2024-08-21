#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

#include <Wire.h> 
#include <SPI.h> 
#include <MFRC522.h> 

/* RFID */ 
#define SS_PIN    21
#define RST_PIN   22
 
MFRC522 rfid(SS_PIN, RST_PIN); 

/* Display */
#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

#define I2C_SDA   33
#define I2C_SCL   32
TwoWire I2CBME = TwoWire(0);

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

// Wifi
const char* ssid = "Embarcados_esp32";
const char* password = "embarcados2024";
WiFiServer server(80);

// Menu
int opcao = -1;
int allowed = 1;

void setup() {
  Serial.begin(115200);

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

  /* RFID */
  Wire.begin(); //INICIALIZA A BIBLIOTECA WIRE
  SPI.begin(); //INICIALIZA O BARRAMENTO SPI
  rfid.PCD_Init(); //INICIALIZA MFRC522

  /* Display */
  // Initialize the LCD display and turn on backlight
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
}

void loop() {
  // Wifi
  WiFiClient client = server.available();
  Serial.println(client);
  if (client) {
    Serial.println("==========================New Client.=================================");
    String currentLine = "";
    while (client.connected()) {
      Serial.print("-");
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<h1>Sistemas Computacionais embarcado - ESP32 RFID</h1>");
            client.println();
            switch(opcao){
              case 1:
                client.println("<p>TAG LIDO</p>");
                client.print("<p>Acesso: ");
                (allowed == 1 ? client.print("Aprovado") : client.print("Negado"));
                client.println("</p>");
                break;
              default:
                client.println("<p>...</p>");
            }

            client.print("Click <a href=\"/READ\">here</a> to read.<br>");
            break;

          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        if (currentLine.endsWith("GET /READ")) {
          Serial.println("READING");
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Aproxime a tag");
          allowed = leituraRfid();
          Serial.print("Read Result: ");
          Serial.println(allowed); 
          opcao = 1;         
        }
        if (currentLine.endsWith("GET /L")) {
          Serial.println("Turning LED OFF");
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
  Serial.println("END");
}

int leituraRfid(){
  int loop = -1;
  while(loop == -1){
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()){
      
    } else {
      /***TAG RFID LIDA***/
      String strID = ""; 
      for (byte i = 0; i < 4; i++) {
        strID +=
        (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
        String(rfid.uid.uidByte[i], HEX) +
        (i!=3 ? ":" : "");
      }
      strID.toUpperCase();
    
      if ((strID.indexOf("A3:FD:96:A9") >= 0) || (strID.indexOf("A3:FC:8C:A9") >= 0)){ // Autoriza a tag 
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Acesso");
        lcd.setCursor(2, 1);
        lcd.print("Autorizado");
        
        Serial.print("Identificador (UID) da tag autorizada: "); 
        Serial.println(strID);


        rfid.PICC_HaltA(); //PARADA DA LEITURA DO CARTÃO
        rfid.PCD_StopCrypto1(); //PARADA DA CRIPTOGRAFIA NO PCD

        loop = 1;
        return(1);
      }else{ //Senão, ag não autorizada    
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Acesso");
        lcd.setCursor(2, 1);
        lcd.print("Negado");

        Serial.print("Identificador (UID) da tag negada: "); 
        Serial.println(strID); 


        rfid.PICC_HaltA(); //PARADA DA LEITURA DO CARTÃO
        rfid.PCD_StopCrypto1(); //PARADA DA CRIPTOGRAFIA NO PCD
        
        loop = 0;
        return(0);
      }
    }

  }
}