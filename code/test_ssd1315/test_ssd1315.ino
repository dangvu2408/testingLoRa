#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// SSD1315/SSD1306 constructor - dùng I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ESP32 pins
#define OLED_SDA 21
#define OLED_SCL 22

void setup() {
  // Khởi động UART
  Serial.begin(115200);
  Serial.println("SSD1315 Test Start");

  // Khởi động I2C đúng chân
  Wire.begin(OLED_SDA, OLED_SCL);

  // Khởi động màn OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {   // 0x3C or 0x3D
    Serial.println("SSD1315 NOT FOUND!");
    while (1);
  }

  Serial.println("SSD1315 Connected!");
  display.clearDisplay();

  // Hiển thị dòng chữ test
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("Hello!");
  display.display();
  delay(10000);

  // Demo đơn giản
  display.clearDisplay();
  display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
  display.setCursor(20, 28);
  display.setTextSize(1);
  display.println("SSD1315 OK!");
  display.display();
}

void loop() {
  // Nhấp nháy text
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(30, 20);
  display.println("ON");
  display.display();
  delay(500);

  display.clearDisplay();
  display.setCursor(30, 20);
  display.println("OFF");
  display.display();
  delay(500);
}
