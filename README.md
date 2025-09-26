# IoT-FireAlert \_mini_project

🚨 ระบบตรวจจับอุณหภูมิและแก๊ส พร้อมแจ้งเตือนผ่าน Telegram และบันทึกข้อมูลลง Google Sheets

---

## 🔧 อุปกรณ์ที่ใช้

- **ESP32**
- **DHT22** (Temperature & Humidity Sensor)
- **MQ-2** (Gas Sensor)
- **n8n** สำหรับ Automation
- **Telegram Bot** สำหรับแจ้งเตือน
- **Google Sheets** สำหรับบันทึกข้อมูล
- **Library ESP32**
  - DHT sensor library
  - HttpClient
  - WiFiManager

---

## 📌 การเชื่อมต่ออุปกรณ์

คุณสามารถดูการเชื่อมต่อได้ที่ [Wokwi Simulation Link](https://wokwi.com/projects/443150859549464577)

![Wokwi Circuit](https://raw.githubusercontent.com/sharkvortex/IoT-FireAlert/main/images/wokwi.png)  
_ภาพแสดงการเชื่อมต่อวงจร ESP32 กับ DHT22 และ MQ-2_

---

## 🖼 รูปตัวอย่าง Flow n8n

[ดาวน์โหลด Flow n8n](<a href="https://raw.githubusercontent.com/sharkvortex/IoT-FireAlert/main/download/mini_project.json" download>ดาวน์โหลด Flow n8n</a>
)

![n8n Flow Diagram](https://raw.githubusercontent.com/sharkvortex/IoT-FireAlert/main/images/n8n.png)  
_ภาพแสดง Workflow ของ n8n สำหรับ IoT-FireAlert_

---

## ⚙️ การทำงานของโปรเจ็ค

    1. ESP32 อ่านข้อมูลจากเซนเซอร์ **DHT22** และ **MQ-2**
    2. ส่งข้อมูลไปยัง **n8n** ทุก ๆ 30 นาที
    3. หากค่าอุณหภูมิหรือแก๊สเกินเกณฑ์ที่ตั้งไว้ → n8n ส่งแจ้งเตือนผ่าน **Telegram Bot** ทุก 1 นาที
    4. ข้อมูลทั้งหมดจะถูกบันทึกลง **Google Sheets** เพื่อวิเคราะห์ย้อนหลัง

---

![Telegram Alert Example](https://raw.githubusercontent.com/sharkvortex/IoT-FireAlert/main/images/telegram.png)  
_ตัวอย่างการแจ้งเตือนบน Telegram_

---

## 📜 License

MIT License
