# 🔥 HỆ THỐNG PHÁT HIỆN VÀ CẢNH BÁO KHÍ GAS

## 📋 Giới Thiệu Dự Án

Đây là dự án hệ thống phát hiện và cảnh báo khí gas thông minh sử dụng **NodeMCU (ESP8266)** và **cảm biến khí gas MQ2**. Hệ thống có khả năng:

- ✅ Phát hiện nồng độ khí gas trong không khí
- 🚨 Cảnh báo qua còi buzzer và đèn LED với 3 mức độ
- 🌐 Hiển thị dữ liệu theo thời gian thực qua Web Dashboard
- 📊 Theo dõi lịch sử nồng độ khí gas
- 📱 Truy cập từ xa qua WiFi

---

## 🛠️ Linh Kiện Cần Thiết

### Phần Cứng

| STT | Linh Kiện | Số Lượng | Ghi Chú |
|-----|-----------|----------|---------|
| 1 | NodeMCU ESP8266 | 1 | Vi điều khiển chính |
| 2 | Cảm biến khí gas MQ2 | 1 | Phát hiện LPG, propane, methane, khói |
| 3 | Còi Buzzer | 1 | Còi báo động |
| 4 | LED Đỏ (5mm) | 1 | Báo nguy hiểm |
| 5 | LED Vàng (5mm) | 1 | Báo cảnh báo |
| 6 | LED Xanh (5mm) | 1 | Báo an toàn |
| 7 | Điện trở 220Ω | 3 | Hạn chế dòng cho LED |
| 8 | Breadboard | 1 | Bảng test mạch |
| 9 | Dây nối Dupont | 1 bộ | Kết nối các linh kiện |
| 10 | Nguồn 5V (USB) | 1 | Cấp nguồn cho NodeMCU |

### Phần Mềm

- **Arduino IDE** (phiên bản 1.8.x hoặc mới hơn)
- **Thư viện ESP8266** cho Arduino IDE
- **Thư viện ArduinoJson** (phiên bản 6.x)

---

## 📐 Sơ Đồ Kết Nối

### Kết Nối Chi Tiết

```
NODEMCU          MQ2 SENSOR
---------        ----------
3.3V      <-->   VCC
GND       <-->   GND
A0        <-->   AO (Analog Output)

NODEMCU          BUZZER
---------        ------
D5        <-->   (+) Dương
GND       <-->   (-) Âm

NODEMCU          LED ĐỎ           LED VÀNG         LED XANH
---------        -------          ---------        ---------
D6        <-->   Anode   <-->     (220Ω)   <-->   GND
D8        <-->   Anode   <-->     (220Ω)   <-->   GND
D7        <-->   Anode   <-->     (220Ω)   <-->   GND
```

### Sơ Đồ Hình Vẽ

```
                     ┌─────────────┐
                     │   NodeMCU   │
                     │   ESP8266   │
                     └──────┬──────┘
                            │
        ┌───────────────────┼───────────────────┐
        │                   │                   │
        │                   │                   │
   ┌────▼────┐         ┌────▼────┐         ┌───▼────┐
   │   MQ2   │         │ BUZZER  │         │  LEDs  │
   │ Sensor  │         │         │         │ R/Y/G  │
   └─────────┘         └─────────┘         └────────┘
```

---

## ⚙️ Hướng Dẫn Cài Đặt

### Bước 1: Cài Đặt Arduino IDE

1. Tải Arduino IDE từ: https://www.arduino.cc/en/software
2. Cài đặt và khởi động Arduino IDE

### Bước 2: Cài Đặt Board ESP8266

1. Mở **Arduino IDE**
2. Vào **File → Preferences**
3. Trong mục **Additional Board Manager URLs**, thêm:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
4. Vào **Tools → Board → Boards Manager**
5. Tìm kiếm "**esp8266**" và cài đặt
6. Chọn board: **Tools → Board → ESP8266 Boards → NodeMCU 1.0 (ESP-12E Module)**

