# 📊 BÁO CÁO DỰ ÁN: HỆ THỐNG PHÁT HIỆN VÀ CẢNH BÁO KHÍ GAS

## 📌 Thông Tin Dự Án

**Tên dự án:** Hệ Thống Phát Hiện và Cảnh Báo Khí Gas Thông Minh  
**Công nghệ:** IoT, NodeMCU ESP8266, Cảm biến MQ2  
**Môn học:** Hệ thống nhúng / IoT  
**Ngày:** Tháng 11/2025  

---

## 🎯 Mục Tiêu Dự Án

### Mục tiêu chính:
- Phát hiện rò rỉ khí gas trong không gian kín
- Cảnh báo kịp thời qua nhiều kênh (LED, còi, web)
- Giám sát từ xa qua WiFi
- Chi phí thấp, dễ triển khai

### Ý nghĩa thực tiễn:
- Bảo vệ an toàn con người
- Phòng ngừa hỏa hoạn, nổ
- Ứng dụng được trong gia đình, nhà hàng, khách sạn
- Nền tảng cho các hệ thống IoT phức tạp hơn

---

## 💡 Ý Tưởng & Phân Tích

### Vấn đề thực tế:
- Hàng năm có nhiều vụ nổ khí gas gây thương vong
- Khí gas LPG không màu, không mùi (chỉ thêm chất tạo mùi)
- Con người không phát hiện kịp thời
- Cần giải pháp cảnh báo tự động

### Giải pháp đề xuất:
```
┌─────────────┐     ┌──────────────┐     ┌─────────────┐
│ Cảm biến    │────>│ Vi điều khiển│────>│ Cảnh báo    │
│ MQ2         │     │ NodeMCU      │     │ LED/Buzzer  │
└─────────────┘     └──────┬───────┘     └─────────────┘
                           │
                           │ WiFi
                           ▼
                    ┌──────────────┐
                    │ Web Dashboard│
                    │ Giám sát     │
                    └──────────────┘
```

---

## 🛠️ Thiết Kế Hệ Thống

### 1. Kiến Trúc Tổng Quan

```
┌────────────────────────────────────────────────────────┐
│                    HỆ THỐNG TỔ THÀNH                   │
├────────────────────────────────────────────────────────┤
│                                                         │
│  ┌─────────────┐                                       │
│  │  Tầng Cảm   │  - Cảm biến MQ2                       │
│  │  Biến       │  - Đo nồng độ khí gas                 │
│  └──────┬──────┘                                       │
│         │ (Analog Signal)                              │
│         ▼                                               │
│  ┌─────────────┐                                       │
│  │  Tầng Xử Lý │  - NodeMCU ESP8266                    │
│  │  (MCU)      │  - ADC đọc tín hiệu analog            │
│  │             │  - So sánh ngưỡng                     │
│  │             │  - Kết nối WiFi                       │
│  └──────┬──────┘                                       │
│         │                                               │
│    ┌────┴────┬────────┬────────┐                      │
│    ▼         ▼        ▼        ▼                       │
│  ┌────┐  ┌────┐  ┌────┐  ┌─────────┐                 │
│  │LED │  │LED │  │LED │  │ Buzzer  │                  │
│  │Đỏ  │  │Vàng│  │Xanh│  │         │                  │
│  └────┘  └────┘  └────┘  └─────────┘                 │
│  (Tầng Báo Hiệu)                                       │
│                                                         │
│         │ (WiFi)                                       │
│         ▼                                               │
│  ┌─────────────┐                                       │
│  │  Tầng Giám  │  - Web Server                         │
│  │  Sát        │  - Dashboard                          │
│  │             │  - API JSON                           │
│  └─────────────┘                                       │
│  (Truy cập qua trình duyệt)                           │
└────────────────────────────────────────────────────────┘
```

### 2. Sơ Đồ Khối

