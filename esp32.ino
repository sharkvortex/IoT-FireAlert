#include <WiFi.h>
#include <WiFiManager.h>   
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 23            // GPIO ที่ต่อ DHT22
#define DHTTYPE DHT22

#define GAS_ANALOG_PIN 34   // GPIO ที่ต่อ AOUT ของ MQ-2

// เกณฑ์ตรวจจับ
int threshold_GAS = 1000;   // ค่าแก๊สที่จะส่งแจ้งเตือน
int threshold_TEMP = 50;    // อุณหภูมิที่จะส่งแจ้งเตือน

// URL สำหรับส่งข้อมูล Webhook n8n
const char* SERVER_URL = "http://example.com/webhook";
const char* NOTIFY_URL = "http://example.com/webhook-notify";

// Bearer Token สำหรับการแจ้งเตือน
const char* BEARER_TOKEN = "YOUR_BEARER_TOKEN_HERE";

DHT dht(DHTPIN, DHTTYPE);

// ตัวแปรเวลา
unsigned long lastSendData = 0;
unsigned long lastSendNotify = 0;
const unsigned long intervalData = 30 * 60 * 1000; // 30 นาที
const unsigned long intervalNotify = 1 * 60 * 1000; // 1 นาที

void setup() {
  Serial.begin(115200);

  // เริ่ม DHT22
  dht.begin();

  // ใช้ WiFiManager เพื่อเชื่อมต่อ Wi-Fi อัตโนมัติ
  WiFiManager wifiManager;
  wifiManager.autoConnect("ESP32_AutoConnect");

  Serial.println("WiFi connected.");
  Serial.println(WiFi.localIP());
}

void loop() {
  float temperature = dht.readTemperature();   // อ่านอุณหภูมิ
  float humidity = dht.readHumidity();         // อ่านความชื้น
  int gasAnalog = analogRead(GAS_ANALOG_PIN); // อ่านค่า AO จาก MQ-2

  // ตรวจสอบเซนเซอร์อ่านค่าผิดพลาด
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading from DHT sensor!");
    delay(2000);
    return;
  }

  // แสดงผลบน Serial Monitor
  Serial.print("Temperature: "); Serial.print(temperature); Serial.print(" °C, ");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.print(" %, ");
  Serial.print("Gas Analog: "); Serial.println(gasAnalog);

  unsigned long currentMillis = millis();

  // --- ส่งข้อมูลทุก ๆ 30 นาที ---
  if (currentMillis - lastSendData >= intervalData) {
    sendData(SERVER_URL, temperature, humidity, gasAnalog, "");
    lastSendData = currentMillis;
  }

  // --- ถ้าเข้าเกณฑ์ ส่งแจ้งเตือนทุก ๆ 1 นาที ---
  if ((temperature > threshold_TEMP || gasAnalog > threshold_GAS) &&
      (currentMillis - lastSendNotify >= intervalNotify)) {
    sendData(NOTIFY_URL, temperature, humidity, gasAnalog, BEARER_TOKEN);
    lastSendNotify = currentMillis;
  }

  delay(1000); 
}

/*
 * ฟังก์ชันส่งข้อมูลไป server
 * url: URL ปลายทาง
 * temp, hum, gas: ค่าที่จะส่ง
 * token: Bearer Token สำหรับ header ถ้าไม่มีใส่ ""
 */
void sendData(const char* url, float temp, float hum, int gas, const char* token) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    // ถ้ามี Bearer Token ให้เพิ่ม header
    if (strlen(token) > 0) {
      String bearer = "Bearer ";
      bearer += token;
      http.addHeader("Authorization", bearer);
    }

    // สร้าง JSON body
    String body = "{";
    body += "\"temperature\":" + String(temp, 2) + ",";
    body += "\"humidity\":" + String(hum, 2) + ",";
    body += "\"gas\":" + String(gas);
    body += "}";

    int httpResponseCode = http.POST(body);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println("Response: " + http.getString());
    } else {
      Serial.print("Error sending POST: ");
      Serial.println(http.errorToString(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}
