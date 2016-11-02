
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define DEBUG

#define trig 12
#define echo 13

int n, n1 = 0;
int engine = 0;

int postt=0;

//-------------------------------------
int audioPin=A0;
int audioThreshold=20;
int audioAverage=0;
int audiotest=0;
int audioFlag=0;

int rSound=0;
int temporal=0;

int servoPin = 4; 
int moving=0;
int motorEnabler=0;


int trigPin=trig; 
int echoPin=echo; 
int duracion=0;
int medida=0;
int rUltra = 0;


const char * ssid = "Los 26";
const char * pass = "roldanquierepene";

WiFiServer server(69);
WiFiClient client;
StaticJsonBuffer<500> jsonBuffer;

void setup() {
  Serial.begin(115200);
   pinMode(audioPin,INPUT);   
   pinMode(trig, OUTPUT);
pinMode(echo, INPUT);
pinMode(servoPin,OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  #ifdef DEBUG
    Serial.print("Connecting to ");
    Serial.println(ssid);
  #endif
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {delay(500);Serial.print(".");}
  setAudioAverage(5);
  Serial.print("\nConnected @: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client){
    if(motorEnabler>0){swing();delay(20);motorEnabler--;}
    else if( ((isMoving()==1) && (isCry()==1)) && (motorEnabler==0) ){moving=1;motorEnabler=5;}
    else{moving=0;}
    return;
    }
    
  #ifdef DEBUG
    Serial.println("new client request");
  #endif
  String req = client.readString();
  #ifdef DEBUG
    Serial.println(req);
  #endif
  client.flush();

  char state = processRequest(req);
    
  if (state == 's'){writeOk(client);}
  else if (state == 'w'){writeBadResponse(client);}
  else{JsonObject & json = generateJson(jsonBuffer, state);writeResponse(client, json);}
    
  delay(1);
  #ifdef DEBUG
    Serial.println("Client disonnected");
  #endif
  client.stop();
}


char processRequest(String & req) {
  if (req.indexOf("GET") != -1) {
    if ( req.indexOf("/proximity") != -1) {return 'p';}
    else if (req.indexOf("/crying") != -1) {return 'c';}
    else if (req.indexOf("/movement") != -1) {return 'm';}
    else {Serial.println("invalid request");client.stop();return 'w';}
  }
  else if (req.indexOf("POST") != -1) {postt=1;
    if ( req.indexOf("/users/signin HTTP") != -1) {Serial.println(req.indexOf("user="));Serial.println(req.indexOf("pass="));return 's';}
    else if (req.indexOf("/users/login HTTP") != -1) {Serial.println(req.indexOf("user="));Serial.println(req.indexOf("pass="));return 's';}
    else if (req.indexOf("/movement HTTP") != -1) {if (req.indexOf("engine=1") != -1){return 'e';}else{return 'd';}}
    else {Serial.println("invalid request");client.stop();return 'w';}
  }
  else {Serial.println("invalid request");client.stop();return 'w'; }
}

JsonObject& generateJson(JsonBuffer& jsonBuffer, char choice) {
  JsonObject& root = jsonBuffer.createObject();
  if (choice == 'p') {root["proximity"] = isMoving();}
  else if (choice == 'c') {root["crying"] = String(isCry());}
  else if (choice == 'm') {root["movement"] = String(moving);}
  else if (choice == 'e'){moving=1;motorEnabler=5;}
  else if (choice == 'd'){moving=0;motorEnabler=0;}
  return root;
}

void writeResponse(WiFiClient& client, JsonObject& json) {
  String headers = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n";
  headers += "Access-Control-Allow-Origin: *\r\nConnection: close\r\n";
  client.println(headers);
  #ifdef DEBUG
    json.printTo(Serial);
    delay(111);
  #else
    delay(333);
  #endif
  if(postt==0){json.prettyPrintTo(client);if(json.size()<1){Serial.println("-------------------------Error detectado-------------------");delay(500);ESP.reset();}}
  else{postt=0;}
    
}

void writeOk(WiFiClient& client) {String headers = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n";headers += "Access-Control-Allow-Origin: *\r\nConnection: close\r\n";client.println(headers);}

void writeBadResponse(WiFiClient& client) {String headers = "HTTP/1.1 404 Not Found\r\nContent-Type: application/json\r\n";headers += "Access-Control-Allow-Origin: *\r\nConnection: close\r\n";client.println(headers);}

int getSound(){
  int temp=0;
  int sound=0;
  for(int i=0;i<5;i++){temp=analogRead(audioPin);sound = sound + temp;}
  sound = sound/5;
  return sound;
  }


int isCry(){
   audioFlag=0;
   temporal = 0;
   temporal=getSound();
   if((temporal > (audioAverage + audioThreshold))||(temporal < (audioAverage - audioThreshold))){audioFlag=1;audiotest=temporal;}
   return audioFlag;
   }

void setAudioAverage(int cycles){
  int total=0;
  int temporalValue=0;
  for(int i=0;i<cycles;i++){temporalValue=analogRead(audioPin);total = total + temporalValue;}
   audioAverage = total/cycles; 
  }

int isMoving()
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duracion = pulseIn(echoPin, HIGH);
medida = duracion/58;
rUltra = 0;
if(medida > 12 && medida < 18){rUltra = 1;}
return rUltra;

}


void servoMove (int angle)
{
   float pause;
   pause = angle*2000.0/180.0 + 500;
   digitalWrite(servoPin, HIGH);
   delayMicroseconds(pause);
   digitalWrite(servoPin, LOW);
   delayMicroseconds(25000-pause);
}

void swing(){
  int pos=0;
  for (pos = 90; pos < 110; pos += 1) {servoMove(pos);delay(50);}
  for (pos = 110; pos > 90; pos -= 1) {servoMove(pos);delay(50);};
  for (pos = 90; pos > 70; pos -= 1) {servoMove(pos);delay(50);}
  for (pos = 70; pos < 90; pos += 1) {servoMove(pos);delay(50);}
  }

