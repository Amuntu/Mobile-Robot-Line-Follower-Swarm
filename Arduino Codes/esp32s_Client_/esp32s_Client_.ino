#include <WiFi.h>
#include <HTTPClient.h>

#define noData "NO"

#define motor1Pin1 27
#define motor1Pin2 26
#define enable1Pin 14
#define motor2Pin1 25
#define motor2Pin2 33 
#define enable2Pin 32 

#define LS 35
#define RS 12
#define LLS 36
#define RRS 11
#define BLACK 2000
// Setting PWM properties
#define freq 30000
#define pwmChannel 0
#define pwmChannel2 0
#define resolution 8

// PID constants
#define Kp 0.0125
#define Kd 0
#define Ki 0

unsigned long previousMillis = 0;
const long interval = 3000;

String commingDataFromServer="NO"; // final data comming from server .
bool Connected=false;

int P,D,I;
int lastError;

int motorspeeda, motorspeedb;
int basespeeda=200,basespeedb=200;
int maxspeeda=220,maxspeedb=220;
int numberOfNodes=0;
int Path[30];
// Set your access point network credentials
const char* ssid = "ROBOT";
const char* password = "Robot1234";

//Your IP address or domain name with URL path
const char* serverNamepath = "http://192.168.4.1/PathR1";

void getDataFromServer();
String httpGETRequest(const char*) ;
void ReConnect(); 
void PID_control();

char nodes2Command();
void StringPath2Int();

void Stop();
void Back(), Forward(), Left(), Right();
bool onBlack(int s);

void setup() {
 //Serial.begin(115200);
  WiFi.begin(ssid, password);
  //Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
  //  Serial.print(".");
  }
 // Serial.println("");
 // Serial.print("Connected to WiFi network with IP Address: ");
 // Serial.println(WiFi.localIP());*/
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  ledcSetup(pwmChannel2, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel2);
  ledcWrite(pwmChannel,0); 
  ledcWrite(pwmChannel2,0);   
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}
void loop() {
  getDataFromServer(serverNamepath);
  /*PID_control();
  ledcWrite(pwmChannel,motorspeeda); 
  ledcWrite(pwmChannel2,motorspeedb);*/
}

void getDataFromServer(const char* serverName){
  unsigned long currentMillis = millis(); // Store the time now .
   
  if(currentMillis - previousMillis >= interval)
  {
    if(WiFi.status()== WL_CONNECTED) // Check WiFi connection status
    { 
      commingData = httpGETRequest(serverName); // send the server request to http in order to get data .
      //Serial.println("Data: " + commingData );
      previousMillis = currentMillis; // save the last HTTP GET Request
    }
    else
      ReConnect();
  }
}

String httpGETRequest(const char* serverName)
{
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
  
  int httpResponseCode = http.GET(); // Send HTTP POST request
  
  String payload = "--"; // temporary string to hold the comming data .
  if (httpResponseCode>0)
  {
   // Serial.println(httpResponseCode);
    payload = http.getString(); // get the data from server and store it .
  }
  else
  {
    //Serial.print("Error code: ");
    //Serial.println(httpResponseCode);
  }
  http.end(); // Free resources
  return payload; // return the comming data from this function to stor it in "commingData" variable .
}
void ReConnect()
{
   WiFi.begin(ssid, password);
  //Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) 
    delay(500);
}
void PID_control() {
  int error = analogRead(LS)-analogRead(RS);
  P = error;
  I = I + error;
  D = error - lastError;
  lastError = error;
  int motorspeed = P*Kp + I*Ki + D*Kd;
  
  motorspeeda = basespeeda + motorspeed;
  motorspeedb = basespeedb - motorspeed;
  
  if (motorspeeda > maxspeeda) {
    motorspeeda = maxspeeda;
  }
  if (motorspeedb > maxspeedb) {
    motorspeedb = maxspeedb;
  }
  ledcWrite(pwmChannel,motorspeeda); 
  ledcWrite(pwmChannel2,motorspeedb);   
}
bool onBlack(int s){
  return analogRead(s)>BLACK;
}
void Back(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  delay(500);
  while(!onBlack(LLS)&&!onBlack(RRS)){
    PID_control();
  }
}
void Forward(){
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  while(!onBlack(LLS)&&!onBlack(RRS)){
    PID_control();
  }
}
void Left(){
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  ledcWrite(pwmChannel,maxspeeda); 
  ledcWrite(pwmChannel2,maxspeedb);   
  delay(500);
  while(!onBlack(RS))
      ;
  delay(100);
  Forward();
}
void Right(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  ledcWrite(pwmChannel,maxspeeda); 
  ledcWrite(pwmChannel2,maxspeedb);   
  delay(500);
  while(!onBlack(LS))
      ;
  delay(100);
  Forward();
}
void Stop(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  ledcWrite(pwmChannel,0); 
  ledcWrite(pwmChannel2,0);   
}
void StringPath2Int(){
  int j=0,k=0;
  int n=commingDataFromServer.length();
  char Temp[2];
  for(int i=0;i<n;i++){
    if(commingDataFromServer[i]=='.'){
      k=0;
      path[j]=atoi(Temp);
      j++;
      continue;
    }
    Temp[k]=commingDataFromServer[i];
    
  }
}
char nodes2Command(int i){
            if(now=='U' &&path[i+1]==path[i]+1 ){
                return 'R';now='R';
            }
            else if (now=='U'&&path[i+1]==path[i]-1){
                return 'L';now='L';
            }
            else if (now=='U'&&path[i+1]==path[i]+11){
                return 'F';now='U';                    
            }                                        
            else if (now=='D'&&path[i+1]==path[i]-1){
                return 'R';now='L';
            }
            else if (now=='D'&&path[i+1]==path[i]+1){                                        
                return 'L';now='R';                  
            }                                      
            else if (now=='D'&&path[i+1]==path[i]-11){
                return 'F';now='D';
            }
            else if (now=='R'&&path[i+1]==path[i]+1){
                return 'F';now='R';
            }
            else if (now=='R'&&path[i+1]==path[i]-11){
                return 'R';now='D';
            }
            else if (now=='R'&&path[i+1]==path[i]+11){
                return 'L';now='U';
            }
            else if (now=='L'&&path[i+1]==path[i]-1){
                return 'F';now='L';
            }
            else if (now=='L'&&path[i+1]==path[i]+11){
                return 'R';now='U';
            }
            else if (now=='L'&&path[i+1]==path[i]-11){
                return 'L';now='D';
            }
            else if (now=='U'&&path[i+1]==path[i]-11){
                return 'B';now='D';
            }
            else if (now=='D'&&path[i+1]==path[i]+11){
                return 'B';now='U';
            }
            else if (now=='R'&&path[i+1]==path[i]-1){
                return 'B';now='L';
            }
            else if (now=='L'&&path[i+1]==path[i]+1){
                return 'B';now='R';
            }
}
