#include <DHT.h>

const int DHT_PIN = 2;       
DHT dht(DHT_PIN, DHT11);

void setupDHT() {
  dht.begin();
}

float readTemp() {
  float temp = dht.readTemperature();
  // Nếu lỗi không đọc được thì trả về 0
  return isnan(temp) ? 0 : temp; 
}

float readHum() {
  float hum = dht.readHumidity();
  return isnan(hum) ? 0 : hum;
}