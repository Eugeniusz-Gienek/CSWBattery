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
  if(DEBUG && (DEBUG_LEVEL >=1) && Serial) {
    Serial.println("[CSWBattery] Initializing battery.");
  }
  if(DEBUG && (DEBUG_LEVEL >=10) && Serial) {
    Serial.print("[CSWBattery] Battery params: ");
    Serial.print("  - battery pin: ");
    Serial.println(battery_pin);
    Serial.print("  - precision: ");
    Serial.println(precision);
  }
  if(battery_pin != -100) _battery_pin = battery_pin;
  if(precision != -100) _voltage_precision = precision;
}

int CSWBattery::getSectionsNum() {
  return _sections_num;
}
void CSWBattery::setSectionsNum(int sections) {
  if(DEBUG && (DEBUG_LEVEL >=1) && Serial) {
    Serial.print("[CSWBattery] Setting sections num to: ");
    Serial.println(sections);
  }
  _sections_num = sections;
}

void CSWBattery::setBatteryPin(int battery_pin) {
  if(DEBUG && (DEBUG_LEVEL >=1) && Serial) {
    Serial.print("[CSWBattery] Setting battery pin to: ");
    Serial.println(battery_pin);
  }
  _battery_pin = battery_pin;
}

int CSWBattery::getBatteryPin() {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    Serial.print("[CSWBattery] Getting battery pin: ");
    Serial.println(_battery_pin);
  }
  return _battery_pin;
}

int CSWBattery::getVoltagePrecision() {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    Serial.print("[CSWBattery] Getting voltage precision: ");
    Serial.println(_voltage_precision);
  }
  return _voltage_precision;
}

void CSWBattery::setVoltagePrecision(int precision) {
  if(DEBUG && (DEBUG_LEVEL >=1) && Serial) {
    Serial.print("[CSWBattery] Setting voltage precision to: ");
    Serial.println(precision);
  }
  _voltage_precision = precision;
}

void CSWBattery::setBatteryCoefficient(float c) {
  if(DEBUG && (DEBUG_LEVEL >=1) && Serial) {
    Serial.print("[CSWBattery] Setting battery coefficient to: ");
    Serial.println(c);
  }
  batteryCf=c;
}

float CSWBattery::getBatteryCoefficient(bool get_default) {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    Serial.print("[CSWBattery] Getting battery coefficient");
    Serial.print(get_default ? " (default one)" : "");
    Serial.print(": ");
    Serial.println(get_default ? defaultBatteryCf : batteryCf);
  }
  return get_default ? defaultBatteryCf : batteryCf;
}

bool CSWBattery::checkBatteryVoltageChanged(int check_type) {
  bool res = true;
  int _lbvi;
  int _bvi;
  float _lbv;
  float _bv;
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    Serial.print("[CSWBattery] Checking if battery voltage has changed. Type of check is: ");
  }
  switch(check_type) {
    case 1:
      //sections
      if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
        Serial.print("sections: ");
      }
      _lbvi = _last_battery_voltage_section;
      _bvi = this->getBatteryVoltageSection();
      if (_lbvi!=_bvi) {
        for(int i=0;i<_battery_check_times;i++) {
          _bvi = this->getBatteryVoltageSection();
          if (_lbvi == _bvi) {
            if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
              Serial.print("Got fluke.");
            }
            res = false; //fluke
          }
          delay(_battery_check_delay_ms);
        }
        if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
          Serial.println(res ? "Changed." : "Not changed.");
        }
        return res;
      } else {
        if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
          Serial.println("Not changed.");
        }
        return false;
      }
    break;
    case 2:
      //percentage
      if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
        Serial.print("precentage: ");
      }
      _lbvi = _last_battery_voltage_percentage;
      _bvi = this->getBatteryVoltagePercentage();
      if (_lbvi!=_bvi) {
        for(int i=0;i<_battery_check_times;i++) {
          _bvi = this->getBatteryVoltagePercentage();
          if (_lbvi == _bvi) {
            if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
              Serial.print("Got fluke.");
            }
            res = false; //fluke
          }
          delay(_battery_check_delay_ms);
        }
        if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
          Serial.println(res ? "Changed." : "Not changed.");
        }
        return res;
      } else {
        if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
          Serial.println("Not changed.");
        }
        return false;
      }
      //return (_last_battery_voltage_percentage!=this->getBatteryVoltagePercentage());
    case 3:
      //voltage
    default:
      if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
        Serial.print("voltage: ");
      }
      _lbv = _last_battery_voltage;
      _bv = this->getBatteryVoltage();
      if (_lbv!=_bv) {
        for(int i=0;i<_battery_check_times;i++) {
          _bv = this->getBatteryVoltage();
          if (_lbv == _bv) {
            if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
              Serial.print("Got fluke.");
            }
            res = false; //fluke
          }
          delay(_battery_check_delay_ms);
        }
        if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
          Serial.println(res ? "Changed." : "Not changed.");
        }
        return res;
      } else {
        if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
          Serial.println("Not changed.");
        }
        return false;
      }
  }
  return __null;
}

