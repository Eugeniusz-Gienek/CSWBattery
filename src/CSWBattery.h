/**
  ******************************************************************************
  * @file    CSWBattery.h
  * @author  Eugene at sky.community
  * @version V1.0.0
  * @date    12-December-2022
  * @brief   The functionality to work with the battery of the smartwatch based on esp32.
  *
  ******************************************************************************
  */
#ifndef CSWBattery_h
#define CSWBattery_h
#include <stdint.h>
#include "Arduino.h"

class CSWBattery {
  public:
    CSWBattery(int battery_pin=-100, int precision=-100);
    void setBatteryPin(int battery_pin);
    int getBatteryPin();
    float getBatteryVoltage(bool no_update=false, bool get_raw=false, int override_precision=-1);
    int getVoltagePrecision();
    void setVoltagePrecision(int precision=-1);
    int getBatteryVoltageSection(bool no_update=false);
    int getBatteryVoltagePercentage(bool no_update=false);
    bool checkIfWeAreCharging();
    bool checkIfEmpty();
    int getSectionsNum();
    void setSectionsNum(int sections);
    unsigned long getLastCheckTime();
    void setLastCheckTime(unsigned long tm=0);
    float getLastBatteryVoltage();
    void setLastBatteryVoltage(float v);
    void setLastBatteryVoltageSection(int s);
    int getLastBatteryVoltageSection(void);
    void setLastBatteryVoltagePercentage(int s);
    int getLastBatteryVoltagePercentage(void);
    void setBatterryCoefficient(float c);
    float getBatterryCoefficient(void);
    bool checkBatteryVoltageChanged(int check_type=1);
    void calibrateBattery(int precision=1);
    void resetBattery();
    void setCalibrationIterations(int c);
    int getCalibrationIterations();
    void setBatteryIsCalibrated(bool cs=true);
    bool getBatteryIsCalibrated();
  private:
    const float _fully_charged_voltage=4.2;
    const float _fully_uncharged_voltage=3.7;
    const float _charging_threshold=4.3;
    const float defaultBatteryCf=1.1;
    float _last_battery_voltage=-1;
    int _last_battery_voltage_section=-1;
    int _last_battery_voltage_percentage=-1;
    int _voltage_precision=1;
    int _sections_num = 5;
    int _battery_pin=-100;
    unsigned long _last_check_tm=0;
    float batteryCf=1.1;
    float batteryVCalibrationDiffThreshold=0.3;
    int _calibrationIterations = 10;
    bool _calibrationStatus=false;
};
#endif
