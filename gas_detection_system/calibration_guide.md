# 🎯 HƯỚNG DẪN HIỆU CHỈNH CẢM BIẾN MQ2

## 📋 Mục Lục
1. [Tại Sao Cần Hiệu Chỉnh](#tại-sao-cần-hiệu-chỉnh)
2. [Chuẩn Bị](#chuẩn-bị)
3. [Quy Trình Hiệu Chỉnh](#quy-trình-hiệu-chỉnh)
4. [Điều Chỉnh Biến Trở](#điều-chỉnh-biến-trở)
5. [Xác Định Ngưỡng Cảnh Báo](#xác-định-ngưỡng-cảnh-báo)
6. [Kiểm Tra Độ Chính Xác](#kiểm-tra-độ-chính-xác)

---

## 🤔 Tại Sao Cần Hiệu Chỉnh?

### Lý do chính:

1. **Mỗi cảm biến khác nhau:**
   - Độ nhạy không giống nhau
   - Tuổi thọ và lịch sử sử dụng khác nhau

2. **Môi trường khác nhau:**
   - Nhiệt độ, độ ẩm ảnh hưởng
   - Độ cao so với mực nước biển
   - Chất lượng không khí nền

3. **Ứng dụng cụ thể:**
   - Nhà bếp: Nhiều hơi nước, dầu mỡ
   - Garage: Khí xăng, dầu diesel
   - Phòng kín: Ít lưu thông gió

**Không hiệu chỉnh → Cảnh báo sai → Nguy hiểm!**

---

## 🛠️ Chuẩn Bị

### Dụng cụ cần có:

- ✅ Hệ thống đã lắp ráp hoàn chỉnh
- ✅ Tua vít nhỏ (hoặc tô vít gốm)
- ✅ Sổ tay ghi chép
- ✅ Đồng hồ bấm giờ
- ✅ (Tùy chọn) Bật lửa gas để test

### Điều kiện môi trường:

- 🌡️ Nhiệt độ: 20-25°C
- 💧 Độ ẩm: 30-70%
- 🪟 Không gian thoáng, không có khí gas
- ⚠️ Không có người hút thuốc gần

### Thời gian cần:

- **Lần đầu:** 48-72 giờ (làm ấm cảm biến)
- **Hiệu chỉnh:** 1-2 giờ
- **Kiểm tra:** 30 phút

---

## 📊 Quy Trình Hiệu Chỉnh

### Bước 1: Làm Ấm Cảm Biến (Burn-in)

**Lần đầu tiên sử dụng:**

```
Ngày 1:
- Lắp ráp mạch
- Cấp nguồn
- Để hoạt động liên tục 24 giờ
- KHÔNG test với gas

Ngày 2:
- Vẫn để chạy
- Bắt đầu quan sát giá trị ổn định chưa

Ngày 3:
- Giá trị đã ổn định
- Sẵn sàng hiệu chỉnh
```

**Lần sau:**
- Chỉ cần làm ấm 30 phút trước khi dùng

---

### Bước 2: Xác Định Giá Trị Baseline

**Đặt cảm biến trong không khí sạch:**

1. Mở Serial Monitor (115200 baud)
2. Quan sát giá trị trong 10 phút
3. Ghi lại 20 giá trị:

```
Thời gian | Giá trị
----------|--------
0:00      | 245
0:30      | 248
1:00      | 243
1:30      | 250
...       | ...
10:00     | 246
```

4. Tính giá trị trung bình:
```
Baseline = (245 + 248 + 243 + ... + 246) / 20
         = ~246 (ví dụ)
```

5. Tính độ lệch chuẩn:
```
Nếu giá trị dao động ±5: Tốt
Nếu dao động ±10: Chấp nhận được
Nếu dao động ±20: Cảm biến chưa ổn định
```

**Ghi chú quan trọng:**
```
📝 Giá trị Baseline của bạn: _______
📝 Độ dao động: ±_______
📝 Ngày ghi: ___/___/___
```

---

### Bước 3: Test Với Khí Gas

**⚠️ CẢNH BÁO: Làm trong nơi thoáng, có người giám sát!**

**Cách làm an toàn:**

1. **Dùng bật lửa gas:**
   ```
   - Không bật lửa
   - Chỉ xịt gas (nhấn nhẹ)
   - Giữ cách cảm biến ~10cm
   - Xịt 1-2 giây
   - Quan sát giá trị
   ```

2. **Ghi nhận:**
   ```
   - Giá trị tăng lên bao nhiêu?
   - Bao lâu để tăng?
   - Bao lâu để trở về baseline?
   ```

**Ví dụ thực tế:**
```
Trước khi xịt:  246
Sau khi xịt:    687 (tăng 441)
Sau 30 giây:    402
Sau 1 phút:     298
Sau 2 phút:     255 (về gần baseline)
```

---

## 🔧 Điều Chỉnh Biến Trở

### Tìm biến trở trên module MQ2:

```
     Module MQ2
┌──────────────────┐
│                  │
│   [Cảm biến]     │
│                  │
│  VCC AO DO GND   │
│                  │
│      (o)←-Biến trở (màu xanh hoặc trắng)
└──────────────────┘
```

### Cách điều chỉnh:

**Mục tiêu:** Đặt ngưỡng để output DO bật/tắt

1. **Trong không khí sạch:**
   - Vặn biến trở ngược chiều kim đồng hồ (giảm nhạy)
   - Đến khi LED trên module MQ2 TẮT

2. **Xịt gas gần cảm biến:**
   - LED trên module phải BẬT
   - Nếu không bật: Vặn chiều kim đồng hồ (tăng nhạy)

3. **Điều chỉnh tinh:**
   - Vặn từ từ cho đến khi:
     - Không khí sạch: LED tắt
     - Có gas: LED bật
   - Đây là điểm ngưỡng tối ưu

**Lưu ý:**
- Module MQ2 có sẵn chân DO (Digital Output) với LED báo hiệu
- Nhưng project này dùng chân AO (Analog Output) linh hoạt hơn
- Biến trở chỉ ảnh hưởng đến DO, không ảnh hưởng nhiều đến AO

---

## 🎯 Xác Định Ngưỡng Cảnh Báo

### Công thức tính ngưỡng:

```cpp
// Baseline = giá trị trong không khí sạch (ví dụ: 246)

THRESHOLD_SAFE    = Baseline + 50    // 296
THRESHOLD_WARNING = Baseline + 200   // 446  
THRESHOLD_DANGER  = Baseline + 400   // 646
```

### Tùy chỉnh theo môi trường:

**Môi trường nhạy cảm (bệnh viện, trường học):**
```cpp
THRESHOLD_SAFE    = Baseline + 30
THRESHOLD_WARNING = Baseline + 100
THRESHOLD_DANGER  = Baseline + 250
```

**Môi trường công nghiệp (xưởng, garage):**
```cpp
THRESHOLD_SAFE    = Baseline + 100
THRESHOLD_WARNING = Baseline + 300
THRESHOLD_DANGER  = Baseline + 600
```

**Nhà bếp (có hơi nước, nhiệt):**
```cpp
THRESHOLD_SAFE    = Baseline + 80
THRESHOLD_WARNING = Baseline + 250
THRESHOLD_DANGER  = Baseline + 500
```

---

## 📝 Bảng Ghi Chép Hiệu Chỉnh

**Copy và điền vào:**

```
╔═══════════════════════════════════════════════════════════╗
║           BẢNG HIỆU CHỈNH CẢM BIẾN MQ2                   ║
╠═══════════════════════════════════════════════════════════╣
║ Ngày hiệu chỉnh: ___/___/___                             ║
║ Thời gian: ___:___                                        ║
║ Nhiệt độ phòng: _____°C                                   ║
║ Độ ẩm: _____%                                             ║
╠═══════════════════════════════════════════════════════════╣
║ GIÁTRỊ BASELINE (không khí sạch)                         ║
║ ┌───────────────────────────────────────┐                ║
║ │ Giá trị đo được: _______              │                ║
║ │ Độ dao động: ±_______                 │                ║
║ │ Thời gian ổn định: _____ phút         │                ║
║ └───────────────────────────────────────┘                ║
╠═══════════════════════════════════════════════════════════╣
║ NGƯỠNG CẢNH BÁO                                          ║
║ ┌───────────────────────────────────────┐                ║
║ │ THRESHOLD_SAFE:    _______            │                ║
║ │ THRESHOLD_WARNING: _______            │                ║
║ │ THRESHOLD_DANGER:  _______            │                ║
║ └───────────────────────────────────────┘                ║
╠═══════════════════════════════════════════════════════════╣
║ TEST VỚI KHÍ GAS                                         ║
║ ┌───────────────────────────────────────┐                ║
║ │ Giá trị cao nhất đo được: _______     │                ║
║ │ Thời gian phản ứng: _____ giây       │                ║
║ │ Thời gian hồi phục: _____ giây       │                ║
║ └───────────────────────────────────────┘                ║
╠═══════════════════════════════════════════════════════════╣
║ GHI CHÚ                                                   ║
║ ┌───────────────────────────────────────┐                ║
║ │                                        │                ║
║ │                                        │                ║
║ │                                        │                ║
║ └───────────────────────────────────────┘                ║
╚═══════════════════════════════════════════════════════════╝
```

---

## ✅ Kiểm Tra Độ Chính Xác

### Test Case 1: Không Khí Sạch
```
✓ LED xanh sáng
✓ Không có còi
✓ Giá trị ~Baseline
✓ Web hiển thị "AN TOÀN"
```

### Test Case 2: Xịt Gas Nhẹ
```
✓ LED vàng sáng
✓ Còi kêu ngắt quãng
✓ Giá trị tăng lên
✓ Web hiển thị "CẢNH BÁO"
```

### Test Case 3: Xịt Gas Mạnh
```
✓ LED đỏ sáng
✓ Còi kêu liên tục
✓ Giá trị cao
✓ Web hiển thị "NGUY HIỂM"
```

### Test Case 4: Sau Khi Hết Gas
```
✓ Giá trị giảm dần
✓ Quay về trạng thái an toàn
✓ Thời gian hồi phục < 5 phút
```

---

## 🔄 Hiệu Chỉnh Lại Khi Nào?

### Các trường hợp cần hiệu chỉnh lại:

1. **Thời gian:**
   - Mỗi 3-6 tháng (sử dụng thường xuyên)
   - Mỗi năm (sử dụng ít)

2. **Thay đổi môi trường:**
   - Chuyển nơi đặt (nhà bếp → phòng khách)
   - Thay đổi khí hậu (hè → đông)

3. **Độ chính xác giảm:**
   - Cảnh báo sai nhiều lần
   - Không phản ứng với gas
   - Giá trị dao động bất thường

4. **Sau sự cố:**
   - Tiếp xúc khí gas nồng độ cao lâu
   - Rơi hoặc va đập mạnh
   - Ngâm nước

---

## 📈 Nâng Cao: Bù Nhiệt Độ & Độ Ẩm

### Công thức bù (nếu có DHT11/DHT22):

```cpp
// Đọc nhiệt độ và độ ẩm
float temp = dht.readTemperature();
float humidity = dht.readHumidity();

// Đọc giá trị MQ2 thô
int rawValue = analogRead(MQ2_PIN);

// Bù nhiệt độ (giảm 1% mỗi độ C tăng)
float tempFactor = 1.0 - (temp - 25) * 0.01;

// Bù độ ẩm (giảm 0.5% mỗi % độ ẩm tăng)
float humidityFactor = 1.0 - (humidity - 50) * 0.005;

// Giá trị đã bù
int compensatedValue = rawValue * tempFactor * humidityFactor;
```

**Lưu ý:** Công thức này là ước lượng, cần thử nghiệm thực tế.

---

## ⚠️ Lưu Ý An Toàn

### KHI HIỆU CHỈNH:

❌ **KHÔNG:**
- Đốt lửa gần cảm biến
- Test trong không gian kín
- Để một mình khi test với gas
- Test quá nhiều lần liên tiếp

✅ **NÊN:**
- Làm trong nơi thoáng
- Có người giám sát
- Chuẩn bị bình cứu hỏa
- Mở cửa sổ khi test

### Dấu Hiệu Cảm Biến Hỏng:

- ⚠️ Luôn cho giá trị 0 hoặc 1023
- ⚠️ Không phản ứng với gas
- ⚠️ Dao động quá 100 đơn vị/giây
- ⚠️ Cảm biến nóng bất thường (>80°C)
- ⚠️ Có mùi khét hoặc khói

**→ Thay cảm biến mới ngay!**

---

## 📚 Tài Liệu Tham Khảo

- [MQ2 Datasheet](https://www.pololu.com/file/0J309/MQ2.pdf)
- [Gas Sensor Calibration Guide](https://www.instructables.com/How-to-Calibrate-Gas-Sensors/)
- [MQ Series Sensors Tutorial](https://www.sparkfun.com/tutorials/218)

---

**Chúc bạn hiệu chỉnh thành công! 🎯**

*Hãy ghi nhận lại kết quả hiệu chỉnh để lần sau dễ dàng hơn.*