unsigned long CSWBattery::getLastCheckTime() {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    Serial.print("[CSWBattery] Getting last check time: ");
    if(_last_check_tm > 0) Serial.println(_last_check_tm); else Serial.println("NULL");
  }
  return (_last_check_tm > 0) ? _last_check_tm : __null;
}
void CSWBattery::setLastCheckTime(unsigned long tm) {
  if(DEBUG && (DEBUG_LEVEL >=10) && Serial) {
    Serial.print("[CSWBattery] Setting last check time to: ");
    Serial.println(tm);
  }
  _last_check_tm = (tm == 0) ? millis() : tm;
}

float CSWBattery::getLastBatteryVoltage() {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    Serial.print("[CSWBattery] Getting last battery voltage: ");
    Serial.println(_last_battery_voltage);
  }
  return _last_battery_voltage;
}
void CSWBattery::setLastBatteryVoltage(float v) {
  if(DEBUG && (DEBUG_LEVEL >=10) && Serial) {
    Serial.print("[CSWBattery] Setting battery last voltage to: ");
    Serial.println(v);
  }
  _last_battery_voltage = v;
}

float CSWBattery::getBatteryVoltage(bool no_update, bool get_raw, int override_precision, bool check_thoroughly, bool use_default_cf, int override_num_checks_thoroughly) {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    Serial.println("[CSWBattery] Getting battery voltage with params: ");
    if(no_update) Serial.println("- no update;");
    if(get_raw) {
      Serial.print("  - get raw data (without coefficient - current value is ");
      Serial.print(this->getBatteryCoefficient());
      Serial.println(");");
    }
    if(override_precision != -1) {
      Serial.print("  - override battery precision to: ");
      Serial.print(override_precision);
      Serial.println(");");
    }
    if(check_thoroughly) {
      Serial.print("  - check thoroughly (");
      Serial.print((override_num_checks_thoroughly != -1) ? _battery_check_times : override_num_checks_thoroughly);
      Serial.print(" times with a delay ");
      Serial.print(_battery_check_delay_ms);
      Serial.println(" ms and afterwards get the average value);");
    }
    if(use_default_cf) {
      Serial.print("  - using default coefficient: ");
      Serial.print(this->getBatteryCoefficient(true));
      Serial.println(");");
    }
    if(override_num_checks_thoroughly != -1) {
      Serial.print("  - override number of checks for thoroughly checking to: ");
      Serial.print(override_num_checks_thoroughly);
      Serial.println(");");
    }
  }
  int __voltage_precision = (override_precision != -1) ? override_precision : _voltage_precision;
  float __last_battery_voltage=0;
  float __batteryCf = this->getBatteryCoefficient(use_default_cf);
  if(check_thoroughly) {
    int __battery_check_times = (override_num_checks_thoroughly == -1) ? _battery_check_times : override_num_checks_thoroughly;
    for(int i=0;i<__battery_check_times;i++) {
      __last_battery_voltage += (__voltage_precision >= 2) ?
        ceil(((float)(analogRead(_battery_pin)) / 4095*2*3.3*(get_raw ? 1 : __batteryCf))* pow(10.0,__voltage_precision)) / pow(10.0,__voltage_precision)
        :
        (
          (__voltage_precision == 1) ?
          ceil(((float)(analogRead(_battery_pin)) / 4095*2*3.3*(get_raw ? 1 : __batteryCf))* 10.0) / 10.0
          :
          floor(((float)(analogRead(_battery_pin)) / 4095*2*3.3*(get_raw ? 1 : batteryCf)))
        );
        delay(_battery_check_delay_ms);
    }
    __last_battery_voltage = __last_battery_voltage/__battery_check_times;
  } else {
    __last_battery_voltage = (__voltage_precision >= 2) ?
      ceil(((float)(analogRead(_battery_pin)) / 4095*2*3.3*(get_raw ? 1 : __batteryCf))* pow(10.0,__voltage_precision)) / pow(10.0,__voltage_precision)
      :
      (
        (__voltage_precision == 1) ?
        ceil(((float)(analogRead(_battery_pin)) / 4095*2*3.3*(get_raw ? 1 : __batteryCf))* 10.0) / 10.0
        :
        floor(((float)(analogRead(_battery_pin)) / 4095*2*3.3*(get_raw ? 1 : __batteryCf)))
      );
  }
  if(!no_update) _last_battery_voltage = __last_battery_voltage;
  return __last_battery_voltage;
}