### Bước 3: Cài Đặt Thư Viện

1. Vào **Sketch → Include Library → Manage Libraries**
2. Tìm và cài đặt:
   - **ArduinoJson** (by Benoit Blanchon) - phiên bản 6.x

### Bước 4: Nạp Code

1. Kết nối NodeMCU với máy tính qua cáp USB
2. Mở file `gas_detection_system.ino`
3. **QUAN TRỌNG**: Sửa thông tin WiFi trong code:
   ```cpp
   const char* ssid = "TEN_WIFI_CUA_BAN";          // Thay tên WiFi
   const char* password = "MAT_KHAU_WIFI";         // Thay mật khẩu
   ```
4. Chọn cổng COM: **Tools → Port → COMx** (trên Windows) hoặc **/dev/ttyUSBx** (trên Linux)
5. Nhấn nút **Upload** (mũi tên sang phải)
6. Đợi quá trình nạp code hoàn tất

### Bước 5: Lắp Ráp Phần Cứng

1. Kết nối các linh kiện theo sơ đồ ở trên
2. **Lưu ý**: Cảm biến MQ2 cần làm ấm khoảng 24-48 giờ để hoạt động chính xác
3. Đảm bảo tất cả các kết nối chắc chắn

### Bước 6: Kiểm Tra Hoạt Động

1. Mở **Serial Monitor**: **Tools → Serial Monitor**
2. Chọn baud rate: **115200**
3. Quan sát:
   - Kết nối WiFi thành công
   - Địa chỉ IP của thiết bị
   - Giá trị khí gas đang đọc được

---

## 🚀 Sử Dụng Hệ Thống

### Truy Cập Web Dashboard

1. Sau khi hệ thống khởi động, kiểm tra Serial Monitor để lấy địa chỉ IP
2. Mở trình duyệt web (Chrome, Firefox, Safari, ...)
3. Nhập địa chỉ IP, ví dụ: `http://192.168.1.100`
4. Bạn sẽ thấy giao diện dashboard hiển thị:
   - Nồng độ khí gas hiện tại
   - Trạng thái an toàn
   - Biểu đồ theo dõi
   - Thông tin hệ thống

### Các Mức Cảnh Báo

| Mức Độ | Giá Trị | LED | Còi | Mô Tả |
|--------|---------|-----|-----|-------|
| 🟢 **AN TOÀN** | < 300 | Xanh | Tắt | Không khí bình thường |
| 🟡 **CẢNH BÁO** | 300 - 500 | Vàng | Kêu ngắt quãng | Phát hiện khí gas nhẹ |
| 🟠 **MỨC CAO** | 500 - 700 | Vàng + Đỏ | Kêu nhanh | Nồng độ khí gas cao |
| 🔴 **NGUY HIỂM** | > 700 | Đỏ | Kêu liên tục | Nguy hiểm! Cần sơ tán |

### Tùy Chỉnh Ngưỡng

Bạn có thể thay đổi ngưỡng cảnh báo trong code:

```cpp
const int THRESHOLD_SAFE = 300;        // Ngưỡng an toàn
const int THRESHOLD_WARNING = 500;      // Ngưỡng cảnh báo
const int THRESHOLD_DANGER = 700;       // Ngưỡng nguy hiểm
```

**Lưu ý**: Các giá trị trên là giá trị mặc định. Bạn nên hiệu chỉnh dựa trên môi trường thực tế và đặc tính cảm biến.

---

## 📊 Tính Năng

### Đã Hoàn Thành ✅

- [x] Đọc dữ liệu từ cảm biến MQ2
- [x] Cảnh báo 3 mức độ (An toàn, Cảnh báo, Nguy hiểm)
- [x] Còi buzzer với âm thanh khác nhau cho mỗi mức
- [x] LED hiển thị trạng thái (Xanh, Vàng, Đỏ)
- [x] Web Server hiển thị dashboard
- [x] Giao diện web responsive, đẹp mắt
- [x] Biểu đồ theo dõi theo thời gian thực
- [x] API JSON để lấy dữ liệu
- [x] Lưu lịch sử 50 điểm dữ liệu

