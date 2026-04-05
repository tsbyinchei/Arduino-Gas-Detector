#include <Servo.h>
#include <DHT.h>

// --- KHAI BÁO CHÂN CẮM ---
#define GAS_PIN A0
#define DHTPIN 2 
#define FLAME_PIN 3
#define BUZZER_PIN 4
#define SERVO_PIN_1 9
#define SERVO_PIN_2 6
#define RELAY_FAN_PIN 7   
#define DHTTYPE DHT11
 
// --- KHỞI TẠO ĐỐI TƯỢNG ---
Servo s1, s2;
DHT dht(DHTPIN, DHTTYPE); 

// --- BIẾN TOÀN CỤC (Lưu dữ liệu thô) ---
int gasVal = 0;
int flameVal = 1; 
int tempVal = 0;
int flameCount = 0;

// Các cờ trạng thái điều khiển từ LabVIEW
bool reqS1 = false;
bool reqS2 = false;
bool reqBz = false;
bool reqF = false;

// Mỗi servo có thể cần chiều quay khác nhau tùy cơ khí lắp đặt
const int S1_OPEN_ANGLE = 90;
const int S1_CLOSE_ANGLE = 0;
const int S2_OPEN_ANGLE = 0;
const int S2_CLOSE_ANGLE = 90;

void xuLyLenhLabVIEW(char c);
void dieuKhienPhanCung();
void guiDataLabVIEW();

void setup() {
  Serial.begin(9600); 
  dht.begin(); 
  
  pinMode(FLAME_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_FAN_PIN, OUTPUT);
  s1.attach(SERVO_PIN_1, 500, 2400);
  s2.attach(SERVO_PIN_2, 500, 2400);
  
  // Trạng thái ban đầu: Đóng/Tắt
  s1.write(S1_CLOSE_ANGLE);
  s2.write(S2_CLOSE_ANGLE);
  digitalWrite(RELAY_FAN_PIN, LOW); 
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  // 1. ĐỌC DỮ LIỆU THÔ
  gasVal = analogRead(GAS_PIN);
  // flameVal = digitalRead(FLAME_PIN);
  // Đọc dữ liệu thô vào biến tạm
  int rawFlame = digitalRead(FLAME_PIN);
  
  // LỌC NHIỄU: Lửa phải cháy liên tục 3 vòng lặp (khoảng 0.3 giây) mới xác nhận
  if (rawFlame == LOW) {
    flameCount++;
    if (flameCount >= 3) {
      flameVal = 0; // Chốt là có cháy thật, báo lên LabVIEW số 0
      flameCount = 3; // Giữ biến không bị tràn
    }
  } else {
    flameCount = 0; // Tắt bật lửa -> Reset bộ đếm ngay lập tức
    flameVal = 1;   // Báo lên LabVIEW số 1 (An toàn)
  }
  float t = dht.readTemperature();
  if (!isnan(t)) tempVal = round(t);

  // 2. GỬI DỮ LIỆU SANG LABVIEW (Gọi hàm phía dưới)
  guiDataLabVIEW();

  // 3. NHẬN LỆNH TỪ LABVIEW
  while (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == '\n' || cmd == '\r') continue;
    xuLyLenhLabVIEW(cmd); // Chuyền ký tự nhận được vào hàm xử lý
  }

  // 4. THỰC THI PHẦN CỨNG (Gọi hàm phía dưới)
  dieuKhienPhanCung();
  
  delay(100); // Phản hồi 0.1s/lần cho LabVIEW mượt
}

// --- CÁC HÀM BỔ TRỢ (Giúp code loop gọn gàng) ---

void xuLyLenhLabVIEW(char c) {
  // Bộ lệnh: HOA là Bật/Mở, thường là Tắt/Đóng
  switch (c) {
    case 'F': reqF = true; break;   // Quạt ON
    case 'f': reqF = false; break;  // Quạt OFF
    case 'B': reqBz = true; break;  // Còi ON
    case 'b': reqBz = false; break; // Còi OFF
    case 'D':
      reqS1 = true;
      reqS2 = true;
      break;                         // Mở đồng thời 2 cánh
    case 'd':
      reqS1 = false;
      reqS2 = false;
      break;                         // Đóng đồng thời 2 cánh
    default: break;                  // Bỏ qua ký tự không hợp lệ
  }
}

void dieuKhienPhanCung() {
  int s1Target = reqS1 ? S1_OPEN_ANGLE : S1_CLOSE_ANGLE;
  int s2Target = reqS2 ? S2_OPEN_ANGLE : S2_CLOSE_ANGLE;

  // Luôn phát lại lệnh để servo tự đồng bộ nếu bị lệch do nhiễu nguồn/cơ khí
  s1.write(s1Target);
  s2.write(s2Target);

  digitalWrite(RELAY_FAN_PIN, reqF ? HIGH : LOW);
  digitalWrite(BUZZER_PIN, reqBz ? HIGH : LOW);
}

void guiDataLabVIEW() {
  // Gửi chuỗi: Gas,Flame,Temp
  Serial.print(gasVal); Serial.print(",");
  Serial.print(flameVal); Serial.print(",");
  Serial.println(tempVal);
}