int CSWBattery::getBatteryVoltageSection(bool no_update, bool check_thoroughly) {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    Serial.print("[CSWBattery] Getting battery voltage section with params: ");
    Serial.println(no_update ? "  - without update;" : "");
    Serial.println(check_thoroughly ? " - check thoroughly;" : "");
  }
  float v = this->getBatteryVoltage(false, false, -1, check_thoroughly);
  int voltage_section = -1;
  if (v >= _charging_threshold) voltage_section = -1; else {
    voltage_section = ceil((min(v,_fully_charged_voltage)-_fully_uncharged_voltage) * _sections_num/(_fully_charged_voltage-_fully_uncharged_voltage));
  }
  if(!no_update) _last_battery_voltage_section = voltage_section;
  return voltage_section;
}

int CSWBattery::getBatteryVoltagePercentage(bool no_update, bool check_thoroughly) {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    Serial.print("[CSWBattery] Getting battery voltage percentage with params: ");
    Serial.println(no_update ? "  - without update;" : "");
    Serial.println(check_thoroughly ? " - check thoroughly;" : "");
  }
  float v = this->getBatteryVoltage(false, false, -1, check_thoroughly);
  int voltage_p = -1;
  if (v >= _charging_threshold) voltage_p = -1; else {
    voltage_p = round((min(v,_fully_charged_voltage)-_fully_uncharged_voltage)*100/(_fully_charged_voltage-_fully_uncharged_voltage));
  }
  if(!no_update) _last_battery_voltage_percentage = voltage_p;
  return voltage_p;
}

void CSWBattery::setLastBatteryVoltageSection(int s) {
  if(DEBUG && (DEBUG_LEVEL >=10) && Serial) {
    Serial.print("[CSWBattery] Setting last battery voltage section to: ");
    Serial.println(s);
  }
  _last_battery_voltage_section = s;
}

int CSWBattery::getLastBatteryVoltageSection() {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    Serial.print("[CSWBattery] Getting battery last voltage section: ");
    Serial.println(_last_battery_voltage_section);
  }
  return _last_battery_voltage_section;
}

void CSWBattery::setLastBatteryVoltagePercentage(int s) {
  if(DEBUG && (DEBUG_LEVEL >=10) && Serial) {
    Serial.print("[CSWBattery] Setting last battery voltage percentage time to: ");
    Serial.println(s);
  }
  _last_battery_voltage_percentage = s;
}

int CSWBattery::getLastBatteryVoltagePercentage() {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    Serial.print("[CSWBattery] Getting battery last voltage percentage: ");
    Serial.println(_last_battery_voltage_percentage);
  }
  return _last_battery_voltage_percentage;
}

bool CSWBattery::checkIfWeAreCharging() {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    bool res = (this->getBatteryVoltage() >= _charging_threshold);
    Serial.print("[CSWBattery] Checking if we are charging: ");
    Serial.println(res ? "yes." : "no.");
    return res;
  }
  else return (this->getBatteryVoltage() >= _charging_threshold);
}