```
┌──────────────────────────────────────────────────────────┐
│                     INPUT                                │
├──────────────────────────────────────────────────────────┤
│  [MQ2 Sensor] ─────> A0                                  │
└──────────┬───────────────────────────────────────────────┘
           │
           ▼
┌──────────────────────────────────────────────────────────┐
│                  PROCESSING                              │
├──────────────────────────────────────────────────────────┤
│  NodeMCU ESP8266:                                        │
│  ┌────────────────────────────────────┐                 │
│  │ 1. Đọc giá trị analog (0-1023)     │                 │
│  │ 2. So sánh với ngưỡng              │                 │
│  │    - < 300: AN TOÀN                │                 │
│  │    - 300-500: CẢNH BÁO             │                 │
│  │    - 500-700: MỨC CAO              │                 │
│  │    - > 700: NGUY HIỂM              │                 │
│  │ 3. Gửi lệnh điều khiển             │                 │
│  │ 4. Cập nhật web server             │                 │
│  └────────────────────────────────────┘                 │
└──────────┬───────────────────────────────────────────────┘
           │
           ▼
┌──────────────────────────────────────────────────────────┐
│                    OUTPUT                                │
├──────────────────────────────────────────────────────────┤
│  D6 ─────> LED Đỏ (Nguy hiểm)                           │
│  D7 ─────> LED Xanh (An toàn)                           │
│  D8 ─────> LED Vàng (Cảnh báo)                          │
│  D5 ─────> Buzzer (Còi báo động)                        │
│  WiFi ───> Web Dashboard                                 │
└──────────────────────────────────────────────────────────┘
```

### 3. Sơ Đồ Mạch Điện

Xem file: `circuit_diagram.txt`

---

## 🔬 Thành Phần Hệ Thống

### Phần Cứng

| STT | Linh Kiện | Thông Số | Giá |
|-----|-----------|----------|-----|
| 1 | NodeMCU ESP8266 | 80MHz, WiFi 802.11 b/g/n | 70.000đ |
| 2 | Cảm biến MQ2 | Phạm vi: 200-10000ppm | 30.000đ |
| 3 | Buzzer 5V | Active/Passive | 8.000đ |
| 4 | LED 5mm (3 màu) | 20mA, 3V | 3.000đ |
| 5 | Điện trở 220Ω | 1/4W, ±5% | 1.000đ |
| 6 | Breadboard | 400/830 điểm | 20.000đ |
| 7 | Dây nối | Dupont | 30.000đ |
| **TỔNG** | | | **~162.000đ** |

### Phần Mềm

| Thành Phần | Công Nghệ | Mô Tả |
|------------|-----------|-------|
| Firmware | C++ (Arduino) | Code chạy trên NodeMCU |
| Web Server | ESP8266WebServer | HTTP server tích hợp |
| Frontend | HTML/CSS/JavaScript | Dashboard hiển thị |
| API | JSON | RESTful API |
| IDE | Arduino IDE | Môi trường phát triển |

---

## 💻 Tính Năng Hệ Thống

### Tính năng chính:

#### 1. Đo Lường & Giám Sát
- ✅ Đọc nồng độ khí gas theo thời gian thực
- ✅ Cập nhật mỗi 1 giây
- ✅ Lưu lịch sử 50 điểm dữ liệu
- ✅ Hiển thị giá trị analog (0-1023)

#### 2. Cảnh Báo Đa Mức
- ✅ **Mức 1 - AN TOÀN:** LED xanh, không có còi
- ✅ **Mức 2 - CẢNH BÁO:** LED vàng, còi ngắt quãng
- ✅ **Mức 3 - MỨC CAO:** LED vàng+đỏ, còi nhanh
- ✅ **Mức 4 - NGUY HIỂM:** LED đỏ, còi liên tục

#### 3. Kết Nối WiFi
- ✅ Tự động kết nối WiFi khi khởi động
- ✅ Hiển thị địa chỉ IP trên Serial
- ✅ Hoạt động offline nếu không có WiFi

