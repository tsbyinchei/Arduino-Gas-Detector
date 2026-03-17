#include <Servo.h>
#include <DHT.h>

// --- KHAI BÁO CHÂN CẮM ---
#define GAS_PIN A0
#define DHTPIN 2 
#define FLAME_PIN 3
#define BUZZER_PIN 4
#define SERVO_PIN_1 5
#define SERVO_PIN_2 6
#define RELAY_FAN_PIN 7   
#define DHTTYPE DHT11

// --- KHỞI TẠO ĐỐI TƯỢNG ---
Servo s1, s2;
DHT dht(DHTPIN, DHTTYPE); 

// --- BIẾN TOÀN CỤC ---
int gasVal = 0;
int flameVal = 1; 
int tempVal = 0; 
int statusGas = 0;
int statusFlame = 0; 
int statusTemp = 0;  

bool reqS1 = false;
bool reqS2 = false;
bool reqBz = false;
bool reqF = false;

void setup() {
  Serial.begin(9600);
  dht.begin(); 
  
  pinMode(FLAME_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_FAN_PIN, OUTPUT);
  
  s1.attach(SERVO_PIN_1);
  s2.attach(SERVO_PIN_2);
  
  s1.write(0);
  s2.write(0);
  digitalWrite(RELAY_FAN_PIN, LOW); 
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  // 1. Reset toàn bộ cờ yêu cầu phần cứng mỗi vòng lặp
  reqS1 = false;
  reqS2 = false;
  reqBz = false;
  reqF = false;

  // 2. Gọi các hàm xử lý logic từ 3 file cảm biến
  xuLyGas();
  xuLyLua();
  xuLyNhietDo();

  // 3. Thực thi điều khiển phần cứng dựa trên các cờ
  dieuKhienPhanCung();

  // 4. Gửi chuỗi dữ liệu sang LabVIEW
  guiDataLabVIEW();
  
  delay(500); // Lấy mẫu 0.5s / lần
}

void dieuKhienPhanCung() {
  // Servo Cửa sổ
  if(reqS1) s1.write(90);
  else s1.write(0);

  // Servo Cửa chính
  if(reqS2) s2.write(90);
  else s2.write(0);

  // Quạt tản nhiệt
  if(reqF) digitalWrite(RELAY_FAN_PIN, HIGH); // Bật
  else digitalWrite(RELAY_FAN_PIN, LOW);      // Tắt 

  // Còi báo động
  if(reqBz) digitalWrite(BUZZER_PIN, HIGH);
  else digitalWrite(BUZZER_PIN, LOW);
}

void guiDataLabVIEW() {
  // Gửi chuỗi: GasVal,StatusGas,FlameVal,StatusFlame,TempVal,StatusTemp
  Serial.print(gasVal); Serial.print(",");
  Serial.print(statusGas); Serial.print(",");
  Serial.print(flameVal); Serial.print(",");
  Serial.print(statusFlame); Serial.print(",");
  Serial.print(tempVal); Serial.print(",");
  Serial.println(statusTemp);
}