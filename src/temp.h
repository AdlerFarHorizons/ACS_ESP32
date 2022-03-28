#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS_1 26
OneWire oneWire_in(ONE_WIRE_BUS_1);
DallasTemperature sensor_inhouse(&oneWire_in);

float ext_temp;

void temp_setup(){
    Serial.println("Dallas Temperature Control Library Demo - TwoPin_DS18B20");
    sensor_inhouse.begin();
}

void get_temp(){
    sensor_inhouse.requestTemperatures();
    ext_temp =  sensor_inhouse.getTempCByIndex(0);
}