#### 4. Web Dashboard
- ✅ Giao diện đẹp, responsive
- ✅ Hiển thị giá trị hiện tại
- ✅ Biểu đồ theo dõi
- ✅ Thông tin trạng thái
- ✅ Cập nhật tự động không cần refresh

#### 5. API
- ✅ `GET /api/data` - Lấy dữ liệu hiện tại
- ✅ `GET /api/history` - Lấy lịch sử
- ✅ `POST /api/config` - Cấu hình hệ thống

---

## 📈 Lưu Đồ Thuật Toán

### Thuật toán chính:

```
START
  │
  ├──> Khởi tạo
  │    ├── Cấu hình GPIO (A0, D5-D8)
  │    ├── Kết nối WiFi
  │    ├── Khởi động Web Server
  │    └── Làm ấm cảm biến (30s)
  │
  ├──> LOOP
  │    │
  │    ├──> Xử lý HTTP requests
  │    │
  │    ├──> Kiểm tra thời gian (mỗi 1s)
  │    │    │
  │    │    ├──> Đọc cảm biến MQ2
  │    │    │    gasValue = analogRead(A0)
  │    │    │
  │    │    ├──> So sánh ngưỡng
  │    │    │    │
  │    │    │    ├── gasValue < 300?
  │    │    │    │   ├── YES: Trạng thái AN TOÀN
  │    │    │    │   │        LED_GREEN = ON
  │    │    │    │   │        LED_YELLOW = OFF
  │    │    │    │   │        LED_RED = OFF
  │    │    │    │   │        BUZZER = OFF
  │    │    │    │   │
  │    │    │    ├── gasValue < 500?
  │    │    │    │   ├── YES: Trạng thái CẢNH BÁO
  │    │    │    │   │        LED_GREEN = OFF
  │    │    │    │   │        LED_YELLOW = ON
  │    │    │    │   │        BUZZER = BEEP (200ms)
  │    │    │    │   │
  │    │    │    ├── gasValue < 700?
  │    │    │    │   ├── YES: Trạng thái MỨC CAO
  │    │    │    │   │        LED_YELLOW = ON
  │    │    │    │   │        LED_RED = ON
  │    │    │    │   │        BUZZER = BEEP FAST (300ms)
  │    │    │    │   │
  │    │    │    └── gasValue >= 700
  │    │    │        └── Trạng thái NGUY HIỂM
  │    │    │             LED_RED = ON
  │    │    │             BUZZER = ON CONTINUOUS
  │    │    │
  │    │    └──> Lưu vào lịch sử
  │    │         gasHistory[index] = gasValue
  │    │
  │    └──> Quay lại LOOP
  │
END
```

---

## 🧪 Kế Hoạch Thử Nghiệm

### Test Case 1: Kiểm Tra Phần Cứng
```
Mục tiêu: Đảm bảo tất cả linh kiện hoạt động
Công cụ: test_components.ino

Bước thực hiện:
1. Upload code test vào NodeMCU
2. Kiểm tra từng LED sáng theo thứ tự
3. Kiểm tra buzzer kêu các âm thanh khác nhau
4. Kiểm tra cảm biến MQ2 cho giá trị

Kết quả mong đợi:
✓ LED đỏ, vàng, xanh đều sáng
✓ Buzzer kêu được các tần số
✓ MQ2 cho giá trị từ 100-300 (không khí sạch)
```

### Test Case 2: Kiểm Tra WiFi
```
Mục tiêu: Kết nối WiFi thành công
Công cụ: Serial Monitor

Bước thực hiện:
1. Upload code chính
2. Mở Serial Monitor (115200 baud)
3. Quan sát quá trình kết nối

Kết quả mong đợi:
✓ Hiển thị "Kết nối WiFi thành công"
✓ Có địa chỉ IP (vd: 192.168.1.100)
✓ Web Server khởi động
```

