const int FLAME_PIN = 3;     

void setupFlame() {
  pinMode(FLAME_PIN, INPUT);
}

int readFlame() {
  int flameStatus = digitalRead(FLAME_PIN);
  return flameStatus;
}