#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// --- KHAI BÁO HÀM TỪ 3 FILE CỦA 3 NGƯỜI ĐỂ MẠCH HIỂU ---
void setupDHT();
float readTemp();
float readHum();

void setupGas();
int readGas();

void setupFlame();
int readFlame();

// --- CHÂN ĐẦU RA ---
const int BUZZER_PIN = 4;    
const int SERVO1_PIN = 5;    // Cửa sổ
const int SERVO2_PIN = 6;    // Cửa chính
const int RELAY_PIN = 7;     // Quạt

// --- CÀI ĐẶT NGƯỠNG KÍCH HOẠT ---
const int GAS_THRESHOLD = 400;          
const float TEMP_FIRE_THRESHOLD = 50.0; 
const float TEMP_HOT_THRESHOLD = 33.0;  

Servo servoCuaSo;
Servo servoCuaChinh;
LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup() {
  Serial.begin(9600);
  
  // Gọi hàm khởi động cảm biến của cả 3 người
  setupDHT();
  setupGas();
  setupFlame();
  
  // Khởi tạo đầu ra
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  servoCuaSo.attach(SERVO1_PIN);
  servoCuaChinh.attach(SERVO2_PIN);
  
  lcd.init();                      
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Home Init.");
  
  // Trạng thái an toàn mặc định
  digitalWrite(BUZZER_PIN, HIGH); // Ép còi tắt
  digitalWrite(RELAY_PIN, LOW);  
  servoCuaSo.write(0);           
  servoCuaChinh.write(0);        
  
  delay(4000); // Đợi 4 giây cho cảm biến ổn định
  lcd.clear();
}

void loop() {
  // LẤY DỮ LIỆU TỪ HÀM CỦA 3 THÀNH VIÊN
  float temp = readTemp();
  float hum = readHum();
  int gasValue = readGas();
  int flameStatus = readFlame();

  // --- HIỂN THỊ DÒNG 1 ---
  lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print(temp, 1); lcd.print("C "); 
  lcd.print("H:"); lcd.print(hum, 1); lcd.print("%  ");  

  // --- XỬ LÝ LOGIC DÒNG 2 ---
  lcd.setCursor(0, 1); 
  
  // CẤP 1: BÁO CHÁY / RÒ GAS
  if (gasValue > GAS_THRESHOLD || flameStatus == LOW || temp >= TEMP_FIRE_THRESHOLD) {
    digitalWrite(BUZZER_PIN, LOW);  // KÊU CÒI
    digitalWrite(RELAY_PIN, HIGH);  // Bật quạt
    servoCuaSo.write(180);          // Mở cửa sổ
    servoCuaChinh.write(180);       // Mở cửa chính
    lcd.print("!!! NGUY HIEM!!!");
  } 
  // CẤP 2: THÔNG GIÓ
  else if (temp >= TEMP_HOT_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH); // TẮT CÒI
    digitalWrite(RELAY_PIN, HIGH);  // Bật quạt
    servoCuaSo.write(180);          // Mở cửa sổ
    servoCuaChinh.write(0);         // Đóng cửa chính
    lcd.print("Che do Thong Gio");
  } 
  // CẤP 3: AN TOÀN
  else {
    digitalWrite(BUZZER_PIN, HIGH); // TẮT CÒI
    digitalWrite(RELAY_PIN, LOW);   // Tắt quạt
    servoCuaSo.write(0);            // Đóng cửa sổ
    servoCuaChinh.write(0);         // Đóng cửa chính
    lcd.print("He Thong An Toan");
  }
  
  delay(1000); 
}