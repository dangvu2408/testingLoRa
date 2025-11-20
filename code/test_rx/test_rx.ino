#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// OLED pins
#define OLED_SDA 21
#define OLED_SCL 22

// LoRa pins
#define LORA_SCK 18
#define LORA_MISO 19
#define LORA_MOSI 23
#define LORA_CS 15
#define LORA_RST 14
#define LORA_IRQ 2

int currentCR = 7;
unsigned long lastReceiveTime = 0;
float bitrate = 0;

void setup() {
  Serial.begin(115200);

  // OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("RX Starting...");
  display.display();

  // LoRa
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI);
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);

  if (!LoRa.begin(433E6)) {
    display.clearDisplay();
    display.println("LoRa FAIL!");
    display.display();
    while (1);
  }

  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(currentCR);
  LoRa.enableCrc();

  display.clearDisplay();
  display.println("LoRa RX Ready!");
  display.display();
}

void loop() {
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    String msg = LoRa.readString();
    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    unsigned long now = micros();

    if (lastReceiveTime != 0) {
      float dt = (now - lastReceiveTime) / 1000000.0; // giây giữa 2 gói
      bitrate = 1.0 / dt; // vì TX gửi 1 bit mỗi lần
    }

    lastReceiveTime = now;

    Serial.printf("RX: %s | RSSI=%d | SNR=%.2f | CR=%d\n",
                  msg.c_str(), rssi, snr, currentCR);

    // Display
    display.clearDisplay();
    display.setCursor(0, 0);
    display.printf("RX: %s\n", msg.c_str());
    display.printf("RSSI: %d dBm\n", rssi);
    display.printf("SNR : %.2f dB\n", snr);
    display.printf("CR  : %d\n", currentCR);
    display.printf("Rate: %.2f bps\n", bitrate);
    display.display();
  }
}
