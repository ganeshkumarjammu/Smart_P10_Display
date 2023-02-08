#include <HardwareSerial.h>
#include <WiFi.h>
#include <WebServer.h>
#include "NTPClient.h"               
#include <TimeLib.h>   

const char *ssid     = "Gani";
const char *password = "HighHopes";

String data ="";
HardwareSerial UART2(2);
WebServer server(80);

 
WiFiUDP ntpUDP;
 
 
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 19800, 60000);
 
char Time[ ] = "TIME:00:00:00";
char Date[ ] = "DATE:00/00/2000";
byte last_second, second_, minute_, hour_, day_, month_;
byte prevMin = 0;
int year_;


void setup() {
  // Start the UART2 communication with 9600 baud rate
  UART2.begin(9600, SERIAL_8N1, 16, 17);
  
  Serial.begin(115200);
  // Wait for the Serial communication to be established
  while (!UART2) {};

  // Start the Wi-Fi communication
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  timeClient.begin();
  
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
    data = server.arg("data");
    UART2.println(String(Time)+"\n"+String(Date)+"\n"+data);
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
  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();    // Get Unix epoch time from the NTP server
 
  second_ = second(unix_epoch);
  if (last_second != second_) {
 
    minute_ = minute(unix_epoch);
    hour_   = hour(unix_epoch);
    day_    = day(unix_epoch);
    month_  = month(unix_epoch);
    year_   = year(unix_epoch);
    
    Time[12] = second_ % 10 + 48;
    Time[11] = second_ / 10 + 48;
    Time[9]  = minute_ % 10 + 48;
    Time[8]  = minute_ / 10 + 48;
    Time[6]  = hour_   % 10 + 48;
    Time[5]  = hour_   / 10 + 48;
 
    Date[5]  = day_   / 10 + 48;
    Date[6]  = day_   % 10 + 48;
    Date[8]  = month_  / 10 + 48;
    Date[9]  = month_  % 10 + 48;
    Date[13] = (year_   / 10) % 10 + 48;
    Date[14] = year_   % 10 % 10 + 48;
  }
  if(minute_ != prevMin){
  prevMin = minute_ ;
  UART2.println(String(Time)+"\n"+String(Date)+"\n"+data);
  delay(100);
  }
  server.handleClient();
}
