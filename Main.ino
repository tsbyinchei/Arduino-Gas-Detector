#include <Servo.h>
#include <DHT.h>

// --- KHAI BÁO CHÂN CẮM ---
#define GAS_PIN A0
#define DHTPIN 2 
#define FLAME_PIN 3
#define BUZZER_PIN 4
#define SERVO_WINDOW_PIN 5
#define SERVO_DOOR_PIN 6
#define RELAY_FAN_PIN 7   
#define DHTTYPE DHT11

// --- KHỞI TẠO ĐỐI TƯỢNG ---
Servo sWindow, sDoor;
DHT dht(DHTPIN, DHTTYPE); 

// --- BIẾN TOÀN CỤC ---
int gasVal = 0;
int flameVal = 1; 
int tempVal = 0; 
int statusGas = 0;
int statusFlame = 0; 
int statusTemp = 0;  

bool reqWindowOpen = false;
bool reqDoorOpen = false;
bool reqBuzzer = false;
bool reqFan = false;

void setup() {
  Serial.begin(9600);
  dht.begin(); 
  
  pinMode(FLAME_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_FAN_PIN, OUTPUT);
  
  sWindow.attach(SERVO_WINDOW_PIN);
  sDoor.attach(SERVO_DOOR_PIN);
  
  sWindow.write(0);
  sDoor.write(0);
  digitalWrite(RELAY_FAN_PIN, LOW); 
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  // 1. Reset toàn bộ cờ yêu cầu phần cứng mỗi vòng lặp
  reqWindowOpen = false;
  reqDoorOpen = false;
  reqBuzzer = false;
  reqFan = false;

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
  if(reqWindowOpen) sWindow.write(90);
  else sWindow.write(0);

  // Servo Cửa chính
  if(reqDoorOpen) sDoor.write(90);
  else sDoor.write(0);

  // Quạt tản nhiệt
  if(reqFan) digitalWrite(RELAY_FAN_PIN, HIGH); // Bật
  else digitalWrite(RELAY_FAN_PIN, LOW);      // Tắt 

  // Còi báo động
  if(reqBuzzer) digitalWrite(BUZZER_PIN, HIGH);
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