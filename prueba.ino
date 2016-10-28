
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define DEBUG

#define trig 12
#define echo 13

int n, n1 = 0;
int engine = 0;


//-------------------------------------
int audioPin=A0;
int audioThreshold=40;
int audioAverage=0;
int audiotest=0;


int servoPin = 4; 
int moving=0;
int motorEnabler=0;

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
  if (!client){


    if(motorEnabler>0)
    {swing();delay(20);motorEnabler--;}
    
    else if( ((isMoving()==1) && (isCry()==1)) && (motorEnabler==0) )
    {moving=1;motorEnabler=5;}
    
    else
    {moving=0;}

    return;

    }
  #ifdef DEBUG
    Serial.println("new client request");
  #endif

  while (!client.available()){
    delay(1);}

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

/*int isCradling() {
  int result = engine ? 0 : 1;
  engine = !engine;
  return result;
}*/

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
      if (req.indexOf("engine=1") != -1)
        return 'e'; //success
      else
	return 'd';
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
    root["movement"] = String(moving);
  }
  else if (choice == 'e'){
    moving=1;motorEnabler=5;
  }
  else if (choice == 'd'){
    moving=0;motorEnabler=0;
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

int isMoving()
{

int trigPin=trig; int echoPin=echo;
//Devuelve la distancia detectada por el sensor en centimetros o
//en pulgadas
 
int duracion;
int medida;
 
//En la documentacion establece que para iniciar una medicion
//hay que suministrar un pulso de 10 microsegundos
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
 
//Calcula el tiempo que tarda en recibir el eco
duracion = pulseIn(echoPin, HIGH);
 
//Si metric es cierto, devuelve la distancia en centimetros.
//Si no, devuelve la distancia en pulgadas.
//Aplicamos las formulas que aparecen en la documentacion
 
medida = duracion/58;

int retorno = 0;

if(medida > 12 && medida < 18){retorno = 1;}
return retorno;

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
for (pos = 90; pos <= 115; pos += 1) { // goes from 0 degrees to 180 degrees
    servoMove(pos);              // tell servo to go to position in variable 'pos'
    delay(35);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 115; pos >= 65; pos -= 1) { // goes from 180 degrees to 0 degrees
    servoMove(pos);              // tell servo to go to position in variable 'pos'
    delay(35);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 65; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    servoMove(pos);              // tell servo to go to position in variable 'pos'
    delay(35);                       // waits 15ms for the servo to reach the position
  }
  }