### Có Thể Mở Rộng 🔮

- [ ] Gửi thông báo qua Telegram Bot
- [ ] Gửi email cảnh báo
- [ ] Kết nối IFTTT để điều khiển thiết bị khác
- [ ] Lưu dữ liệu lên cloud (Firebase, ThingSpeak)
- [ ] Thêm cảm biến nhiệt độ, độ ẩm DHT11/DHT22
- [ ] Chế độ tiết kiệm pin (Deep Sleep)
- [ ] Ứng dụng mobile (Android/iOS)
- [ ] Tích hợp học máy để dự đoán rò rỉ khí gas

---

## 🔧 Xử Lý Sự Cố

### Vấn Đề: Không Kết Nối Được WiFi

**Giải pháp:**
- Kiểm tra tên WiFi và mật khẩu đã đúng chưa
- Đảm bảo WiFi là 2.4GHz (ESP8266 không hỗ trợ 5GHz)
- Kiểm tra router WiFi có bật không
- Thử reset NodeMCU và nạp lại code

### Vấn Đề: Giá Trị Cảm Biến Không Ổn Định

**Giải pháp:**
- Cảm biến MQ2 cần làm ấm ít nhất 24-48 giờ để ổn định
- Kiểm tra nguồn cấp có đủ 5V không
- Kiểm tra kết nối dây từ A0 đến cảm biến
- Thử hiệu chuẩn lại cảm biến trong môi trường sạch

### Vấn Đề: LED/Buzzer Không Hoạt Động

**Giải pháp:**
- Kiểm tra cực tính LED (chân dài là Anode +)
- Kiểm tra điện trở đã kết nối đúng chưa
- Thử thay LED/buzzer khác để kiểm tra
- Dùng multimeter kiểm tra điện áp trên chân GPIO

### Vấn Đề: Không Mở Được Web Dashboard

**Giải pháp:**
- Kiểm tra Serial Monitor xem có địa chỉ IP không
- Đảm bảo máy tính/điện thoại cùng mạng WiFi với NodeMCU
- Thử ping địa chỉ IP từ command line
- Xóa cache trình duyệt hoặc thử trình duyệt khác

---

## 📚 Tài Liệu Tham Khảo

### Datasheet & Documentation

