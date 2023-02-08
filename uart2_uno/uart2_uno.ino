#include <SoftwareSerial.h>

// Create a software serial instance on pins 2 and 3
SoftwareSerial UART2(0, 1);

void setup() {
  // Start the UART2 communication with 9600 baud rate
  UART2.begin(9600);
  Serial.begin(115200);
  // Wait for the UART2 communication to be established
  while (!UART2) {};

  // Print a message to indicate that the communication is ready
  Serial.println("UART2 communication ready");
}

void loop() {
  // Check if there is data available on UART2
  if (UART2.available() > 0) {
    // Read the data from UART2
    String data = UART2.readStringUntil('\n');

    // Print the data on the Serial Monitor
    Serial.println("Data received from ESP32: " + data);
  }
}