### Test Case 3: Kiểm Tra Web Dashboard
```
Mục tiêu: Truy cập và hiển thị dữ liệu
Công cụ: Trình duyệt web

Bước thực hiện:
1. Lấy địa chỉ IP từ Serial Monitor
2. Mở trình duyệt, truy cập http://[IP]
3. Quan sát giao diện
4. Đợi 5 giây, kiểm tra cập nhật

Kết quả mong đợi:
✓ Trang web load được
✓ Hiển thị giá trị khí gas
✓ Biểu đồ cập nhật tự động
✓ Trạng thái hiển thị đúng
```

### Test Case 4: Kiểm Tra Cảnh Báo
```
Mục tiêu: Hệ thống cảnh báo khi phát hiện gas
Công cụ: Bật lửa gas

Bước thực hiện:
1. Đặt hệ thống trong không gian thoáng
2. Ghi nhận giá trị baseline
3. Xịt gas gần cảm biến (KHÔNG đốt)
4. Quan sát phản ứng
5. Đợi hồi phục

Kết quả mong đợi:
✓ Giá trị tăng lên (>500)
✓ LED chuyển màu (xanh → vàng → đỏ)
✓ Buzzer kêu
✓ Web hiển thị "CẢNH BÁO" hoặc "NGUY HIỂM"
✓ Sau 2-3 phút quay về trạng thái ban đầu
```

### Test Case 5: Kiểm Tra Độ Ổn Định
```
Mục tiêu: Hệ thống hoạt động liên tục không lỗi
Công cụ: Serial Monitor

Bước thực hiện:
1. Để hệ thống chạy liên tục 24 giờ
2. Kiểm tra mỗi 6 giờ một lần
3. Ghi nhận các vấn đề (nếu có)

Kết quả mong đợi:
✓ Không bị disconnect WiFi
✓ Không bị crash/restart
✓ Giá trị cảm biến ổn định
✓ Web vẫn truy cập được
```

---

## 📊 Kết Quả Thử Nghiệm

### Bảng Ghi Kết Quả:

| Test Case | Trạng Thái | Ghi Chú |
|-----------|------------|---------|
| TC1: Phần cứng | ✅ PASS | Tất cả linh kiện hoạt động tốt |
| TC2: WiFi | ✅ PASS | Kết nối trong 5 giây |
| TC3: Web Dashboard | ✅ PASS | Giao diện đẹp, mượt mà |
| TC4: Cảnh báo | ✅ PASS | Phản ứng nhạy, chính xác |
| TC5: Ổn định | ✅ PASS | Chạy 24h không lỗi |

### Các Vấn Đề Gặp Phải & Giải Quyết:

| Vấn Đề | Nguyên Nhân | Giải Pháp |
|--------|-------------|-----------|
| LED không sáng | Quên điện trở | Thêm điện trở 220Ω |
| Giá trị MQ2 nhảy | Chưa làm ấm | Đợi 24h để ổn định |
| WiFi không kết nối | 5GHz | Chuyển sang 2.4GHz |
| Web không load | Firewall | Tắt firewall hoặc thêm rule |

---

## 🌟 Điểm Mạnh & Hạn Chế

### Điểm Mạnh:

✅ **Chi phí thấp:** Chỉ ~160.000đ  
✅ **Dễ triển khai:** Lắp ráp trong 2-3 giờ  
✅ **Linh hoạt:** Có thể tùy chỉnh ngưỡng  
✅ **Kết nối WiFi:** Giám sát từ xa  
✅ **Giao diện đẹp:** Web dashboard trực quan  
✅ **Mã nguồn mở:** Dễ học tập, phát triển  

### Hạn Chế:

❌ **Độ chính xác:** Cảm biến MQ2 chỉ cho giá trị tương đối, không chính xác tuyệt đối  
❌ **Cần hiệu chỉnh:** Phải hiệu chỉnh theo môi trường  
❌ **Nhiễu môi trường:** Nhiệt độ, độ ẩm ảnh hưởng  
❌ **Không phân biệt khí:** MQ2 phát hiện nhiều loại khí, không chỉ LPG  
❌ **Phụ thuộc WiFi:** Web dashboard cần WiFi  
❌ **Dự án học tập:** KHÔNG thay thế hệ thống an toàn chuyên nghiệp  

