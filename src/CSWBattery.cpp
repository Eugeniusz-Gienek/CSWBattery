/**
  ******************************************************************************
  * @file    CSWBattery.cpp
  * @author  Eugene at sky.community
  * @version V1.0.0
  * @date    12-December-2022
  * @brief   The functionality to work with the battery of the smartwatch based on esp32.
  *
  ******************************************************************************
  */

#include "Arduino.h"
#include "CSWBattery.h"

CSWBattery::CSWBattery(int battery_pin, int precision) {
    if(battery_pin != -100) _battery_pin = battery_pin;
    if(precision != -100) _voltage_precision = precision;
}

int CSWBattery::getSectionsNum() {
  return _sections_num;
}
void CSWBattery::setSectionsNum(int sections) {
  _sections_num = sections;
}

void CSWBattery::setBatteryPin(int battery_pin) {
  _battery_pin = battery_pin;
}

int CSWBattery::getBatteryPin() {
  return _battery_pin;
}

int CSWBattery::getVoltagePrecision() {
  return _voltage_precision;
}

void CSWBattery::setVoltagePrecision(int precision) {
  _voltage_precision = precision;
}

void CSWBattery::setBatterryCoefficient(float c) {
  batteryCf=c;
}

float CSWBattery::getBatterryCoefficient() {
  return batteryCf;
}

bool CSWBattery::checkBatteryVoltageChanged(int check_type) {
  switch(check_type) {
    case 1:
      //sections
      return (_last_battery_voltage_section!=this->getBatteryVoltageSection());
    break;
    case 2:
      //percentage
      return (_last_battery_voltage_percentage!=this->getBatteryVoltagePercentage());
    case 3:
      //voltage
    default:
      return (_last_battery_voltage!=this->getBatteryVoltage());
  }
  return __null;
}

unsigned long CSWBattery::getLastCheckTime() {
  return _last_check_tm > 0 ? _last_check_tm : __null;
}
void CSWBattery::setLastCheckTime(unsigned long tm) {
  _last_check_tm = (tm == 0) ? millis() : tm;
}

float CSWBattery::getLastBatteryVoltage() {
  return _last_battery_voltage;
}
void CSWBattery::setLastBatteryVoltage(float v) {
  _last_battery_voltage = v;
}

float CSWBattery::getBatteryVoltage(bool no_update, bool get_raw, int override_precision) {
  int __voltage_precision = (override_precision != -1) ? override_precision : _voltage_precision;
  float __last_battery_voltage = (__voltage_precision >= 2) ?
    ceil(((float)(analogRead(_battery_pin)) / 4095*2*3.3*(get_raw ? 1 : batteryCf))* pow(10.0,__voltage_precision)) / pow(10.0,__voltage_precision)
    :
    (
      (__voltage_precision == 1) ?
      ceil(((float)(analogRead(_battery_pin)) / 4095*2*3.3*(get_raw ? 1 : batteryCf))* 10.0) / 10.0
      :
      floor(((float)(analogRead(_battery_pin)) / 4095*2*3.3*(get_raw ? 1 : batteryCf)))
    );
  if(!no_update) _last_battery_voltage = __last_battery_voltage;
  return __last_battery_voltage;
}

int CSWBattery::getBatteryVoltageSection(bool no_update) {
  float v = this->getBatteryVoltage();
  int voltage_section = -1;
  if (v >= _charging_threshold) voltage_section = -1; else
    voltage_section = ceil((min(v,_fully_charged_voltage)-_fully_uncharged_voltage) * _sections_num/(_fully_charged_voltage-_fully_uncharged_voltage));
  if(!no_update) _last_battery_voltage_section = voltage_section;
  return voltage_section;
}

int CSWBattery::getBatteryVoltagePercentage(bool no_update) {
  float v = this->getBatteryVoltage();
  int voltage_p = -1;
  if (v >= _charging_threshold) voltage_p = -1; else
    voltage_p = round((min(v,_fully_charged_voltage)-_fully_uncharged_voltage)*100/(_fully_charged_voltage-_fully_uncharged_voltage));
  if(!no_update) _last_battery_voltage_percentage = voltage_p;
  return voltage_p;
}

void CSWBattery::setLastBatteryVoltageSection(int s) {
  _last_battery_voltage_section = s;
}

int CSWBattery::getLastBatteryVoltageSection() {
  return _last_battery_voltage_section;
}

void CSWBattery::setLastBatteryVoltagePercentage(int s) {
  _last_battery_voltage_percentage = s;
}

int CSWBattery::getLastBatteryVoltagePercentage() {
  return _last_battery_voltage_percentage;
}

bool CSWBattery::checkIfWeAreCharging() {
  return (this->getBatteryVoltage() >= _charging_threshold);
}

void CSWBattery::setBatteryIsCalibrated(bool cs) {
  _calibrationStatus = cs;
}
bool CSWBattery::getBatteryIsCalibrated() {
  return _calibrationStatus;
}

bool CSWBattery::checkIfEmpty() {
  return (this->getBatteryVoltage() <= _fully_uncharged_voltage);
}

void CSWBattery::resetBattery() {
  batteryCf = defaultBatteryCf;
}

void CSWBattery::calibrateBattery(int precision) {
  float batteryV1=getBatteryVoltage(true);
  float batteryV2=batteryV1;
  while (abs(batteryV1 - batteryV2) <= batteryVCalibrationDiffThreshold) {
    //waiting for the unplug of the charger
    batteryV2 = getBatteryVoltage(true);
    delay(100);
  }
  //now batteryV2 HAS to be equal to _fully_charged_voltage;
  float bat_sum=0;
  for(int i=0;i<_calibrationIterations;i++) {
    batteryV1 = getBatteryVoltage(true, true, precision);
      bat_sum += batteryV1;
      delay(100);
  }
  batteryV1 = bat_sum/_calibrationIterations;
  batteryCf = _fully_charged_voltage / batteryV1;
  this->setBatteryIsCalibrated(true);
}

void CSWBattery::setCalibrationIterations(int c) {
  _calibrationIterations = c;
}
int CSWBattery::getCalibrationIterations() {
  return _calibrationIterations;
}