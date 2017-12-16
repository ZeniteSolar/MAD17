/*
 * bt.c
 *
 * Created: 16/12/2017 01:05:11
 *  Author: Natan Ogliari
 */ 

#include "bt.h"
#include "machine.h"

void bt_send_package(void)
{
  mpackage.header = 0xFFFFFFFF;
  mpackage.timestamp = timestamp++;
  mpackage.dataSize = 20;           // size in bytes
  mpackage.data.InputCurrent = control.InCurrent;
  mpackage.data.OutputCurrent = control.OutCurrent;
  mpackage.data.MotorVoltage = control.MotorVoltage;
  mpackage.data.BatteryVoltage = control.BatteryVoltage;
  mpackage.data.MotorTemperature = control.MotorTemperature;
  mpackage.data.BatteryTemperature = control.BatteryTemperature;
  mpackage.data.MotorRPM = 0;
  mpackage.data.WaterSpeed = 0;
  mpackage.data.Torque1 = 0;
  mpackage.data.Torque2 = 0;
  mpackage.sum = calculate_check_sum( (uint8_t*) & (mpackage.data), sizeof(mpackage.data));

  usart_send_buffer((uint8_t*) &mpackage, sizeof(mpackage));
/*
#ifdef DEBUG
  Serial.print(timestamp); Serial.print("::\t");
  Serial.print(mpackage.header,HEX); Serial.print(",");
  Serial.print(mpackage.timestamp*1.f); Serial.print(":,");
  Serial.print(mpackage.dataSize,HEX); Serial.print(", ");
  Serial.print(mpackage.data.InputCurrent,HEX); Serial.print(", ");
  Serial.print(mpackage.data.OutputCurrent,HEX); Serial.print(", ");
  Serial.print(mpackage.data.MotorVoltage,HEX); Serial.print(", ");
  Serial.print(mpackage.data.BatteryVoltage,HEX); Serial.print(", ");
  Serial.print(mpackage.data.MotorTemperature,HEX); Serial.print(", ");
  Serial.print(mpackage.data.BatteryTemperature,HEX); Serial.print(", ");
  Serial.print(mpackage.data.MotorRPM,HEX); Serial.print(", ");
  Serial.print(mpackage.data.WaterSpeed,HEX); Serial.print(", ");
  Serial.print(mpackage.data.Torque1,HEX); Serial.print(", ");
  Serial.print(mpackage.data.Torque2,HEX); Serial.print("\n");
#endif
#ifdef DEBUG
  Serial.print(timestamp); Serial.print("::\t");
  Serial.print(mpackage.header, HEX); Serial.print(",");
  Serial.print(mpackage.timestamp*1.f); Serial.print(":,");
  Serial.print(mpackage.dataSize); Serial.print(", ");
  Serial.print(mpackage.data.InputCurrent); Serial.print(", ");
  Serial.print(mpackage.data.OutputCurrent); Serial.print(", ");
  Serial.print(mpackage.data.MotorVoltage); Serial.print(", ");
  Serial.print(mpackage.data.BatteryVoltage); Serial.print(", ");
  Serial.print(mpackage.data.MotorTemperature); Serial.print(", ");
  Serial.print(mpackage.data.BatteryTemperature); Serial.print(", ");
  Serial.print(mpackage.data.MotorRPM); Serial.print(", ");
  Serial.print(mpackage.data.WaterSpeed); Serial.print(", ");
  Serial.print(mpackage.data.Torque1); Serial.print(", ");
  Serial.print(mpackage.data.Torque2); Serial.print("\n");
#endif 
*/
  /*Serial.println("A package has been sent:");
    unsigned char *pkg_array = (unsigned char*) &mpackage;
    for(unsigned char i = 0;i < sizeof(mpackage); i++){
    //blueToothSerial.write(pkg_array[i]);
    Serial.print("0x");
    Serial.print(pkg_array[i], HEX);
    Serial.print("\t");
    }
    Serial.println();
  */ 
 
}


/**
   @brief Makes a 8bits Integer check sum w/ one's complement
   @param *data is a int pointer to the prealocated data
   @param dataLenght is the length of data
   @param checkSumByte indicates where the check sum byte is in the data
*/
uint8_t calculate_check_sum(uint8_t *bytes, int8_t dataLenght)
{
  uint16_t sum = 0;
  uint8_t *bytes_start = bytes;     // keeps the pointer reference
  for (dataLenght -= 1; dataLenght >= 0; dataLenght--) {
    if (((sum += bytes[dataLenght]) & 0xFF00) != 0) {
      sum = (uint16_t)((~sum) & 0xFF); // ones' complement
    }
  }
  bytes = bytes_start;              // takes back the pointer reference

  return (uint8_t) (sum & 0xFF);
}
