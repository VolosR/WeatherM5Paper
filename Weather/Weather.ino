#include <M5EPD.h>
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson.git
#include <NTPClient.h>           //https://github.com/taranais/NTPClient
#include "Orbitron_Medium_20.h"
#include "Orbitron44.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>

M5EPD_Canvas canvas(&M5.EPD);

char temStr[10];
char humStr[10];

float temHere;
float humHere;

const char* ssid     = "xxxxxxxxxxx";       ///EDIIIT
const char* password = "xxxxxxxx"; //EDI8IT
String town="Zagreb";              //EDDIT
String Country="HR";                //EDDIT
const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q="+town+","+Country+"&units=metric&APPID=";
const String key = "xxxxxxxxxxxxxxxx"; /*EDDITTTTTTTTTTTTTTTTTTTTTTTT                      */

String payload=""; //whole json 
 String tmp="" ; //temperatur
  String hum="" ; //humidity
  

StaticJsonDocument<1000> doc;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;



void setup(void) {
   
    M5.begin();
    M5.EPD.SetRotation(90);
    M5.EPD.Clear(true);
    M5.RTC.begin();
    M5.SHT30.Begin();
    canvas.createCanvas(540, 960);
    canvas.useFreetypeFont(false);
    canvas.setFreeFont(&Orbitron_Medium_25); 
    canvas.drawJpgFile(SD, "/back.jpg");
    canvas.pushCanvas(0,0,UPDATE_MODE_GC16 );

WiFi.begin(ssid, password);

  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
   
  }
   timeClient.begin(); 
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(3600);   /*EDDITTTTTTTTTTTTTTTTTTTTTTTT                      */
 // getData();
  delay(100);
}
int i=0;
String tt="";
int count=0;
String curSeconds="";

void loop() {

   
   getData();
   
 

   
    

          while(!timeClient.update()) {
          timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
     formattedDate = timeClient.getFormattedDate();
 // Serial.println(formattedDate);

    
    
    
   
 
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
 
 
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  curSeconds=timeStamp.substring(6,8);
  String current=timeStamp.substring(0,5);
      


    canvas.drawString(current,140,884);
    canvas.drawString(String(M5.getBatteryVoltage()/1000.00),430,884);
     
    canvas.pushCanvas(0,0,UPDATE_MODE_A2  );    
  delay(700);
  M5.shutdown(600);
}




void getData()
{
    
   if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
 
    http.begin(endpoint + key); //Specify the URL
    int httpCode = http.GET();  //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
         payload = http.getString();
 
      }
 
    else {
      //Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
 char inp[1000];
 payload.toCharArray(inp,1000);
 deserializeJson(doc,inp);
  
  String tmp2 = doc["main"]["temp"];
  String hum2 = doc["main"]["humidity"];
  String town2 = doc["name"];
  String pressure = doc["main"]["pressure"];
  String visib = doc["main"]["visibility"];
  String windSpeed = doc["wind"]["speed"];
  tmp=tmp2;
  hum=hum2;

    M5.SHT30.UpdateData();
    temHere = M5.SHT30.GetTemperature();
    humHere = M5.SHT30.GetRelHumidity();

    canvas.setFreeFont(&Orbitron_Bold_44);
    canvas.drawString(tmp.substring(0,4),340,260);
    canvas.drawString(String(temHere).substring(0,4),180,260);
    canvas.setFreeFont(&Orbitron_Bold_66);
    canvas.drawString(hum,348,580);
    canvas.drawString(String((int)humHere),190,580);
    canvas.setFreeFont(&Orbitron_Medium_25);
    canvas.drawString(town2,122,784);
    canvas.drawString(pressure+" hPa",164,846);
   
    canvas.drawString(windSpeed+" m/s",122,816);
    
   
  
   
 }
         

     
