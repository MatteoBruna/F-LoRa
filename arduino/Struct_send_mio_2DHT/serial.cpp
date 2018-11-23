extern "C" {
    void serial_init(int baud) {
      Serial.begin(baud);
    }
}
