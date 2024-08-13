#include <LiquidCrystal_I2C.h>

#include <Wire.h> 
#include <SPI.h> 
#include <MFRC522.h> 

/* RFID */ 
#define SS_PIN 10 //PIN SDA
#define RST_PIN 9 //PIN RESET
 
MFRC522 rfid(SS_PIN, RST_PIN); 

/* LED */ 
const int pinoLedVerde = 3; //LED VERDE
const int pinoLedVermelho = 2; //LED VERMELHO

/* Display */
#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

void setup() {
  Serial.begin(9600);
  /* Display */
  // Initialize the LCD display and turn on backlight
  lcd.init();
  lcd.backlight();

  /* RFID */
  Wire.begin(); //INICIALIZA A BIBLIOTECA WIRE
  SPI.begin(); //INICIALIZA O BARRAMENTO SPI
  rfid.PCD_Init(); //INICIALIZA MFRC522
}

void loop() {
  leituraRfid();
}

//FUNÇÃO DE VALIDAÇÃO DA TAG RFID
void leituraRfid(){
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) //VERIFICA SE O CARTÃO PRESENTE NO LEITOR É DIFERENTE DO ÚLTIMO CARTÃO LIDO. CASO NÃO SEJA, FAZ
    return; //RETORNA PARA LER NOVAMENTE
 
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
  }else{ //Senão, ag não autorizada    
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Acesso");
    lcd.setCursor(2, 1);
    lcd.print("Negado");

    Serial.print("Identificador (UID) da tag negada: "); 
    Serial.println(strID); 
  }
  rfid.PICC_HaltA(); //PARADA DA LEITURA DO CARTÃO
  rfid.PCD_StopCrypto1(); //PARADA DA CRIPTOGRAFIA NO PCD

  delay(3000);

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Aproxime a tag");
}

