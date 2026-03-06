void xuLyNhietDo() {
  // Đọc nhiệt độ từ DHT11 
  float t = dht.readTemperature(); 

  // Kiểm tra nếu cảm biến chưa sẵn sàng hoặc lỗi dây
  if (isnan(t)) {
    return; 
  }
  
  tempVal = round(t); 

  // --- Logic điều khiển quạt và trạng thái  ---
  if (tempVal < 30) {
    statusTemp = 0;
  } 
  else if (tempVal >= 30 && tempVal <= 40) {
    statusTemp = 1;
    reqFan = true; 
  } 
  else if (tempVal > 40) {
    statusTemp = 2;
    reqFan = true;
    reqWindowOpen = true; 
    reqBuzzer = true; 
  }
}