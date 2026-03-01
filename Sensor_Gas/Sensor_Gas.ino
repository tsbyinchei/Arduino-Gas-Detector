const int MQ2_PIN = A0;      

void setupGas() {
  // Chân Analog A0 không bắt buộc phải khai báo pinMode
  // nhưng cứ viết sẵn hàm setup để đồng bộ cấu trúc với các bạn khác
}

int readGas() {
  int gasValue = analogRead(MQ2_PIN);
  return gasValue;
}