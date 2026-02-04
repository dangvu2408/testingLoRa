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

/* ================= STATISTICS ================= */
long lastSeq = -1;              // SEQ gói trước
unsigned long okPackets = 0;    // Gói đúng
unsigned long errPackets = 0;   // Gói lỗi payload
unsigned long lostPackets = 0;  // Gói mất (SEQ nhảy)

int currentCR = 7;
unsigned long startTime = 0;
unsigned long lastReceiveTime = 0;
float bitrate = 0;
unsigned long packetCount = 0;

String crop(String s, int maxLen) {
  if (s.length() <= maxLen) return s;
  return s.substring(0, maxLen - 3) + "...";
}

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
  LoRa.disableCrc();

  startTime = millis();

  display.clearDisplay();
  display.println("LoRa RX Ready!");
  display.display();
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return;

  float time_s = (millis() - startTime) / 1000.0;

  String raw = "";
  while (LoRa.available()) {
    raw += (char)LoRa.read();
  }

  int rssi = LoRa.packetRssi();
  float snr = LoRa.packetSnr();

  long seq = -1;
  int lastSpace = raw.lastIndexOf(' ');
  if (lastSpace >= 0) {
    seq = raw.substring(lastSpace + 1).toInt();
  }

  bool payloadOK = raw.startsWith("Hello World ");

  if (!payloadOK || seq < 0) {
    errPackets++;
  } else {
    okPackets++;
    if (lastSeq != -1 && seq > lastSeq + 1) {
      lostPackets += (seq - lastSeq - 1);
    }
    lastSeq = seq;
  }

  Serial.printf(
    "RAW=\"%s\" | RSSI=%d | SNR=%.2f | Time=%.2f\n",
    raw.c_str(),
    rssi,
    snr,
    time_s
  );

  display.clearDisplay();
  display.setCursor(0, 0);
  display.printf("t=%.1fs\n", time_s);
  display.println(crop(raw, 21));
  display.println(crop(raw.substring(21), 21));
  display.printf("RSSI=%d\n", rssi);
  display.printf("SNR=%.2f\n", snr);
  display.printf("OK:%lu\n", okPackets);
  display.printf("ERR:%lu\n", errPackets);
  display.printf("LOST:%lu\n", lostPackets);
  display.display();
}
