#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <ADS1115_WE.h>

#define V2 ADS1115_COMP_0_GND
#define V1 ADS1115_COMP_1_GND

Adafruit_MCP4725 dac;
ADS1115_WE adc = ADS1115_WE(0x48);
float V0 = 1.5, I = 21.;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  dac.begin(0x60);
  dac.setVoltage((V0 * 4095) / 5, false);

  if (!adc.init())
    Serial.println("ADS1115 not connected!");
  adc.setVoltageRange_mV(ADS1115_RANGE_6144);

  delay(500);
}

void loop()
{
  if (I <= 20 && V0 < 4)
  {
    dac.setVoltage((V0 * 4095) / 5, false);
    delay(100);
    I = read();
    V0 += 0.01;
  }
  else
  {
    Serial.println("start");
    V0 = 1.5;
    dac.setVoltage((V0 * 4095) / 5, false);
    delay(100);
    I = read();
  }
}

float read()
{
  String output = "";
  char buff[10];
  float temp_V1 = readChannel(V1);
  float temp_V2 = readChannel(V2);
  float voltage = temp_V1 - temp_V2;
  dtostrf(voltage, 4, 6, buff);
  output += buff;
  output += " ";
  float intensity = temp_V2 / 51. * 1000;
  dtostrf(intensity, 4, 6, buff);
  output += buff;
  Serial.println(output);
  return intensity;
}

float readChannel(ADS1115_MUX channel)
{
  float voltage = 0.0;
  adc.setCompareChannels(channel);
  adc.startSingleMeasurement();
  while (adc.isBusy())
  {
  }
  voltage = adc.getResult_V();
  return voltage;
}