- [NodeMCU ESP8266 Documentation](https://nodemcu.readthedocs.io/)
- [MQ2 Gas Sensor Datasheet](https://www.pololu.com/file/0J309/MQ2.pdf)
- [ESP8266 Arduino Core Documentation](https://arduino-esp8266.readthedocs.io/)
- [ArduinoJson Documentation](https://arduinojson.org/)

### Video Hướng Dẫn

- YouTube: "NodeMCU ESP8266 Tutorial"
- YouTube: "MQ2 Gas Sensor with Arduino"
- YouTube: "ESP8266 Web Server Tutorial"

---

## 🧪 Kiểm Tra và Hiệu Chỉnh

### Kiểm Tra Cảm Biến MQ2

1. **Trong không khí sạch:**
   - Giá trị nên ổn định trong khoảng 100-300
   - LED xanh sáng, không có còi

2. **Với khí gas:**
   - Dùng bật lửa gas (không đốt) đưa gần cảm biến
   - Giá trị nên tăng lên rõ rệt
   - LED và còi kích hoạt theo mức độ

### Hiệu Chỉnh Ngưỡng

1. Để cảm biến trong môi trường sạch 30 phút
2. Ghi nhận giá trị trung bình (baseline)
3. Đặt ngưỡng an toàn = baseline + 100
4. Đặt ngưỡng cảnh báo = baseline + 300
5. Đặt ngưỡng nguy hiểm = baseline + 500

---

## 📝 Ghi Chú Quan Trọng

### An Toàn

- ⚠️ **KHÔNG** thử nghiệm với lửa thật gần cảm biến
- ⚠️ Cảm biến MQ2 nóng khi hoạt động - cẩn thận khi chạm vào
- ⚠️ Đây là dự án học tập, KHÔNG nên sử dụng cho mục đích an toàn thực tế
- ⚠️ Cần có hệ thống báo cháy chuyên nghiệp cho an toàn thực sự

### Bảo Quản

- 🔋 Không để cảm biến hoạt động liên tục quá 24 giờ trong giai đoạn đầu
- 🌡️ Không để cảm biến ở nơi quá nóng (>50°C) hoặc quá lạnh (<0°C)
- 💧 Tránh để cảm biến tiếp xúc với nước
- 🧹 Vệ sinh cảm biến định kỳ bằng khí nén

---

## 👨‍💻 Thông Tin Tác Giả

**Dự án môn học:** Hệ thống nhúng / IoT  
**Mục đích:** Học tập và nghiên cứu  
**Ngày tạo:** 2025  
**Công nghệ:** NodeMCU ESP8266, Arduino, IoT  

---

## 📄 Giấy Phép

Dự án này được phát hành dưới giấy phép MIT License - xem file LICENSE để biết thêm chi tiết.

Bạn có thể tự do:
- ✅ Sử dụng cho mục đích học tập
- ✅ Chỉnh sửa và phát triển thêm
- ✅ Chia sẻ với người khác

---

## 🤝 Đóng Góp

Nếu bạn có ý tưởng cải thiện dự án:

1. Fork repository này
2. Tạo branch mới (`git checkout -b feature/TinhNangMoi`)
3. Commit thay đổi (`git commit -m 'Thêm tính năng mới'`)
4. Push lên branch (`git push origin feature/TinhNangMoi`)
5. Tạo Pull Request

---

## ❓ Câu Hỏi Thường Gặp (FAQ)

### Q: Cảm biến MQ2 có thể phát hiện những loại khí nào?

**A:** MQ2 có thể phát hiện:
- LPG (Liquefied Petroleum Gas)
- Propane
- Methane (khí thiên nhiên)
- Butane
- Khí hydro
- Khói

### Q: Độ chính xác của hệ thống như thế nào?

**A:** Đây là dự án học tập với độ chính xác tương đối. Để có hệ thống chính xác cao cần:
- Hiệu chỉnh cảm biến với khí gas chuẩn
- Bù nhiệt độ và độ ẩm
- Sử dụng cảm biến chất lượng tốt hơn

### Q: Có thể sử dụng cảm biến khác thay MQ2 không?

**A:** Có, bạn có thể sử dụng:
- MQ5 (nhạy hơn với LPG và khí thiên nhiên)
- MQ7 (phát hiện CO)
- MQ135 (phát hiện nhiều loại khí)

Chỉ cần thay đổi ngưỡng cảnh báo phù hợp.

### Q: Làm sao để truy cập từ xa qua Internet?

**A:** Bạn có thể:
1. Cấu hình Port Forwarding trên router
2. Sử dụng dịch vụ DDNS (No-IP, DuckDNS)
3. Hoặc dùng ngrok để tạo tunnel
4. Tốt nhất là tích hợp với cloud platform (Firebase, AWS IoT)

---

## 📞 Liên Hệ & Hỗ Trợ

Nếu bạn gặp vấn đề hoặc có câu hỏi:

- 📧 Email: [your-email@example.com]
- 💬 GitHub Issues: Tạo issue mới trên repository
- 📱 Facebook Group: [Link group học tập IoT]

---

## 🎓 Kết Luận

Chúc bạn thành công với dự án! Hệ thống này là nền tảng tốt để học về:
- 📡 IoT và ESP8266
- 🔌 Kết nối cảm biến
- 🌐 Web Server và API
- 🎨 Thiết kế giao diện web
- ⚡ Hệ thống cảnh báo thời gian thực

Hãy tiếp tục phát triển và mở rộng dự án của bạn! 🚀

---

**Cập nhật lần cuối:** Tháng 11, 2025  
**Phiên bản:** 1.0.0
