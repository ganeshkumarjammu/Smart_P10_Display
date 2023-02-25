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
 
//char Time[ ] = "TIME:00:00:00";
char Time[ ] = "TIME:00:00";
char Date[ ] = "DATE:00/00/2000";
byte last_second, second_, minute_, hour_, day_, month_;
byte prevMin = 0;
int year_;

String prevData = "";

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
String html = "<!DOCTYPE html>";
html+="<html> <head><style>.bg-container{";
html+="background-color:#07152b;";
html+="    height:100%;";
html+="   width:100%;";
html+="}.header-image{";
html+="    height:30%;";
html+="    width:100%;";
html+="    background-size:cover;}";
html+=".text{text-align:center;}";
html+=".header{text-align:center;font-family:'Caveat';font-size:35px;color:#fcfcfc;}";
html+=".header-caption{text-align:center;font-family:'Roboto';font-size:25px;color:#3a7bb5;}";
html+=".button1{";
html+="    height:30px;width:100px;background-color:#e5de00;color:#fcfcfa;font-family:'Roboto';font-size:15px;font-weight:bold;border-width:0px;border-radius:30px 90px 30px 90px;}";
html+=" body {background-color: #07152b; }";
html+="  </style>";
html+="   </head>";
html+="  <body>";
html+="    <div class='bg-container'>";
html+="     <img src=' https://ria.sohamacademy.org/wp-content/uploads/2022/01/RiA_logo-1536x275.png'   ";
html+="     class='header-image'/>";
html+="    <div class='text'>";
html+="     <h1 class='header'>ROBOTICS IN ACADEMICS</h1>";
html+="     <P class='header-caption'>Promoting Innovation in Schools & Communities</P>";
html+="     <button class='button1'>Know more</button>";
html+="     </div>";
html+="     <div class='bg-container text'>";
html+="     <form action='/send' id='usrform' method='post'> ";
html+="  <div class='d-flex flex-column'>";
html+="  <p class='header-caption'>Enter Message:</p> ";
html+="  <p class='header-caption'> Data sent to the Arduino Uno: " + data; 
html+= "</p> <input type='text' name='data'>";
html+="  </div>";
html+="  <input type='submit' value='Submit' ></form><br></div></div></body></html>";
    server.send(200, "text/html", html);
  });
  server.on("/send", []() {
    
    data = server.arg("data");
    UART2.println(String(Time)+"\n"+String(Date)+"\n"+data);
String html = "<!DOCTYPE html>";
html+="<html> <head><style>.bg-container{";
html+="background-color:#07152b;";
html+="    height:100%;";
html+="   width:100%;";
html+="}.header-image{";
html+="    height:30%;";
html+="    width:100%;";
html+="    background-size:cover;}";
html+=".text{text-align:center;}";
html+=".header{text-align:center;font-family:'Caveat';font-size:35px;color:#fcfcfc;}";
html+=".header-caption{text-align:center;font-family:'Roboto';font-size:25px;color:#3a7bb5;}";
html+=".button1{";
html+="    height:30px;width:100px;background-color:#e5de00;color:#fcfcfa;font-family:'Roboto';font-size:15px;font-weight:bold;border-width:0px;border-radius:30px 90px 30px 90px;}";
html+=" body {background-color: #07152b; }";
html+="  </style>";
html+="   </head>";
html+="  <body>";
html+="    <div class='bg-container'>";
html+="     <img src=' https://ria.sohamacademy.org/wp-content/uploads/2022/01/RiA_logo-1536x275.png'   ";
html+="     class='header-image'/>";
html+="    <div class='text'>";
html+="     <h1 class='header'>ROBOTICS IN ACADEMICS</h1>";
html+="     <P class='header-caption'>Promoting Innovation in Schools & Communities</P>";
html+="     <button class='button1'>Know more</button>";
html+="     </div>";
html+="     <div class='bg-container text'>"; 
//html += "Data sent to the Arduino Uno: " + data;
html+="     <form action='/send' id='usrform' method='post'> ";
html+="  <div class='d-flex flex-column'>";
html+="  <p class='header-caption'>Enter Message:</p>";
html+="  <p class='header-caption'> Data sent to the Arduino Uno: " + data; 
html+= "</p> <input type='text' name='data'>";
html+="  </div>";
html+="  <input type='submit' value='Submit' ></form><br></div></div></body></html>";
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
    
//    Time[12] = second_ % 10 + 48;
//    Time[11] = second_ / 10 + 48;
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
  
  if((minute_ != prevMin) || (prevData != data )){
  prevMin = minute_ ;
  prevData = data ;
  UART2.println(String(Time)+"\n"+String(Date)+"\n"+data);
  delay(100);
  }
  
  
  server.handleClient();
}
