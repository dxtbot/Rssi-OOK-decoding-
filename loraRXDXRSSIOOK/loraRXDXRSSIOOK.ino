// dexter OOK decoder example using RSSI from Lora //
#include <SPI.h>
#include <LoRa.h>
#define CONFIG_MOSI 27
#define CONFIG_MISO 19
#define CONFIG_CLK  5
#define CONFIG_NSS  18
#define CONFIG_RST  23
#define CONFIG_DIO0 26
#define LORA_PERIOD 433
#define BAND 433E6
//array of frequencies valid for the application to change
long int frequencies[6] = {172450000, 433910000, 433000000, 433000000, 433000000, 433125000};
//controls the current frequency index in the array
int indexFreq = 1;
volatile int incomingPacketSize;
int buttonState = 0;
int rssi = 0;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  LoRa.setGain(20);
  Serial.println("LoRa Receiver");
  SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
  LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.begin(frequencies[indexFreq]);
  LoRa.receive();
  pinMode(25, INPUT);
}

void loop() {
  rssi = LoRa.rssi();
  unsigned long time;
  time = micros() / 100;
  if (rssi < -89 && rssi > -100  ) buttonState = 0;
  if (rssi < -10 && rssi > -88) buttonState = 1;
  delay(100);
  if (buttonState == 1) {
    int tempo = time - 2 ;
    delayMicroseconds(350);
    for (int i = 0; i <= 10; i++) {
      time = micros() / 100;
      rssi = LoRa.rssi();
      if (rssi < -89 && rssi > -100  ) buttonState = 0;
      if (rssi < -10 && rssi > -88) buttonState = 1;
      if (rssi > -90 )Serial.print("1");
      if (rssi < -90 )Serial.print("0");
      //   Serial.print(" TIME ");
      // Serial.print(time - tempo);
      delayMicroseconds(1600);
      Serial.print(" ");
    }
    delay(200);
    Serial.print(" KEY");
    Serial.println(" DONE");
  }
}


