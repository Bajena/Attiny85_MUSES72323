// This sketch is for the ATTiny85 to control the MUSES72323 volume controller.

#include <SPI.h>

// Our ATTiny85 is clocked at 1Mhz.
// 100kHz seems to be an optimal value here - according to the data sheet the clock shouldn't be higher than 1Mhz.
static const SPISettings sMusesSpiSettings(100000, MSBFIRST, SPI_MODE2);

// pin definitions
const int
  DATA_PIN(1),   // data in
  CLOCK_PIN(2),  // shift register clock
  LATCH_PIN(3); // storage register clock (slave select)

void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  SPI.begin();
}

void writeSpi(uint16_t data) {
  SPI.beginTransaction(sMusesSpiSettings);
  digitalWrite(LATCH_PIN, LOW);
  SPI.transfer16(data);
  digitalWrite(LATCH_PIN, HIGH);
  SPI.endTransaction();
}

void loop() {
  // L channel volume
  writeSpi(0b0001000000000010);
  // R channel volume 0db
  writeSpi(0b0001000000000110);

  delay(1000);

  // L channel volume
  writeSpi(0b11111111100000010);
  // R channel volume 0db
  writeSpi(0b11111111100000110);

  delay(1000);

  // Gain
  // writeSpi(0b1000000000001010);

  // // Config
  // writeSpi(0b0000000000001110);
}
