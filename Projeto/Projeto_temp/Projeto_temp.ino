
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress sensor1 = { 0x28, 0x0, 0x3, 0x83, 0x0, 0x0, 0x0, 0x1 };

void setup(void){
  Serial.begin(115200);
  sensors.begin();
}

void loop(void){ 
  sensors.requestTemperatures();
  
  Serial.print("Sensor 1(*C): ");
  Serial.println(sensors.getTempC(sensor1));
  
  delay(2000);
}
