
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define DEBUG

int n, n1 = 0;
int engine = 0;


//-------------------------------------
int audioPin=A0;
int audioThreshold=40;
int audioAverage=0;
int audiotest=0;







const char * ssid = "Los 26";
const char * pass = "roldanquierepene";

WiFiServer server(69);
WiFiClient client;
StaticJsonBuffer<500> jsonBuffer;

void setup() {
  Serial.begin(115200);
   pinMode(audioPin,INPUT);   
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  #ifdef DEBUG
    Serial.print("Connecting to ");
    Serial.println(ssid);
  #endif
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  setAudioAverage(5);
  Serial.print("\nConnected @: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client)
    return;

  #ifdef DEBUG
    Serial.println("new client request");
  #endif

  while (!client.available())
    delay(1);

  String req = client.readString();
  #ifdef DEBUG
    Serial.println(req);
  #endif
  client.flush();

  char state = processRequest(req);
  if (state == 's')
    writeOk(client);
  else if (state == 'w')
    writeBadResponse(client);
  else{
    JsonObject & json = generateJson(jsonBuffer, state);
    writeResponse(client, json);
  }
    
  delay(1);
  #ifdef DEBUG
    Serial.println("Client disonnected");
  #endif
  client.stop();
}

//Tester method
int isMoving() {
  int result = n ? 0 : 1;
  n = !n;
  return result;
}

/*int isCry() {
  int result = n1 ? 0 : 1;
  n1 = !n1;
  return result;
}*/

int isCradling() {
  int result = engine ? 0 : 1;
  engine = !engine;
  return result;
}

char processRequest(String & req) {
  if (req.indexOf("GET") != -1) {
    if ( req.indexOf("/proximity") != -1) {
      //JsonObject & json = generateJson(jsonBuffer, 'p');
      return 'p';
    }
    else if (req.indexOf("/crying") != -1) {
      //JsonObject & json = generateJson(jsonBuffer, 'c');
      return 'c';
    }
    else if (req.indexOf("/movement") != -1) {
      //JsonObject & json = generateJson(jsonBuffer, 'm');
      return 'm';
    }
    else {
      Serial.println("invalid request");
      client.stop();
      return 'w'; //wrong
    }
  }
  else if (req.indexOf("POST") != -1) {
    if ( req.indexOf("/users/signin HTTP") != -1) {
      Serial.println(req.indexOf("user="));
      Serial.println(req.indexOf("pass="));
      return 's'; //success
    }
    else if (req.indexOf("/users/login HTTP") != -1) {
      Serial.println(req.indexOf("user="));
      Serial.println(req.indexOf("pass="));
      return 's'; //success
    }
    else if (req.indexOf("/movement HTTP") != -1) {
      Serial.println(req.indexOf("engine="));
      return 's'; //success
    }
    else {
      Serial.println("invalid request");
      client.stop();
      return 'w'; //wrong
    }
  }
  else {
    Serial.println("invalid request");
    client.stop();
    return 'w'; //wrong
  }

}

JsonObject& generateJson(JsonBuffer& jsonBuffer, char choice) {
  JsonObject& root = jsonBuffer.createObject();
  if (choice == 'p') {
    root["proximity"] = isMoving();
  }
  else if (choice == 'c') {
    root["crying"] = String(isCry());
  }
  else if (choice == 'm') {
    root["movement"] = String(isCradling());
  }
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
    json.prettyPrintTo(client);
}

void writeOk(WiFiClient& client) {
  String headers = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n";
  headers += "Access-Control-Allow-Origin: *\r\nConnection: close\r\n";
  client.println(headers);
}

void writeBadResponse(WiFiClient& client) {
  String headers = "HTTP/1.1 404 Not Found\r\nContent-Type: application/json\r\n";
  headers += "Access-Control-Allow-Origin: *\r\nConnection: close\r\n";
  client.println(headers);
}


int getSound(){

    int total=0;
  int temporalValue=0;
  
  for(int i=0;i<5;i++){
   temporalValue=analogRead(audioPin);
   total = total + temporalValue;
   }
   total = total/5;
   return total ;
  
  }


int isCry(){

   int audioFlag=0;
   int testValue = 0;
   
   testValue=getSound();
   if((testValue > (audioAverage + audioThreshold))||(testValue < (audioAverage - audioThreshold))){
   audioFlag=1;
   Serial.println("----------------------------------------------------------ruido");
   audiotest=testValue;
   }
   return audioFlag;
  
  }

void setAudioAverage(int cycles){

  int total=0;
  int temporalValue=0;
  
  for(int i=0;i<cycles;i++){
   temporalValue=analogRead(audioPin);
   total = total + temporalValue;
   }
   audioAverage = total/cycles; 

  }





