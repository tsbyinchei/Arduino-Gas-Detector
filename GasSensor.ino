void xuLyGas() {
  analogRead(GAS_PIN); 
  delay(5); 

  long sumGas = 0;
  int gasSamples = 50; 
  for (int i = 0; i < gasSamples; i++) {
    sumGas += analogRead(GAS_PIN);
    delay(1); // Trễ ngắn để lấy mẫu chính xác
  }
  
  // Cập nhật giá trị trung bình vào biến toàn cục
  gasVal = sumGas / gasSamples;

  // 2. Logic xử lý ngưỡng (Threshold)
  if (gasVal <= 350) {
    // Trạng thái bình thường
    statusGas = 0; 
  } 
  else {
    statusGas = 2; 
    reqS1 = true; 
    reqS2 = true;
    reqBz = true;     
    reqF = true; 
  }
}