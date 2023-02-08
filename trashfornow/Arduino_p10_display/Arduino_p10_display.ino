#include <SPI.h>
#include <DMD2.h>
#include <fonts/SystemFont5x7.h>
#include <fonts/Arial14.h>

// Set Width to the number of displays wide you have
const int WIDTH = 1;

const uint8_t *FONT = Arial14;
//const uint8_t *FONT = SystemFont5x7;
//String msg = "hello Ganesh" ;
//char *MESSAGE = "Welcome to Robotics Lab";

SoftDMD dmd(WIDTH,1);  // DMD controls the entire display
DMD_TextBox box(dmd);  // "box" provides a text box to automatically write to/scroll the display

void setup() {
  Serial.begin(9600);
  dmd.setBrightness(255);
  dmd.selectFont(FONT);
  dmd.begin();
}

void loop() {
  if (Serial.available() > 0) {
  String message = Serial.readString();
  Serial.println("Received message: " + message);
  //MESSAGE = message .c_str();
  //const char *next = MESSAGE;
  char *next = message.c_str();
  while(*next) {
    Serial.print(*next);
    box.print(*next);
    delay(200);
    next++;
   }
   delay(500);
  }
}
