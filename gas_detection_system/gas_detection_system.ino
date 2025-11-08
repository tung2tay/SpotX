/*
 * HỆ THỐNG PHÁT HIỆN VÀ CẢNH BÁO KHÍ GAS
 * Sử dụng: NodeMCU (ESP8266) + Cảm biến MQ2
 * 
 * Tính năng:
 * - Đọc giá trị khí gas từ cảm biến MQ2
 * - Cảnh báo qua còi buzzer và LED
 * - Hiển thị dữ liệu qua Web Server
 * - Gửi cảnh báo qua WiFi
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// ===== CẤU HÌNH WIFI =====
const char* ssid = "TEN_WIFI_CUA_BAN";          // Thay tên WiFi của bạn
const char* password = "MAT_KHAU_WIFI";         // Thay mật khẩu WiFi

// ===== CẤU HÌNH CHÂN KẾT NỐI =====
#define MQ2_PIN A0          // Chân analog đọc cảm biến MQ2
#define BUZZER_PIN D5       // Chân điều khiển còi buzzer
#define LED_RED D6          // LED đỏ - cảnh báo nguy hiểm
#define LED_GREEN D7        // LED xanh - trạng thái bình thường
#define LED_YELLOW D8       // LED vàng - cảnh báo trung bình

// ===== NGƯỠNG CẢNH BÁO =====
const int THRESHOLD_SAFE = 300;        // Dưới 300: An toàn
const int THRESHOLD_WARNING = 500;      // 300-500: Cảnh báo
const int THRESHOLD_DANGER = 700;       // Trên 700: Nguy hiểm

// ===== BIẾN TOÀN CỤC =====
ESP8266WebServer server(80);
int gasValue = 0;
String alertLevel = "SAFE";
unsigned long lastReadTime = 0;
const unsigned long READ_INTERVAL = 1000;  // Đọc mỗi 1 giây

// ===== LỊCH SỬ DỮ LIỆU =====
const int HISTORY_SIZE = 50;
int gasHistory[HISTORY_SIZE];
int historyIndex = 0;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  // Khởi tạo chân GPIO
  pinMode(MQ2_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  
  // Tắt tất cả đèn và còi
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);  // Sáng đèn xanh khi khởi động
  digitalWrite(LED_YELLOW, LOW);
  
  // Khởi tạo mảng lịch sử
  for(int i = 0; i < HISTORY_SIZE; i++) {
    gasHistory[i] = 0;
  }
  
  // Kết nối WiFi
  Serial.println("\n=== HỆ THỐNG PHÁT HIỆN KHÍ GAS ===");
  Serial.print("Đang kết nối WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ Kết nối WiFi thành công!");
    Serial.print("Địa chỉ IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("Truy cập web dashboard tại: http://" + WiFi.localIP().toString());
  } else {
    Serial.println("\n✗ Không thể kết nối WiFi!");
    Serial.println("Hệ thống vẫn hoạt động ở chế độ offline");
  }
  
  // Cấu hình Web Server
  setupWebServer();
  server.begin();
  Serial.println("✓ Web Server đã khởi động");
  
  // Làm ấm cảm biến MQ2 (cần khoảng 30 giây)
  Serial.println("\n⚠ Đang làm ấm cảm biến MQ2...");
  Serial.println("Vui lòng đợi 30 giây...");
  
  for(int i = 30; i > 0; i--) {
    Serial.print(i);
    Serial.print("... ");
    if(i % 10 == 0) Serial.println();
    delay(1000);
  }
  
  Serial.println("\n✓ Cảm biến đã sẵn sàng!");
  Serial.println("=================================\n");
  
  // Hiệu ứng LED sẵn sàng
  blinkAllLEDs(3);
}

void loop() {
  server.handleClient();
  
  // Đọc cảm biến theo khoảng thời gian
  if (millis() - lastReadTime >= READ_INTERVAL) {
    lastReadTime = millis();
    readGasSensor();
    checkGasLevel();
    
    // Lưu vào lịch sử
    gasHistory[historyIndex] = gasValue;
    historyIndex = (historyIndex + 1) % HISTORY_SIZE;
  }
}

// ===== ĐỌC CÁM BIẾN MQ2 =====
void readGasSensor() {
  // Đọc giá trị analog (0-1023)
  gasValue = analogRead(MQ2_PIN);
  
  // Hiển thị lên Serial Monitor
  Serial.print("Nồng độ khí gas: ");
  Serial.print(gasValue);
  Serial.print(" | Trạng thái: ");
  Serial.println(alertLevel);
}

// ===== KIỂM TRA MỨC ĐỘ KHÍ GAS =====
void checkGasLevel() {
  if (gasValue < THRESHOLD_SAFE) {
    // AN TOÀN
    alertLevel = "SAFE";
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    
  } else if (gasValue < THRESHOLD_WARNING) {
    // CẢNH BÁO TRUNG BÌNH
    alertLevel = "WARNING";
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
    
    // Còi kêu ngắt quãng
    tone(BUZZER_PIN, 1000, 200);
    
  } else if (gasValue < THRESHOLD_DANGER) {
    // CẢNH BÁO CAO
    alertLevel = "HIGH";
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, HIGH);
    
    // Còi kêu nhanh hơn
    tone(BUZZER_PIN, 1500, 300);
    
  } else {
    // NGUY HIỂM
    alertLevel = "DANGER";
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
    
    // Còi kêu liên tục
    tone(BUZZER_PIN, 2000);
  }
}

// ===== HIỆU ỨNG LED =====
void blinkAllLEDs(int times) {
  for(int i = 0; i < times; i++) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    delay(200);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
    delay(200);
  }
}

// ===== CẤU HÌNH WEB SERVER =====
void setupWebServer() {
  // Trang chủ - Dashboard
  server.on("/", HTTP_GET, handleRoot);
  
  // API lấy dữ liệu JSON
  server.on("/api/data", HTTP_GET, handleAPIData);
  
  // API lấy lịch sử
  server.on("/api/history", HTTP_GET, handleAPIHistory);
  
  // API cấu hình ngưỡng
  server.on("/api/config", HTTP_POST, handleAPIConfig);
  
  // Trang không tìm thấy
  server.onNotFound(handleNotFound);
}

// ===== XỬ LÝ TRANG CHỦ =====
void handleRoot() {
  String html = getHTMLPage();
  server.send(200, "text/html", html);
}

// ===== API DỮ LIỆU =====
void handleAPIData() {
  StaticJsonDocument<200> doc;
  
  doc["gasValue"] = gasValue;
  doc["alertLevel"] = alertLevel;
  doc["timestamp"] = millis();
  doc["thresholds"]["safe"] = THRESHOLD_SAFE;
  doc["thresholds"]["warning"] = THRESHOLD_WARNING;
  doc["thresholds"]["danger"] = THRESHOLD_DANGER;
  
  String json;
  serializeJson(doc, json);
  
  server.send(200, "application/json", json);
}

// ===== API LỊCH SỬ =====
void handleAPIHistory() {
  StaticJsonDocument<1024> doc;
  JsonArray data = doc.createNestedArray("data");
  
  for(int i = 0; i < HISTORY_SIZE; i++) {
    int index = (historyIndex + i) % HISTORY_SIZE;
    data.add(gasHistory[index]);
  }
  
  String json;
  serializeJson(doc, json);
  
  server.send(200, "application/json", json);
}

// ===== API CẤU HÌNH =====
void handleAPIConfig() {
  // Xử lý cập nhật cấu hình từ web
  server.send(200, "application/json", "{\"status\":\"ok\"}");
}

// ===== TRANG KHÔNG TÌM THẤY =====
void handleNotFound() {
  server.send(404, "text/plain", "404: Không tìm thấy trang");
}

// ===== TẠO TRANG HTML =====
String getHTMLPage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Hệ Thống Giám Sát Khí Gas</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        
        .container {
            max-width: 1200px;
            margin: 0 auto;
        }
        
        .header {
            text-align: center;
            color: white;
            margin-bottom: 30px;
        }
        
        .header h1 {
            font-size: 2.5em;
            margin-bottom: 10px;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
        }
        
        .header p {
            font-size: 1.2em;
            opacity: 0.9;
        }
        
        .dashboard {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        
        .card {
            background: white;
            border-radius: 15px;
            padding: 25px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.3);
            transition: transform 0.3s;
        }
        
        .card:hover {
            transform: translateY(-5px);
        }
        
        .card h2 {
            color: #333;
            margin-bottom: 15px;
            font-size: 1.5em;
        }
        
        .gas-value {
            font-size: 3em;
            font-weight: bold;
            text-align: center;
            margin: 20px 0;
            color: #667eea;
        }
        
        .status {
            text-align: center;
            padding: 15px;
            border-radius: 10px;
            font-size: 1.3em;
            font-weight: bold;
            margin-top: 15px;
        }
        
        .status-safe {
            background: #10b981;
            color: white;
        }
        
        .status-warning {
            background: #f59e0b;
            color: white;
        }
        
        .status-high {
            background: #ef4444;
            color: white;
            animation: pulse 1s infinite;
        }
        
        .status-danger {
            background: #dc2626;
            color: white;
            animation: pulse 0.5s infinite;
        }
        
        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.5; }
        }
        
        .info-grid {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 15px;
            margin-top: 20px;
        }
        
        .info-item {
            background: #f3f4f6;
            padding: 15px;
            border-radius: 8px;
        }
        
        .info-item label {
            display: block;
            color: #6b7280;
            font-size: 0.9em;
            margin-bottom: 5px;
        }
        
        .info-item value {
            display: block;
            color: #1f2937;
            font-size: 1.3em;
            font-weight: bold;
        }
        
        #chart {
            width: 100%;
            height: 300px;
            background: #f3f4f6;
            border-radius: 10px;
            padding: 20px;
            position: relative;
            overflow: hidden;
        }
        
        .chart-line {
            stroke: #667eea;
            stroke-width: 3;
            fill: none;
        }
        
        .chart-area {
            fill: url(#gradient);
            opacity: 0.3;
        }
        
        .footer {
            text-align: center;
            color: white;
            margin-top: 30px;
            opacity: 0.8;
        }
        
        .loading {
            text-align: center;
            color: #667eea;
            font-size: 1.2em;
            padding: 20px;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🔥 HỆ THỐNG GIÁM SÁT KHÍ GAS</h1>
            <p>Phát hiện và cảnh báo khí gas thông minh</p>
        </div>
        
        <div class="dashboard">
            <div class="card">
                <h2>📊 Nồng Độ Khí Gas</h2>
                <div class="gas-value" id="gasValue">---</div>
                <div class="status status-safe" id="status">Đang tải...</div>
            </div>
            
            <div class="card">
                <h2>⚙️ Thông Tin Hệ Thống</h2>
                <div class="info-grid">
                    <div class="info-item">
                        <label>Ngưỡng An Toàn</label>
                        <value id="thresholdSafe">---</value>
                    </div>
                    <div class="info-item">
                        <label>Ngưỡng Cảnh Báo</label>
                        <value id="thresholdWarning">---</value>
                    </div>
                    <div class="info-item">
                        <label>Ngưỡng Nguy Hiểm</label>
                        <value id="thresholdDanger">---</value>
                    </div>
                    <div class="info-item">
                        <label>Thời Gian Cập Nhật</label>
                        <value id="updateTime">---</value>
                    </div>
                </div>
            </div>
        </div>
        
        <div class="card">
            <h2>📈 Biểu Đồ Theo Dõi</h2>
            <div id="chart">
                <svg width="100%" height="100%" id="chartSvg">
                    <defs>
                        <linearGradient id="gradient" x1="0%" y1="0%" x2="0%" y2="100%">
                            <stop offset="0%" style="stop-color:#667eea;stop-opacity:1" />
                            <stop offset="100%" style="stop-color:#667eea;stop-opacity:0" />
                        </linearGradient>
                    </defs>
                </svg>
            </div>
        </div>
        
        <div class="footer">
            <p>NodeMCU + MQ2 Gas Sensor | Dự án môn học IoT</p>
        </div>
    </div>
    
    <script>
        let historyData = [];
        
        // Cập nhật dữ liệu
        function updateData() {
            fetch('/api/data')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('gasValue').textContent = data.gasValue;
                    document.getElementById('thresholdSafe').textContent = data.thresholds.safe;
                    document.getElementById('thresholdWarning').textContent = data.thresholds.warning;
                    document.getElementById('thresholdDanger').textContent = data.thresholds.danger;
                    
                    const now = new Date();
                    document.getElementById('updateTime').textContent = 
                        now.getHours() + ':' + 
                        String(now.getMinutes()).padStart(2, '0') + ':' + 
                        String(now.getSeconds()).padStart(2, '0');
                    
                    updateStatus(data.alertLevel);
                })
                .catch(error => console.error('Lỗi:', error));
        }
        
        // Cập nhật trạng thái
        function updateStatus(level) {
            const statusDiv = document.getElementById('status');
            statusDiv.className = 'status';
            
            switch(level) {
                case 'SAFE':
                    statusDiv.classList.add('status-safe');
                    statusDiv.textContent = '✅ AN TOÀN';
                    break;
                case 'WARNING':
                    statusDiv.classList.add('status-warning');
                    statusDiv.textContent = '⚠️ CẢNH BÁO';
                    break;
                case 'HIGH':
                    statusDiv.classList.add('status-high');
                    statusDiv.textContent = '🔶 MỨC CAO';
                    break;
                case 'DANGER':
                    statusDiv.classList.add('status-danger');
                    statusDiv.textContent = '🚨 NGUY HIỂM';
                    break;
            }
        }
        
        // Cập nhật biểu đồ
        function updateChart() {
            fetch('/api/history')
                .then(response => response.json())
                .then(data => {
                    historyData = data.data;
                    drawChart();
                })
                .catch(error => console.error('Lỗi:', error));
        }
        
        // Vẽ biểu đồ
        function drawChart() {
            const svg = document.getElementById('chartSvg');
            const width = svg.clientWidth;
            const height = svg.clientHeight;
            const padding = 40;
            
            // Xóa nội dung cũ
            while (svg.childNodes.length > 1) {
                svg.removeChild(svg.lastChild);
            }
            
            if (historyData.length === 0) return;
            
            // Tính toán
            const maxValue = Math.max(...historyData, 1000);
            const xStep = (width - 2 * padding) / (historyData.length - 1);
            const yScale = (height - 2 * padding) / maxValue;
            
            // Vẽ đường
            let pathData = '';
            historyData.forEach((value, index) => {
                const x = padding + index * xStep;
                const y = height - padding - value * yScale;
                pathData += (index === 0 ? 'M' : 'L') + x + ',' + y + ' ';
            });
            
            const path = document.createElementNS('http://www.w3.org/2000/svg', 'path');
            path.setAttribute('d', pathData);
            path.setAttribute('class', 'chart-line');
            svg.appendChild(path);
        }
        
        // Khởi động
        updateData();
        updateChart();
        
        // Cập nhật định kỳ
        setInterval(updateData, 1000);
        setInterval(updateChart, 5000);
    </script>
</body>
</html>
)rawliteral";
  
  return html;
}
