#include <cox.h>

Timer tSense;

static void taskSense(void *) {
  uint8_t read;

  System.ledToggle(0);

  read = Wire.requestFrom(0x4F, 1);
  printf("Request 1 byte from 0x4F: %u byte read.\n", read);

  while (Wire.available()) {
    printf("%02X ", Wire.read());
  }
  printf("\n");

  read = Wire.requestFrom(0x4F, 2);
  printf("Request 2 byte from 0x4F: %u byte read.\n", read);

  int16_t temperature = 0;

  while (Wire.available()) {
    uint8_t c = Wire.read();
    printf("%02X ", c);
    temperature = (temperature << 8) | c;
  }
  printf("=> %d celcius degree\n", temperature);
}

void setup(void) {
  Serial.begin(115200);
  printf("\n*** I2C Test with AT30TSE758 Temperature Sensor ***\n");

  Wire.begin();

  tSense.onFired(taskSense, NULL);
  tSense.startPeriodic(1000);
}
