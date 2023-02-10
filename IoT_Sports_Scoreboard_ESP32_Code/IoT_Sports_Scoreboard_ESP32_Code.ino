// I2C LCD 
// ThingSpeak thingHTTP api 
//   fetching data from cricbuzz https://www.cricbuzz.com/cricket-match/live-scores

 #include <WiFi.h>                                                      
 #include <HTTPClient.h>
 #include <LiquidCrystal_I2C.h> 
 
 #include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier

LiquidCrystal_I2C lcd(0x27, 16, 2);
  //int button = 26;
  const char* ssid = "SSID";              //WIFI SSID Name                             
  const char* password = "PASSWORD";        //WIFI Password
  const char* host = "api.thingspeak.com";  //We read the data from this host                                   
  const int httpPortRead = 80;          // Do not change                                 
                                                  
  const char* url1 = "/apps/thinghttp/send_request?api_key=A1G411THGUTRFLIC";     //Change this URL //thingHTTP API

  int To_remove;      //There are some irrelevant data on the string and here's how I keep the index 
                      //of those characters 
  String team1,score1,team2,score2,Data_Raw,Data_Raw_1,Data_Raw_2,Data_Raw_3,Data_Raw_4,Data_Raw_5,commentary,over1,score2intermediate,over2intermediate,ovr1intermediate,ovr2intermediate;  //Here I keep the numbers that I got
  
  String ovr1,ovr2;  // imp overs
  int score1num,score2num,wicket1num,wicket2num; // imp scores and wickets
  
  int over1num,over2num;

  WiFiClient client;      //Create a WiFi client and http client                                                     
  HTTPClient http; 

  
  void setup() {
    //pinMode(button,INPUT_PULLUP);
    lcd.begin();
    lcd.setCursor(1, 0);
    lcd.print("IPL Score"); //Message at the beginning
    lcd.setCursor(1, 1);
    lcd.print("Loading...");  //Change if you want
    
    Serial.begin(115200);
    WiFi.disconnect();             //Disconnect and reconnect to the Wifi you set                                                 
    delay(1000);                                                                  
    WiFi.begin(ssid, password);                                                   
    Serial.println("Connected to the WiFi network");   //Display feedback on the serial monitor                                        
    Serial.println(WiFi.localIP());
  }

