#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//transmitter lates overwrite
const char *ssid = "INFINIA_IOT";
const char *password = "1q2w3e4r5t6y";
int readingInt=1;
ESP8266WebServer server(80);

void handleSentVar() {
  if (server.hasArg("sensor_reading")) { // this is the variable sent from the client
    int readingInt = server.arg("sensor_reading").toInt();
    server.send(200, "text/html", "Data received");
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, HIGH); //LED OFF AT FIRST
  digitalWrite(5, HIGH);  // RELAY OFF ATFIRST
  delay(1000);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();

  server.on("/data/", HTTP_GET, handleSentVar); // when the server receives a request with /data/ in the string then run the handleSentVar function
  server.begin();
}

void loop() {
  server.handleClient();
  Serial.println(">>>  client val:");
      Serial.println(readingInt);
  if (server.hasArg("sensor_reading")) { // this is the variable sent from the client
    readingInt = server.arg("sensor_reading").toInt(); 
     digitalWrite(LED_BUILTIN, HIGH);
     digitalWrite(5, HIGH);   //RELAY OFF     
    if ( readingInt == 0){
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(5, LOW);  //VRELAY ON
    delay(200);     
  }
  else if(readingInt){
     digitalWrite(LED_BUILTIN, HIGH);
     digitalWrite(5, HIGH);   //RELAY OFF  
  }
}
}
