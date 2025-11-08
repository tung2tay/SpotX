# 🔧 HƯỚNG DẪN XỬ LÝ SỰ CỐ

## 📋 Mục Lục
- [Vấn Đề Kết Nối WiFi](#vấn-đề-kết-nối-wifi)
- [Vấn Đề Cảm Biến MQ2](#vấn-đề-cảm-biến-mq2)
- [Vấn Đề LED Và Buzzer](#vấn-đề-led-và-buzzer)
- [Vấn Đề Upload Code](#vấn-đề-upload-code)
- [Vấn Đề Web Dashboard](#vấn-đề-web-dashboard)
- [Vấn Đề Phần Cứng](#vấn-đề-phần-cứng)

---

## 🌐 Vấn Đề Kết Nối WiFi

### ❌ Lỗi: "Không thể kết nối WiFi"

**Triệu chứng:**
```
Đang kết nối WiFi: YourWiFiName
....................
✗ Không thể kết nối WiFi!
```

**Nguyên nhân & Giải pháp:**

#### 1. WiFi 5GHz
- **Vấn đề:** ESP8266 chỉ hỗ trợ WiFi 2.4GHz
- **Giải pháp:** 
  - Kiểm tra router có phát 2.4GHz không
  - Tách riêng SSID cho 2.4GHz và 5GHz
  - Hoặc chuyển sang chế độ dual band

#### 2. Tên WiFi/Mật Khẩu Sai
- **Vấn đề:** Tên hoặc mật khẩu không chính xác
- **Giải pháp:**
  ```cpp
  // Kiểm tra lại trong code:
  const char* ssid = "TEN_WIFI_CHINH_XAC";      // Phân biệt hoa thường
  const char* password = "mat_khau_chinh_xac";   // Không có khoảng trắng thừa
  ```

#### 3. Router Quá Xa
- **Vấn đề:** Tín hiệu WiFi yếu
- **Giải pháp:**
  - Đặt NodeMCU gần router hơn
  - Kiểm tra tín hiệu WiFi trên điện thoại ở vị trí đặt NodeMCU
  - Dùng WiFi extender nếu cần

#### 4. Quá Nhiều Thiết Bị
- **Vấn đề:** Router đã đầy slot DHCP
- **Giải pháp:**
  - Ngắt một số thiết bị khác
  - Restart router
  - Đặt IP tĩnh cho NodeMCU:
  ```cpp
  IPAddress local_IP(192, 168, 1, 100);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(local_IP, gateway, subnet);
  ```

#### 5. Bảo Mật WPA3
- **Vấn đề:** ESP8266 không hỗ trợ WPA3
- **Giải pháp:**
  - Chuyển router sang WPA2-PSK
  - Hoặc tạo WiFi guest với WPA2

---

### ❌ Lỗi: "Mất kết nối WiFi liên tục"

**Giải pháp:**

Thêm code tự động kết nối lại:
```cpp
void loop() {
  // Kiểm tra kết nối WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Mất kết nối WiFi! Đang kết nối lại...");
    WiFi.reconnect();
    delay(5000);
  }
  
  // Code khác...
}
```

---

## 🔬 Vấn Đề Cảm Biến MQ2

### ❌ Lỗi: "Giá trị cảm biến luôn là 0"

**Nguyên nhân & Giải pháp:**

#### 1. Kết Nối Sai Chân
- **Kiểm tra:** Chân AO của MQ2 có nối vào A0 của NodeMCU không?
- **Giải pháp:**
  - Kiểm tra lại sơ đồ kết nối
  - Dùng multimeter đo điện áp trên chân AO (phải có ~1-3V)

#### 2. Cảm Biến Chưa Được Cấp Nguồn
- **Kiểm tra:** LED nguồn trên module MQ2 có sáng không?
- **Giải pháp:**
  - Kiểm tra VCC nối đúng 3.3V hoặc 5V
  - Kiểm tra GND đã nối chung chưa

#### 3. Cảm Biến Hỏng
- **Kiểm tra:** Thử với cảm biến khác
- **Giải pháp:** Thay cảm biến mới

**Code kiểm tra:**
```cpp
void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
}

void loop() {
  int value = analogRead(A0);
  Serial.print("Giá trị A0: ");
  Serial.println(value);
  delay(1000);
}
```

---

### ❌ Lỗi: "Giá trị cảm biến luôn là 1023 (max)"

**Nguyên nhân & Giải pháp:**

#### 1. Chân A0 Bị Hở Mạch
- **Vấn đề:** Dây nối lỏng hoặc không tiếp xúc
- **Giải pháp:**
  - Ấn chặt dây vào breadboard
  - Thay dây nối mới
  - Hàn chân nếu cần (với dự án lâu dài)

#### 2. Module MQ2 Bị Lỗi
- **Vấn đề:** Mạch trên module hỏng
- **Giải pháp:** Thay module mới

---

### ❌ Lỗi: "Giá trị nhảy lung tung"

**Triệu chứng:**
```
Giá trị: 245
Giá trị: 891
Giá trị: 123
Giá trị: 654
```

**Nguyên nhân & Giải pháp:**

#### 1. Nhiễu Điện
- **Giải pháp:** Thêm bộ lọc phần mềm
```cpp
int readGasSensorFiltered() {
  int sum = 0;
  for(int i = 0; i < 10; i++) {
    sum += analogRead(MQ2_PIN);
    delay(10);
  }
  return sum / 10;  // Trung bình 10 lần đọc
}
```

#### 2. Cảm Biến Chưa Ổn Định
- **Vấn đề:** Chưa làm ấm đủ lâu
- **Giải pháp:** 
  - Để cảm biến chạy liên tục 24-48 giờ
  - Sau đó mới đánh giá hiệu suất

#### 3. Nguồn Không Ổn Định
- **Vấn đề:** Điện áp dao động
- **Giải pháp:**
  - Dùng nguồn adapter thay vì USB máy tính
  - Thêm tụ lọc 100uF song song với VCC-GND

---

### ❌ Lỗi: "Cảm biến không phản ứng với khí gas"

**Kiểm tra:**

1. **Làm ấm chưa?**
   - Cảm biến cần ít nhất 30 giây làm ấm mỗi lần bật
   - Lần đầu cần 24-48 giờ

2. **Thử với khí gas thật:**
   - Dùng bật lửa gas (KHÔNG đốt!)
   - Xịt gần cảm biến
   - Giá trị phải tăng lên

3. **Điều chỉnh biến trở:**
   - Module MQ2 có biến trở xanh để điều chỉnh độ nhạy
   - Vặn theo chiều kim đồng hồ: Tăng độ nhạy
   - Vặn ngược chiều: Giảm độ nhạy

**Video hướng dẫn:** Tìm "MQ2 sensitivity adjustment" trên YouTube

---

## 💡 Vấn Đề LED Và Buzzer

### ❌ Lỗi: "LED không sáng"

**Kiểm tra từng bước:**

#### Bước 1: Kiểm tra LED
```
Dùng pin 3V (2 viên AA nối tiếp):
(+) pin → chân dài LED → điện trở 220Ω → (-) pin
```
- LED sáng: LED tốt
- LED không sáng: LED hỏng hoặc nối ngược

#### Bước 2: Kiểm tra GPIO
```cpp
void setup() {
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
}

void loop() {
  digitalWrite(D6, HIGH); delay(500);
  digitalWrite(D6, LOW);  delay(500);
  digitalWrite(D7, HIGH); delay(500);
  digitalWrite(D7, LOW);  delay(500);
  digitalWrite(D8, HIGH); delay(500);
  digitalWrite(D8, LOW);  delay(500);
}
```
Upload code này và xem LED có nhấp nháy không.

#### Bước 3: Kiểm tra kết nối
- Điện trở có nối nối tiếp với LED không?
- Cathode (-) LED có nối GND không?
- Breadboard có tiếp xúc tốt không?

**Nguyên nhân thường gặp:**
- ❌ Quên nối điện trở → LED cháy
- ❌ Nối ngược cực LED → Không sáng
- ❌ Dây bị lỏng → Không có điện

---

### ❌ Lỗi: "Buzzer không kêu"

**Kiểm tra:**

#### 1. Active vs Passive Buzzer

**Active Buzzer (Có tích phân dao động):**
- Có nguồn là kêu luôn
- Kiểm tra: Nối VCC và GND trực tiếp vào 5V → Phải kêu

**Passive Buzzer (Cần tín hiệu PWM):**
- Cần code có tone() mới kêu
- Mặt dưới có mạch in phức tạp

**Phân biệt:**
```
Active:  Có sticker nhựa dán ở mặt trước
Passive: Có thể thấy mạch xanh bên trong
```

#### 2. Code Kiểm Tra

**Cho Active Buzzer:**
```cpp
void setup() {
  pinMode(D5, OUTPUT);
}

void loop() {
  digitalWrite(D5, HIGH);
  delay(1000);
  digitalWrite(D5, LOW);
  delay(1000);
}
```

**Cho Passive Buzzer:**
```cpp
void setup() {
  pinMode(D5, OUTPUT);
}

void loop() {
  tone(D5, 1000);    // 1000Hz
  delay(1000);
  noTone(D5);
  delay(1000);
}
```

#### 3. Nguyên Nhân Khác
- Âm lượng quá nhỏ → Đưi tai sát vào nghe
- Buzzer hỏng → Thử buzzer khác
- Tần số ngoài tai người → Thử các tần số 500Hz - 3000Hz

---

### ❌ Lỗi: "LED sáng rất mờ"

**Nguyên nhân:**
- Điện trở quá lớn (>1kΩ)
- Nguồn yếu
- LED đang hỏng

**Giải pháp:**
- Kiểm tra điện trở là 220Ω (Đỏ-Đỏ-Nâu)
- Dùng nguồn 5V adapter
- Thay LED mới

---

## 📤 Vấn Đề Upload Code

### ❌ Lỗi: "COM port not found"

**Triệu chứng:**
```
Serial port not found
Please select a serial port
```

**Giải pháp:**

#### Windows:
1. Cài driver CH340 hoặc CP2102:
   - [Driver CH340](http://www.wch.cn/download/CH341SER_EXE.html)
   - [Driver CP2102](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
2. Restart máy tính
3. Kiểm tra Device Manager → Ports (COM & LPT)

#### macOS:
```bash
# Cài driver
brew tap mengbo/ch340g-ch34g-ch34x-mac-os-x-driver
brew install ch340g-ch34g-ch34x-mac-os-x-driver

# Restart máy
sudo reboot
```

#### Linux:
```bash
# Thêm user vào group dialout
sudo usermod -a -G dialout $USER
sudo reboot
```

---

### ❌ Lỗi: "espcomm_open failed"

**Triệu chứng:**
```
warning: espcomm_sync failed
error: espcomm_open failed
error: espcomm_upload_mem failed
```

**Giải pháp:**

#### 1. Nhấn nút FLASH khi upload
- Nhấn giữ nút "FLASH" trên NodeMCU
- Nhấn "Upload" trong Arduino IDE
- Khi thấy "Connecting...", giữ tiếp 2 giây rồi thả ra

#### 2. Chọn đúng cấu hình
```
Tools → Board → NodeMCU 1.0 (ESP-12E Module)
Tools → Upload Speed → 115200
Tools → CPU Frequency → 80MHz
Tools → Flash Size → 4MB (FS:2MB OTA:~1019KB)
```

#### 3. Cáp USB kém chất lượng
- Thử cáp USB khác
- Dùng cáp data, không phải cáp sạc
- Cáp ngắn tốt hơn cáp dài

#### 4. Nguồn không đủ
- Cắm vào cổng USB sau máy tính (không qua hub)
- Hoặc dùng adapter 5V 1A

---

### ❌ Lỗi: Compilation Error

**Ví dụ:**
```
'WiFi' was not declared in this scope
```

**Giải pháp:**
- Kiểm tra đã chọn đúng board ESP8266 chưa
- Kiểm tra đã cài thư viện ESP8266 chưa
- Restart Arduino IDE

---

## 🌐 Vấn Đề Web Dashboard

### ❌ Lỗi: "Không mở được trang web"

**Kiểm tra:**

#### 1. Có địa chỉ IP chưa?
- Mở Serial Monitor
- Xem dòng: "Địa chỉ IP: 192.168.x.x"
- Nếu không có → Vấn đề WiFi (xem phần trên)

#### 2. Cùng mạng WiFi không?
- Máy tính/điện thoại phải kết nối cùng WiFi với NodeMCU
- Ping thử:
```bash
ping 192.168.x.x
```
- Nếu không ping được → Kiểm tra firewall

#### 3. Firewall chặn?
- Tắt firewall tạm thời để kiểm tra
- Hoặc thêm exception cho cổng 80

#### 4. Thử trình duyệt khác
- Chrome, Firefox, Safari đều được
- Xóa cache trình duyệt (Ctrl+Shift+Del)
- Thử chế độ ẩn danh (Incognito)

---

### ❌ Lỗi: "Trang web không cập nhật dữ liệu"

**Kiểm tra Console:**
1. Nhấn F12 trong trình duyệt
2. Vào tab "Console"
3. Xem có lỗi JavaScript không

**Lỗi thường gặp:**
```
GET http://192.168.x.x/api/data net::ERR_CONNECTION_REFUSED
```

**Giải pháp:**
- NodeMCU bị restart → Đợi khởi động lại
- Hết RAM → Xem Serial Monitor có lỗi không

---

## ⚙️ Vấn Đề Phần Cứng

### ❌ Lỗi: "NodeMCU nóng bất thường"

**Nguyên nhân:**
- Bị short mạch
- Tiêu thụ dòng quá lớn

**Giải pháp:**
1. **NGẮT NGUỒN NGAY LẬP TỨC**
2. Kiểm tra lại toàn bộ kết nối
3. Dùng multimeter đo:
   - VCC-GND không được ngắn mạch (phải >1kΩ)
4. Nếu chip nóng → Có thể đã hỏng, cần thay

---

### ❌ Lỗi: "NodeMCU không lên đèn"

**Kiểm tra:**
1. Cáp USB có đèn/kết nối không?
2. Thử cổng USB khác
3. Thử nguồn adapter
4. Nếu vẫn không lên → NodeMCU hỏng

---

### ❌ Lỗi: "Breadboard tiếp xúc kém"

**Triệu chứng:**
- Hệ thống hoạt động rồi không hoạt động
- Phải ấn dây mới chạy

**Giải pháp:**
- Dùng breadboard mới chất lượng tốt
- Hàn các kết nối cố định
- Dùng PCB custom cho dự án lâu dài

---

## 📊 Checklist Tổng Hợp

Khi gặp vấn đề, hãy kiểm tra theo thứ tự:

### ✅ Checklist Phần Cứng
```
□ Nguồn 5V đã cắm chưa?
□ Tất cả GND đã nối chung chưa?
□ LED có đúng cực tính không? (chân dài = +)
□ Điện trở đã có cho tất cả LED chưa?
□ Cảm biến MQ2 có đèn nguồn sáng không?
□ Buzzer có đúng cực tính không?
□ Không có chân nào chập (short) không?
□ Dây nối có chặt không?
```

### ✅ Checklist Phần Mềm
```
□ Đã chọn đúng Board (NodeMCU 1.0)?
□ Đã chọn đúng COM Port?
□ Đã cài thư viện ESP8266?
□ Đã cài thư viện ArduinoJson?
□ Đã sửa tên WiFi và mật khẩu?
□ Serial Monitor đặt 115200 baud?
□ Code compile thành công?
□ Upload không có lỗi?
```

### ✅ Checklist Kết Nối
```
□ WiFi là 2.4GHz?
□ NodeMCU kết nối WiFi thành công?
□ Có địa chỉ IP?
□ Máy tính cùng mạng WiFi?
□ Ping được NodeMCU?
□ Mở được trang web?
```

---

## 🆘 Khi Nào Cần Hỏi Trợ Giúp

Nếu đã thử tất cả cách trên mà vẫn không được, hãy chuẩn bị thông tin sau khi hỏi:

### Thông Tin Cần Có:
1. **Triệu chứng cụ thể:** "LED không sáng" chứ không phải "không chạy"
2. **Serial Monitor output:** Copy toàn bộ output
3. **Code đã sửa:** Đặc biệt phần WiFi
4. **Ảnh kết nối:** Chụp rõ breadboard và dây nối
5. **Đã thử gì:** Liệt kê các cách đã thử
6. **Linh kiện:** NodeMCU V2 hay V3? MQ2 loại nào?

### Nơi Hỏi:
- 📧 GitHub Issues của dự án
- 💬 Nhóm Facebook Arduino/IoT Việt Nam
- 🎓 Hỏi giảng viên/bạn học
- 📱 Diễn đàn Arduino.vn

---

## 📚 Tài Liệu Tham Khảo Thêm

- [ESP8266 Debugging Guide](https://arduino-esp8266.readthedocs.io/en/latest/faq/readme.html)
- [MQ2 Calibration Tutorial](https://www.instructables.com/MQ2-Gas-Sensor-Tutorial/)
- [NodeMCU Pinout Reference](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/)

---

**Chúc bạn sớm khắc phục được sự cố! 💪**

*Nếu bạn đã giải quyết được vấn đề bằng cách khác, hãy chia sẻ để giúp người khác!*
