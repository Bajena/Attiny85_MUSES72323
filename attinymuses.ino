// This sketch is for the ATTiny85 to control the MUSES72323 volume controller.

#include <SPI.h>

// Our ATTiny85 is clocked at 1Mhz.
// 100kHz seems to be an optimal value here - according to the data sheet the clock shouldn't be higher than 1Mhz.
const uint32_t SPI_CLOCK = 100000;
static const SPISettings sMusesSpiSettings(SPI_CLOCK, MSBFIRST, SPI_MODE2);

static const uint16_t internalUnitMute = 0b111111111;
static const uint16_t internalUnit0dB = 0b000100000;

static const uint16_t leftChannelConfig = 0b0000010;
static const uint16_t rightChannelConfig = 0b0000110;

// pin definitions
const int
  DATA_PIN(1),   // data in
  CLOCK_PIN(2),  // shift register clock
  LATCH_PIN(3), // storage register clock (slave select)
  POT_PIN(A2); // PB4

uint16_t joinValues(uint16_t volumeValue, uint16_t channelConfigValue) {
  return (uint16_t)volumeValue << 7 | channelConfigValue;
}

uint16_t fromQuarterOfDecibels(uint16_t quartersOfDb) {
  return internalUnit0dB - quartersOfDb;
}

uint16_t setVolume(uint16_t volume) {
  writeSpi(joinValues(volume, leftChannelConfig));
  writeSpi(joinValues(volume, rightChannelConfig));
}

void writeSpi(uint16_t data) {
  SPI.beginTransaction(sMusesSpiSettings);
  digitalWrite(LATCH_PIN, LOW);
  SPI.transfer16(data);
  digitalWrite(LATCH_PIN, HIGH);
  SPI.endTransaction();
}

void setup() {
  pinMode(POT_PIN, INPUT);
  pinMode(LATCH_PIN, OUTPUT);
  SPI.begin();
}

void loop() {
  long potentiometerValue = analogRead(POT_PIN);
  uint16_t volumeLevel = (uint16_t)map(potentiometerValue, 0, 1023, 447, 0);

  setVolume(fromQuarterOfDecibels(-volumeLevel));

  delay(10);
}
