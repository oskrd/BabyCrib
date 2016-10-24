/*
 * WebServer for ESP8266 using Json
 * Based on Json Web Server by Benoit Blanchon. Whom inspired by "Web Server" from David A. Mellis and Tom Igoe
*/

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define DEBUG

int n = 0;

const char * ssid = "Los Hijos de Vicky";
const char * pass = "fofo1234";

WiFiServer server(69);
WiFiClient client;
StaticJsonBuffer<500> jsonBuffer;

void setup() {
  Serial.begin(115200);

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

  JsonObject & json = processRequest(req);
  writeResponse(client, json);

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


JsonObject & processRequest(String & req) {
  if (req.indexOf("GET") != -1) {
    if ( req.indexOf("/proximity") != -1) {
      JsonObject & json = generateJson(jsonBuffer, 'p');
      return json;
    }
    else if (req.indexOf("/crying") != -1) {
      JsonObject & json = generateJson(jsonBuffer, 'c');
      return json;
    }
    else {
      Serial.println("invalid request");
      client.stop();
      return jsonBuffer.createObject();
    }
  }
  else if (req.indexOf("POST") != -1) {
    if ( req.indexOf("/users/signin") != -1) {
      JsonObject & json = generateJson(jsonBuffer, 'p');
      return json;
    }
    else if (req.indexOf("/users/login") != -1) {
      JsonObject & json = generateJson(jsonBuffer, 'p');
      return json;
    }
    else if (req.indexOf("/movement") != -1) {
      JsonObject & json = generateJson(jsonBuffer, 'p');
      return json;
    }
    else {
      Serial.println("invalid request");
      client.stop();
      return jsonBuffer.createObject();
    }
  }
  else {
    Serial.println("invalid request");
    client.stop();
    return jsonBuffer.createObject();
  }

}

JsonObject& generateJson(JsonBuffer& jsonBuffer, char choice) {
  JsonObject& root = jsonBuffer.createObject();
  if (choice == 'p') {
    root["proximity"] = isMoving();
  }
  else if (choice == 'c') {
    root["crying"] = String(isMoving());
  }
  return root;
}


void writeResponse(WiFiClient& client, JsonObject& json) {
  String headers = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n";
  headers += "Connection: close\r\n";
  client.println(headers);
#ifdef DEBUG
  json.printTo(Serial);
#endif
  json.prettyPrintTo(client);
}