void loop() {
  
     

   //Reading 1: Reading of api
    if( http.begin(host,httpPortRead,url1))        //Connect to the host and the url                                      
      {
        int httpCode = http.GET();                //Check feedback if there's a response                                                  
        if (httpCode > 0)                                                              
        {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {                
              
              Data_Raw = http.getString();   //Here we store the raw data string
            
              
              Data_Raw_1 = Data_Raw;                                  //  <div class="cb-scr-wll-chvrn cb-lv-scrs-col "> <div class="cb-hmscg-bat-txt cb-ovr-flo ">
                                                                      //<div class="cb-ovr-flo cb-hmscg-tm-nm">MI</div>
                                                                      //<div class="cb-ovr-flo" style="display:inline-block; width:140px">7-1 (2 Ovs)</div>
                                                                      //</div> <div class="cb-hmscg-bwl-txt ">
                                                                      //<div class="cb-ovr-flo cb-hmscg-tm-nm">PBKS</div>
                                                                      //<div class="cb-ovr-flo" style="display:inline-block; width:140px"> 57-1 (9 Ovs)</div>
                                                                      //</div> <div class="cb-text-live">Punjab Kings opt to bowl</div> </div>
              
                                                                                                     
              To_remove = Data_Raw_1.indexOf(">");                                                       
              //Serial.println(To_remove); //45
              Data_Raw_1.remove(0,To_remove+1);                        
              //Serial.println(Data_Raw_1);      // first <> removed
              To_remove = Data_Raw_1.indexOf(">");                                                     
              //Serial.println(To_remove); //42  
              Data_Raw_1.remove(0,To_remove+1);      // second <> removed    
              //Serial.println(Data_Raw_1);
              To_remove = Data_Raw_1.indexOf(">");
              Data_Raw_1.remove(0,To_remove+1);      // third <> removed
              //Serial.println(Data_Raw_1); 
              String Data_Raw_1_score= Data_Raw_1;    
              To_remove = Data_Raw_1.indexOf("<");                                                       
              Data_Raw_1.remove(To_remove,Data_Raw_1.length()); 
              //Serial.println(Data_Raw_1);   // got MI team name                                                   
                                                                                                                                                                                                                           
               team1=Data_Raw_1;  
               ////Serial.print("Team1: ");  
               ////Serial.println(team1);    
              //Serial.print("---------");
              
              Data_Raw_2=Data_Raw_1_score;                      
              //Serial.println(Data_Raw_2);             
               To_remove = Data_Raw_2.indexOf(">"); 
              
              //Serial.println(To_remove); 
              Data_Raw_2.remove(0,To_remove+1); 
              //Serial.println(Data_Raw_2);        
               
              To_remove = Data_Raw_2.indexOf(">");
              Data_Raw_2.remove(0,To_remove+1);    
               //Serial.println(Data_Raw_2);
               Data_Raw_3=Data_Raw_2;
                            
              To_remove = Data_Raw_2.indexOf("<"); 
              Data_Raw_2.remove(To_remove,Data_Raw_2.length());
              //Serial.println(Data_Raw_2);
              over1 = Data_Raw_2;
              score1=Data_Raw_2;
              ////Serial.print("Score1: ");
              ////Serial.println(score1);

              ovr1intermediate = score1;
              To_remove = ovr1intermediate.indexOf("(");
              //Serial.println(To_remove); 
              ovr1intermediate.remove(0,To_remove+1);
              //Serial.println(ovr1intermediate);
              To_remove = ovr1intermediate.indexOf(" ");
              ovr1intermediate.remove(To_remove,ovr1intermediate.length());             
              ovr1 = ovr1intermediate;
              //Serial.println(ovr1);           // got over in string form -->ovr1
              
 
              To_remove = Data_Raw_2.indexOf("-");
              //Serial.println(To_remove);
              Data_Raw_2.remove(To_remove,Data_Raw_2.length());
              //Serial.println(Data_Raw_2);
              score1num = Data_Raw_2.toInt();
              //Serial.println(score1num);   // got score1 in int form == score1num

              To_remove = over1.indexOf("-");
              //Serial.println(To_remove);
              over1.remove(0,To_remove+1);
              //Serial.println(over1);
              To_remove = over1.indexOf(" ");
              //Serial.println(To_remove);
              over1.remove(To_remove,over1.length());
              //Serial.println(over1);
              over1num = over1.toInt();
              //Serial.println(over1num);   
                wicket1num = over1num;    
               // Serial.println(wicket1num); // got wicket 1 in int form == over1num
              

              //Serial.println(Data_Raw_3);
              To_remove = Data_Raw_3.indexOf(">");
              //Serial.println(Data_Raw_3);            
              //Serial.println(To_remove);  
              Data_Raw_3.remove(0,To_remove+1); 
              //Serial.println(Data_Raw_3);

              To_remove = Data_Raw_3.indexOf(">");
              Data_Raw_3.remove(0,To_remove+1);
              //Serial.println(Data_Raw_3);

              //To_remove = Data_Raw_3.indexOf(">");
              Data_Raw_3.remove(0,To_remove+1);
              //Serial.println(Data_Raw_3);

              To_remove = Data_Raw_3.indexOf(">");
              Data_Raw_3.remove(0,To_remove+1);
              //Serial.println(Data_Raw_3);

              To_remove = Data_Raw_3.indexOf(">");
              Data_Raw_3.remove(0,To_remove+1);
              Data_Raw_4 = Data_Raw_3;
              //Serial.println(Data_Raw_3);

              To_remove = Data_Raw_3.indexOf("<");
              Data_Raw_3.remove(To_remove,Data_Raw_3.length());
              //Serial.println(Data_Raw_3);
              team2 = Data_Raw_3;
               ////Serial.print("Team 2: ");
               ////Serial.println(team2);
              
              To_remove = Data_Raw_4.indexOf(">");    
              //Serial.println(To_remove); 
              Data_Raw_4.remove(0,To_remove+1);
               //Serial.println(Data_Raw_4);

               To_remove = Data_Raw_4.indexOf(">");    
              //Serial.println(To_remove); 
              Data_Raw_4.remove(0,To_remove+1);
              Data_Raw_5 = Data_Raw_4;
               //Serial.println(Data_Raw_4);

               
               To_remove = Data_Raw_4.indexOf("<");    
              //Serial.println(To_remove); 
              Data_Raw_4.remove(To_remove,Data_Raw_4.length());
               //Serial.println(Data_Raw_4);                                             
               score2 = Data_Raw_4;
               score2intermediate = Data_Raw_4;        
               //Serial.println(score2intermediate);
               ////Serial.print("Score 2: ");
               ////Serial.println(score2);

               To_remove = score2intermediate.indexOf("-");
               //Serial.println(To_remove);
               //Serial.println(score2intermediate);
               over2intermediate = score2intermediate;
               score2intermediate.remove(To_remove,score2intermediate.length());
               //Serial.println(score2intermediate);  // got score 2 in string form separate
               score2num = score2intermediate.toInt();
               //Serial.println(score2num); // score 2 in int form

               //Serial.println(over2intermediate);
               To_remove = over2intermediate.indexOf("-");
               over2intermediate.remove(0,To_remove+1);
               //Serial.println(over2intermediate);
               ovr2intermediate = over2intermediate; 
               over2num = over2intermediate.toInt(); 
               //Serial.println(over2num);   
               wicket2num = over2num;
               //Serial.println(wicket2num);   //got wicket 2 in int form 


               To_remove = ovr2intermediate.indexOf("(");
               ovr2intermediate.remove(0,To_remove+1);
               //Serial.println(ovr2intermediate);
               To_remove = ovr2intermediate.indexOf(" ");
               ovr2intermediate.remove(To_remove,ovr2intermediate.length());
               //Serial.println(ovr2intermediate);
               ovr2 = ovr2intermediate;
               //Serial.println(ovr2);  // got over 2 in string form ---> ovr2
               
              To_remove = Data_Raw_5.indexOf(">");
              Data_Raw_5.remove(0,To_remove+1);
              //Serial.println(Data_Raw_5);

              To_remove = Data_Raw_5.indexOf(">");
              Data_Raw_5.remove(0,To_remove+1);
              //Serial.println(Data_Raw_5);

              To_remove = Data_Raw_5.indexOf(">");
              Data_Raw_5.remove(0,To_remove+1);
              //Serial.println(Data_Raw_5);

              To_remove = Data_Raw_5.indexOf("<");
              Data_Raw_5.remove(To_remove,Data_Raw_5.length());
              //Serial.println(Data_Raw_5);
              commentary = Data_Raw_5;
              ////Serial.println(commentary);
                                                                                       
            }
        }
        else //If we can't get data
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();                                                                 
      } 
    else //If we can't connect to the HTTP
      {
        Serial.printf("[HTTP} Unable to connect\n");
      }

    while (WiFi.status() != WL_CONNECTED)     //In case the Wifi connexion is lost                                    
      { 
        
        WiFi.disconnect();                                                        
        delay(1000);                                                             
        
        WiFi.begin(ssid, password);                                              
        Serial.println("Reconnecting to WiFi..");       
        delay(10000);                                                             
      }


       lcd.clear();         
       lcd.setCursor(0, 0);
       lcd.print(team1);
       lcd.print(" ");
       lcd.setCursor(4, 0);
       lcd.print(score1num);
       lcd.print("-");
       lcd.print(wicket1num);
       lcd.print(" ");
       lcd.print("(");
       lcd.print(ovr1);
       lcd.print(")");

                
       lcd.setCursor(0,1);
       lcd.print(team2);
       lcd.print(" ");
       lcd.setCursor(4, 1);
       lcd.print(score2num);
       lcd.print("-");
       lcd.print(wicket2num);
       lcd.print(" ");
       lcd.print("(");
       lcd.print(ovr2);
       lcd.print(")");
       


       
//        lcd.clear();         // First innings
//        lcd.setCursor(0, 0);
//       lcd.print(team1);
//
//
//        lcd.setCursor(0, 1);
//        lcd.print(score1);

        

//         lcd.clear();         
//        lcd.setCursor(0, 0);
//       lcd.print(commentary);
        

//        lcd.clear();          // Second innings
//        lcd.setCursor(0, 0);
//        lcd.print(team2);
//
//
//        lcd.setCursor(0, 1);
//        lcd.print(score2);

//      if(digitalRead(button)==0){
//        lcd.clear();
//        lcd.setCursor(0, 0);
//        lcd.print(commentary);
//        Serial.println("button pressed");
//      }
        
       }
