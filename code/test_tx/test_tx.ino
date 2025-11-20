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
#define LORA_IRQ 2   // DIO0

long counter = 0;

void setup() {
  Serial.begin(115200);

  // OLED INIT
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("TX Starting...");
  display.display();

  // LoRa INIT
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
  LoRa.setCodingRate4(7);
  LoRa.enableCrc();

  display.clearDisplay();
  display.println("LoRa TX Ready!");
  display.display();
  delay(1000);
}

void loop() {
  // String msg = "Hello World " + String(counter);
  byte bitValue = counter % 2;

  
  LoRa.beginPacket();
  LoRa.print(bitValue);
  LoRa.endPacket();


  // Display to OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.printf("TX bit: %d\n", bitValue);
  display.display();

  counter++;
  delay(500);
}
