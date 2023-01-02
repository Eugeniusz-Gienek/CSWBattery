// #include <GxEPD.h>
// #include <GxDEPG0150BN/GxDEPG0150BN.h>    // 1.54" b/w 200x200
// #include <GxIO/GxIO_SPI/GxIO_SPI.h>
// #include <GxIO/GxIO.h>

#include <CSWBattery.h>


////////////////////////////////////////////////////////////////////////
//Constants
////////////////////////////////////////////////////////////////////////

#define PIN_MOTOR 4
#define PIN_KEY 35
#define PWR_EN 5
#define Backlight 33

#define SPI_SCK 14
#define SPI_DIN 13
#define EPD_CS 15
#define EPD_DC 2
#define SRAM_CS -1
#define EPD_RESET 17
#define EPD_BUSY 16

//battery params
//battery display mode possible values
#define VBAT_DISPLAY_ICON 1
#define VBAT_DISPLAY_PERCENTAGE 2
#define VBAT_DISPLAY_VOLTAGE 3
//battery config
#define VBAT_SECTIONS_NUM 5
#define VBAT_VOLTAGE_PRECISION 1
#define VBAT_PIN 34


// int EPD_dispIndex; // The index of the e-Paper's type
// uint8_t EPD_Image[5000] = {0};
// uint16_t EPD_Image_count = 0;
// GxIO_Class io(SPI, /*CS=*/ 15, /*DC=*/2, /*RST=*/17);
// GxEPD_Class display(io, /*RST=*/17, /*BUSY=*/16);

#define ROTATION_DIR 0

#define BTR_TIMEOUT 5000


////////////////////////////////////////////////////////////////////////
//EOF Constants
///////////////////////////////////////////////////////////////////////

int batteryDisplayMode = VBAT_DISPLAY_ICON;

//init our battery
CSWBattery BatteryA(VBAT_PIN, VBAT_VOLTAGE_PRECISION);

void showBatteryCallback() {
  switch (batteryDisplayMode) {
    case VBAT_DISPLAY_PERCENTAGE:
      if (BatteryA.checkIfWeAreCharging() || (BatteryA.getLastBatteryVoltagePercentage() == -1)) {
        Serial.println("The battery is charging.");
      } else {
        Serial.print("Battery level: ");
        Serial.print(BatteryA.getBatteryVoltagePercentage());
        Serial.print("%");
      }
      break;
    case VBAT_DISPLAY_VOLTAGE:
      Serial.print("Battery level: ");
      Serial.printf("%.*f", VBAT_VOLTAGE_PRECISION, BatteryA.getLastBatteryVoltage());
      Serial.print("V");
      break;
    case VBAT_DISPLAY_ICON:
      //draw icon. Required graphics, which is not covered in this example.
    default:
      //VBAT_DISPLAY_ICON
      break;
  }
}

void showBattery(bool force_show = false) {
  if (millis() - BatteryA.getLastCheckTime() <= BTR_TIMEOUT) return;
  BatteryA.setLastCheckTime();
  Serial.println("Battery probing: Voltage = "); Serial.print(BatteryA.getLastBatteryVoltage()); Serial.print(" Volts; ");
  if (force_show || BatteryA.checkBatteryVoltageChanged(batteryDisplayMode)) {
    Serial.println("Retrieving battery status...");
    showBatteryCallback();
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("Battery test");
  //SPI.begin(SPI_SCK, -1, SPI_DIN, EPD_CS);
  pinMode(PWR_EN,OUTPUT);

  digitalWrite(PWR_EN,HIGH);
  
  BatteryA.setSectionsNum(VBAT_SECTIONS_NUM);
  BatteryA.setBatterryCoefficient(1.1);
  BatteryA.setLastCheckTime();
  
  #if defined(DEBUG) && DEBUG>=1
  Serial.println("Setup done.");
  #endif
}
void loop() {
  showBattery();
}