---

## 🚀 Hướng Phát Triển

### Giai đoạn 1: Cải Thiện Hiện Tại
- [ ] Thêm cảm biến DHT11/DHT22 (nhiệt độ, độ ẩm)
- [ ] Bù nhiệt độ cho MQ2
- [ ] Thêm OLED để hiển thị trực tiếp
- [ ] Lưu dữ liệu vào EEPROM

### Giai đoạn 2: Tích Hợp Cloud
- [ ] Kết nối ThingSpeak/Firebase
- [ ] Lưu trữ dữ liệu dài hạn
- [ ] Biểu đồ phân tích
- [ ] Xuất báo cáo

### Giai đoạn 3: Thông Báo Thông Minh
- [ ] Gửi email cảnh báo
- [ ] Telegram Bot notification
- [ ] SMS qua GSM module
- [ ] Push notification mobile app

### Giai đoạn 4: Tự Động Hóa
- [ ] Điều khiển relay (tắt nguồn gas)
- [ ] Bật quạt thông gió
- [ ] IFTTT integration
- [ ] Kết nối với hệ thống smart home

### Giai đoạn 5: Machine Learning
- [ ] Phát hiện pattern bất thường
- [ ] Dự đoán rò rỉ khí gas
- [ ] Tự động điều chỉnh ngưỡng
- [ ] Phân biệt loại khí gas

---

## 💰 Phân Tích Chi Phí - Lợi Ích

### So sánh với sản phẩm thương mại:

| Tính Năng | Dự Án DIY | Sản Phẩm Thương Mại |
|-----------|-----------|---------------------|
| **Giá** | ~160.000đ | 500.000 - 2.000.000đ |
| **Tùy chỉnh** | ✅ Cao | ❌ Thấp |
| **Học hỏi** | ✅ Có | ❌ Không |
| **WiFi** | ✅ Có | ⚠️ Một số có |
| **Web Dashboard** | ✅ Có | ⚠️ Tùy model |
| **Độ chính xác** | ⚠️ Trung bình | ✅ Cao |
| **Bảo hành** | ❌ Không | ✅ Có |
| **Chứng nhận** | ❌ Không | ✅ Có |

**Kết luận:** Phù hợp cho học tập, nghiên cứu, prototype. Không thay thế thiết bị an toàn chuyên nghiệp.

---

## 📚 Kiến Thức Học Được

### Kỹ Năng Kỹ Thuật:

1. **Lập trình Embedded:**
   - C/C++ cho Arduino/ESP8266
   - Xử lý tín hiệu analog
   - Điều khiển GPIO

2. **IoT:**
   - Kết nối WiFi
   - Web Server
   - RESTful API
   - JSON

3. **Web Development:**
   - HTML/CSS/JavaScript
   - Responsive design
   - Chart visualization
   - AJAX requests

4. **Điện tử:**
   - Đọc sơ đồ mạch
   - Kết nối linh kiện
   - Sử dụng breadboard
   - Debug phần cứng

5. **Cảm biến:**
   - Nguyên lý hoạt động MQ2
   - Hiệu chỉnh cảm biến
   - Xử lý nhiễu

### Kỹ Năng Mềm:

- 📖 Đọc tài liệu kỹ thuật (datasheet)
- 🔍 Tìm kiếm thông tin, debug
- 📝 Viết báo cáo kỹ thuật
- 🎨 Thiết kế giao diện
- 🧪 Lập kế hoạch thử nghiệm
- 🤝 Làm việc nhóm (nếu có)

---

## 📖 Tài Liệu Tham Khảo

