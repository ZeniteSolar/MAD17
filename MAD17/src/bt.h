/*
 * bt.h
 *
 * Created: 16/12/2017 01:05:25
 *  Author: Natan Ogliari
 */ 


#ifndef BT_H_
#define BT_H_

#include "usart.h"

#define HEADER_SIZE           2
#define SUM_SIZE              1

uint64_t timestamp;

struct  __attribute__((packed)) sensors { // 20 bytes
	uint16_t  InputCurrent,
	OutputCurrent,
	MotorVoltage,
	BatteryVoltage,
	MotorTemperature,
	BatteryTemperature,
	MotorRPM,
	WaterSpeed,
	Torque1,
	Torque2;
};

struct  __attribute__((packed)) package { // 28 bytes
	uint32_t  header;       //4 bytes
	uint64_t  timestamp;    //8 bytes
	uint16_t  dataSize;     //2 bytes
	uint16_t  sum;          //2 bytes
	struct    sensors data; //x bytes
};

// bluetooth functions
void bt_send_package(void);
uint8_t calculate_check_sum(uint8_t *bytes, int8_t dataLenght);

volatile struct package mpackage;

#endif /* BT_H_ */