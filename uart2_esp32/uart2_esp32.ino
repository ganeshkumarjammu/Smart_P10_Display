#include <HardwareSerial.h>
#include <WiFi.h>
#include <WebServer.h>

HardwareSerial UART2(2);
WebServer server(80);

void setup() {
  // Start the UART2 communication with 9600 baud rate
  UART2.begin(9600, SERIAL_8N1, 16, 17);
  Serial.begin(115200);
  // Wait for the Serial communication to be established
  while (!UART2) {};

  // Start the Wi-Fi communication
  WiFi.begin("Gani", "HighHopes");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
  
  // Get the IP address of the ESP32
  IPAddress ip = WiFi.localIP();
  Serial.println("IP address: " + ip.toString());

  // Start the Web Server
  server.on("/", []() {
    String html = "<html><body>";
    html += "<form action='/send' method='post'>";
    html += "<input type='text' name='data'><br>";
    html += "<input type='submit' value='Submit'><br>";
    html += "</form></body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/send", []() {
    String data = server.arg("data");
    UART2.println(data);
    String html = "<html><body>";
    html += "Data sent to the Arduino Uno: " + data;
     html += "<form action='/send' method='post'>";
    html += "<input type='text' name='data'><br>";
    html += "<input type='submit' value='Submit'><br>";
    html += "</form></body></html>";  
    server.send(200, "text/html", html);
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
