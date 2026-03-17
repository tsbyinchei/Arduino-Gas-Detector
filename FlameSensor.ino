int flameCount = 0; // Biến đếm số lần phát hiện lửa

void xuLyLua() {
  flameVal = digitalRead(FLAME_PIN);
  
  if (flameVal == LOW) { // Mức LOW = Có lửa
    flameCount++;
    
    // Phải phát hiện có lửa 3 lần liên tiếp (chống nhiễu) mới báo động
    if (flameCount >= 3) { 
      statusFlame = 1; // Chuyển trạng thái sang báo cháy
      
      reqS1 = true; // Mở hết cửa thoát hiểm
      reqS2 = true;
      reqBz = true;     // Hú còi
      reqF = true; 
      flameCount = 3; // Giữ biến đếm không bị tràn số
    }
  } 
  else {
    // Nếu hết lửa hoặc chỉ là tín hiệu xẹt qua (nhiễu) -> Reset
    flameCount = 0; 
    statusFlame = 0; 
  }
}