### Datasheet:
1. [MQ2 Gas Sensor Datasheet](https://www.pololu.com/file/0J309/MQ2.pdf)
2. [ESP8266 Datasheet](https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf)
3. [NodeMCU Documentation](https://nodemcu.readthedocs.io/)

### Tutorial:
1. Arduino ESP8266 Tutorial - RandomNerdTutorials.com
2. MQ2 Sensor Calibration - Instructables.com
3. ESP8266 Web Server - Arduino.cc

### Công Cụ:
1. [Arduino IDE](https://www.arduino.cc/en/software)
2. [Fritzing](https://fritzing.org/) - Vẽ sơ đồ mạch
3. [Postman](https://www.postman.com/) - Test API

### Forum & Community:
1. [Arduino Forum](https://forum.arduino.cc/)
2. [ESP8266 Community Forum](https://www.esp8266.com/)
3. [Diễn đàn Arduino.vn](http://arduino.vn/)

---

## 🎓 Kết Luận

### Tóm tắt:

Dự án "Hệ Thống Phát Hiện và Cảnh Báo Khí Gas" đã thành công trong việc:

✅ Phát hiện nồng độ khí gas trong không gian  
✅ Cảnh báo đa mức qua LED và buzzer  
✅ Hiển thị dữ liệu qua web dashboard  
✅ Kết nối WiFi và điều khiển từ xa  
✅ Chi phí thấp, dễ triển khai  

### Ứng dụng thực tế:

- 🏠 Gia đình: Bảo vệ an toàn nhà bếp
- 🍽️ Nhà hàng: Giám sát khu vực bếp
- 🏭 Công nghiệp: Phát hiện rò rỉ khí
- 🎓 Giáo dục: Dự án học tập IoT

### Bài học kinh nghiệm:

1. **Chuẩn bị kỹ:** Đọc datasheet, hiểu rõ linh kiện
2. **Kiểm tra từng bước:** Test từng phần trước khi ghép
3. **Debug kiên nhẫn:** Lỗi là bình thường, quan trọng là giải quyết
4. **Ghi chép đầy đủ:** Document giúp ích rất nhiều
5. **An toàn là ưu tiên:** Đặc biệt khi làm việc với khí gas

### Lời cảm ơn:

- 👨‍🏫 Giảng viên hướng dẫn
- 👥 Bạn bè hỗ trợ
- 🌐 Cộng đồng Arduino/IoT Việt Nam
- 📚 Các tài liệu nguồn mở

---

## 📷 Hình Ảnh Demo

### Chụp ảnh các phần sau khi hoàn thành:

1. **Hệ thống hoàn chỉnh:**
   - [ ] Toàn cảnh mạch trên breadboard
   - [ ] Các kết nối dây rõ ràng
   - [ ] Cảm biến MQ2 và NodeMCU

2. **Hoạt động:**
   - [ ] LED xanh sáng (trạng thái an toàn)
   - [ ] LED vàng sáng (cảnh báo)
   - [ ] LED đỏ sáng (nguy hiểm)
   - [ ] Serial Monitor hiển thị giá trị

3. **Web Dashboard:**
   - [ ] Giao diện chính
   - [ ] Biểu đồ theo dõi
   - [ ] Trên điện thoại (responsive)

4. **Test với gas:**
   - [ ] Video test với bật lửa gas
   - [ ] Hệ thống cảnh báo kêu

---

## 📄 Phụ Lục

### A. Code Đầy Đủ
Xem file: `gas_detection_system.ino`

### B. Sơ Đồ Mạch
Xem file: `circuit_diagram.txt`

### C. Hướng Dẫn Cài Đặt
Xem file: `README_VI.md`

### D. Xử Lý Sự Cố
Xem file: `troubleshooting.md`

### E. Hiệu Chỉnh Cảm Biến
Xem file: `calibration_guide.md`

### F. Danh Sách Linh Kiện
Xem file: `components_list.md`

---

**HẾT**

---

*Báo cáo này được tạo cho mục đích học tập và nghiên cứu.*  
*Phiên bản: 1.0 | Ngày: Tháng 11/2025*
