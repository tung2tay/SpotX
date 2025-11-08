/*
 * CHƯƠNG TRÌNH KIỂM TRA LINH KIỆN
 * Dùng để test từng linh kiện riêng lẻ
 */

// ===== CẤU HÌNH CHÂN =====
#define MQ2_PIN A0
#define BUZZER_PIN D5
#define LED_RED D6
#define LED_GREEN D7
#define LED_YELLOW D8

// ===== CHỌN CHẾ ĐỘ TEST =====
// Comment/Uncomment dòng muốn test
#define TEST_LED           // Test LED
// #define TEST_BUZZER     // Test Buzzer
// #define TEST_MQ2        // Test cảm biến MQ2
// #define TEST_ALL        // Test tất cả

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n=== CHƯƠNG TRÌNH KIỂM TRA LINH KIỆN ===\n");
  
  // Khởi tạo chân
  pinMode(MQ2_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  
  // Tắt hết
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
}

void loop() {
  #ifdef TEST_LED
    testLED();
  #endif
  
  #ifdef TEST_BUZZER
    testBuzzer();
  #endif
  
  #ifdef TEST_MQ2
    testMQ2();
  #endif
  
  #ifdef TEST_ALL
    testAll();
  #endif
}

// ===== TEST LED =====
void testLED() {
  Serial.println("Test LED Đỏ...");
  digitalWrite(LED_RED, HIGH);
  delay(1000);
  digitalWrite(LED_RED, LOW);
  delay(500);
  
  Serial.println("Test LED Vàng...");
  digitalWrite(LED_YELLOW, HIGH);
  delay(1000);
  digitalWrite(LED_YELLOW, LOW);
  delay(500);
  
  Serial.println("Test LED Xanh...");
  digitalWrite(LED_GREEN, HIGH);
  delay(1000);
  digitalWrite(LED_GREEN, LOW);
  delay(500);
  
  Serial.println("Sáng tất cả LED...");
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_YELLOW, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  delay(1000);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
  delay(1000);
  
  Serial.println("---\n");
}

// ===== TEST BUZZER =====
void testBuzzer() {
  Serial.println("Test Buzzer - Âm thấp (500Hz)");
  tone(BUZZER_PIN, 500);
  delay(500);
  noTone(BUZZER_PIN);
  delay(300);
  
  Serial.println("Test Buzzer - Âm trung (1000Hz)");
  tone(BUZZER_PIN, 1000);
  delay(500);
  noTone(BUZZER_PIN);
  delay(300);
  
  Serial.println("Test Buzzer - Âm cao (2000Hz)");
  tone(BUZZER_PIN, 2000);
  delay(500);
  noTone(BUZZER_PIN);
  delay(300);
  
  Serial.println("Test Buzzer - Còi dài");
  tone(BUZZER_PIN, 1500);
  delay(2000);
  noTone(BUZZER_PIN);
  delay(1000);
  
  Serial.println("---\n");
}

// ===== TEST MQ2 =====
void testMQ2() {
  int value = analogRead(MQ2_PIN);
  
  Serial.print("Giá trị MQ2: ");
  Serial.print(value);
  Serial.print(" | Điện áp: ");
  Serial.print(value * 3.3 / 1024.0);
  Serial.println("V");
  
  // Đánh giá
  if (value == 0) {
    Serial.println("⚠ CẢNH BÁO: Giá trị = 0 (Kiểm tra kết nối!)");
  } else if (value >= 1020) {
    Serial.println("⚠ CẢNH BÁO: Giá trị = Max (Kiểm tra kết nối!)");
  } else if (value < 100) {
    Serial.println("✓ Giá trị thấp - Có thể cảm biến chưa ổn định");
  } else {
    Serial.println("✓ Giá trị bình thường");
  }
  
  delay(1000);
  Serial.println();
}

// ===== TEST TẤT CẢ =====
void testAll() {
  Serial.println("=== TEST TOÀN BỘ HỆ THỐNG ===\n");
  
  // 1. Test LED
  Serial.println("1. Kiểm tra LED...");
  digitalWrite(LED_RED, HIGH);
  delay(300);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, HIGH);
  delay(300);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, HIGH);
  delay(300);
  digitalWrite(LED_GREEN, LOW);
  Serial.println("✓ LED hoàn tất\n");
  
  // 2. Test Buzzer
  Serial.println("2. Kiểm tra Buzzer...");
  tone(BUZZER_PIN, 1000, 200);
  delay(500);
  Serial.println("✓ Buzzer hoàn tất\n");
  
  // 3. Test MQ2
  Serial.println("3. Kiểm tra MQ2...");
  int value = analogRead(MQ2_PIN);
  Serial.print("   Giá trị: ");
  Serial.println(value);
  if (value > 0 && value < 1020) {
    Serial.println("✓ MQ2 hoạt động\n");
  } else {
    Serial.println("✗ MQ2 có vấn đề!\n");
  }
  
  // 4. Mô phỏng cảnh báo
  Serial.println("4. Mô phỏng cảnh báo...");
  
  Serial.println("   - Trạng thái AN TOÀN");
  digitalWrite(LED_GREEN, HIGH);
  delay(1000);
  digitalWrite(LED_GREEN, LOW);
  
  Serial.println("   - Trạng thái CẢNH BÁO");
  digitalWrite(LED_YELLOW, HIGH);
  tone(BUZZER_PIN, 1000, 200);
  delay(1000);
  digitalWrite(LED_YELLOW, LOW);
  
  Serial.println("   - Trạng thái NGUY HIỂM");
  digitalWrite(LED_RED, HIGH);
  tone(BUZZER_PIN, 2000);
  delay(1000);
  noTone(BUZZER_PIN);
  digitalWrite(LED_RED, LOW);
  
  Serial.println("✓ Mô phỏng hoàn tất\n");
  
  Serial.println("=== KẾT THÚC TEST ===\n");
  delay(3000);
}