void CSWBattery::setBatteryIsCalibrated(bool cs) {
  if(DEBUG && (DEBUG_LEVEL >=10) && Serial) {
    Serial.print("[CSWBattery] Setting battery calibration to: ");
    Serial.println(cs ? "calibrated." : "not calibrated.");
  }
  _calibrationStatus = cs;
}
bool CSWBattery::getBatteryIsCalibrated() {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    Serial.print("[CSWBattery] Checking if battery is calibrated: ");
    Serial.println(_calibrationStatus ? "yes." : "no.");
  }
  return _calibrationStatus;
}

bool CSWBattery::checkIfEmpty() {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    bool res = (this->getBatteryVoltage() <= _fully_uncharged_voltage);
    Serial.print("[CSWBattery] Checking if battery is empty: ");
    Serial.println(res ? "yes." : "no.");
    return res;
  }
  else return (this->getBatteryVoltage() <= _fully_uncharged_voltage);
}

void CSWBattery::resetBattery() {
  if(DEBUG && (DEBUG_LEVEL >=1) && Serial) {
    Serial.print("[CSWBattery] Resetting battery coefficient. New value is: ");
    Serial.println(defaultBatteryCf);
  }
  batteryCf = defaultBatteryCf;
  this->setBatteryIsCalibrated(false);
}

void CSWBattery::calibrateBattery(int precision) {
  if(DEBUG && (DEBUG_LEVEL >=1) && Serial) {
    Serial.print("[CSWBattery] Calibrating battery. Current coefficient value is: ");
    Serial.println(this->getBatteryCoefficient());
  }
  float batteryV1=getBatteryVoltage(true, false, -1, true, true);
  float batteryV2=batteryV1;
  // we olny care about the difference between the values - the first
  // value (on charger) is none of our interest
  while (abs(batteryV1 - batteryV2) <= batteryVCalibrationDiffThreshold) {
    //waiting for the unplug of the charger
    batteryV2 = getBatteryVoltage(true, false, -1, true, true);
    delay(100);
  }
  // because when the board is unplugged the battery voltage can go
  // dramatically down and then back to normal
  delay(batteryCalibrationDelayMS);
  //now batteryV2 HAS to be equal to _fully_charged_voltage;
  batteryV1 = getBatteryVoltage(true, true, precision, true, false, _calibrationIterations);
  batteryCf = _fully_charged_voltage / batteryV1;
  if(DEBUG && (DEBUG_LEVEL >=1) && Serial) {
    Serial.print("[CSWBattery] New coefficient value is: ");
    Serial.println(this->getBatteryCoefficient());
  }
  this->setBatteryIsCalibrated(true);
}

void CSWBattery::setCalibrationIterations(int c) {
  if(DEBUG && (DEBUG_LEVEL >=1) && Serial) {
    Serial.print("[CSWBattery] Setting battery calibration interations to: ");
    Serial.println(c);
  }
  _calibrationIterations = c;
}
int CSWBattery::getCalibrationIterations() {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    Serial.print("[CSWBattery] Getting calibration iterations: ");
    Serial.println(_calibrationIterations);
  }
  return _calibrationIterations;
}

void CSWBattery::setDebugLevel(int d) {
  if(DEBUG && Serial) {
    Serial.print("[CSWBattery] Setting debug level to: ");
    Serial.println(d);
  }
  if(d >= -1) DEBUG_LEVEL = d;
}

int CSWBattery::getDebugLevel(void) {
  if(DEBUG && (DEBUG_LEVEL >=99) && Serial) {
    Serial.print("[CSWBattery] Getting debug level: ");
    Serial.println(DEBUG_LEVEL);
  }
  return DEBUG_LEVEL;
}
void CSWBattery::setDebug(bool d) {
  if(DEBUG && Serial) {
    Serial.print("[CSWBattery] Setting debug to: ");
    Serial.println(d ? "on." : "off");
  }
  DEBUG = d;
}
void CSWBattery::enableDebug(void) {
  if(DEBUG && Serial) {
    Serial.print("[CSWBattery] Setting debug to: on.");
  }
  DEBUG = true;
}
void CSWBattery::disableDebug(void) {
  if(DEBUG && Serial) {
    Serial.print("[CSWBattery] Setting debug to: off.");
  }
  DEBUG = false;
}