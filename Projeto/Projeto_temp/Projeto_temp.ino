
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS1 15
OneWire oneWire1(ONE_WIRE_BUS1);
DallasTemperature sensor1(&oneWire1);

#define ONE_WIRE_BUS2 18
OneWire oneWire2(ONE_WIRE_BUS2);
DallasTemperature sensor2(&oneWire2);

void setup(void){
  Serial.begin(115200);
  sensor1.begin();
  sensor2.begin();
}

void loop(void){ 
  sensor1.requestTemperatures();
  sensor2.requestTemperatures();
  
  Serial.print("Sensor 1(*C): ");
  Serial.print(sensor1.getTempCByIndex(0));

  Serial.print(" | Sensor 2(*C): ");
  Serial.println(sensor2.getTempCByIndex(0));
  
  delay(2000);
}
