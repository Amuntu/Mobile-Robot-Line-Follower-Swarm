#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define noData "NO"

// Set your access point network credentials
const char* ssid = "ROBOT";
const char* password = "Robot1234";

AsyncWebServer server(80);
bool R1=false,R2=false;
bool InterruptR1=false,InterruptR2=false;

char RR1[100]="01.12.13.14.",RR2[100]="..";
void getFromSerialQt(); // incomming Data from Qt.

void setup(){
  Serial.begin(115200);
  WiFi.softAP(ssid);//Remove the password, if you want the AP (Access Point) to be open

  server.on("/PathR1", HTTP_GET, [](AsyncWebServerRequest *request){
    //if(R1){
      request->send_P(200, "text/plain", RR1);
      R1=false;
 /*  }
   else{
    char no[3]="&&";
    request->send_P(200, "text/plain", no);
   }*/
  });
  server.on("/InterruptR1", HTTP_GET, [](AsyncWebServerRequest *request){
  if(InterruptR1){
    char T[3]="TT";
      request->send_P(200, "text/plain", T);
  }
  });
  server.on("/PathR2", HTTP_GET, [](AsyncWebServerRequest *request){
    if(R2){
      request->send_P(200, "text/plain", RR2);
      R2=false;
   }
   else{
    char no[3]="&&";
    request->send_P(200, "text/plain", no);
   }
  });
  server.on("/InterruptR2", HTTP_GET, [](AsyncWebServerRequest *request){
  if(InterruptR1){
    char T[3]="TT";
      request->send_P(200, "text/plain", T);
  }
  });
  server.begin();// Start server
}
 
void loop(){
  while(!Serial.available())
  ;
  getFromSerialQt();
}
void getFromSerialQt(){
  String comming=Serial.readString();
  int i=0;
  if(comming[i]=='_'){
    i++;
    if(comming[i]=='1'){
      i+=2;
      R1=true;
      int j=0;
      while(comming[i]!='_'){
        RR1[j]=comming[i];
        i++;
        j++;
      }
    }
    if(comming[i]=='2'){
      i+=2;
      R2=true;
      int j=0;
      while(comming[i]!='_'){
        RR2[j]=comming[i];
        i++;
        j++;
      }
    }
  }
  if(comming[i]=='I'){
    i++;
    if(comming[i]=='1')
      InterruptR1=true;
    else
      InterruptR2=true;
  }
}
