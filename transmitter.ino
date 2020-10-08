#include <ESP8266WiFi.h>
#include <Ticker.h>




const char *ssid = "INFINIA_IOT";
const char *password = "1q2w3e4r5t6y";

int sensorValue = 0;        // value read from the potentiometer
int outputValue = 0;        // value sent to server

Ticker Cem_Tick;
volatile int WatchDogCount = 0;

void ISRWatchDog(){
  WatchDogCount++;
 // Serial.print("Counter's value increased to");
 // Serial.println(WatchDogCount);
  if  (WatchDogCount == 28800){
    WatchDogCount = 0;
    ESP.reset();
  }
}

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(2,INPUT);
  Serial.begin(115200);
  Cem_Tick.attach(1,ISRWatchDog);//1 st argument is in seconds after which the ISRwatchdog() exacutes
  delay(10);

  // Explicitly set the ESP8266 to be a WiFi-client
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("wi fi not connected");
  }

}

void loop() {
  // read the analog in value:
  sensorValue = digitalRead(2);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const char * host = "192.168.4.1";
  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request. Something like /data/?sensor_reading=123
  String url = "/data/";
  url += "?sensor_reading=";
  url += sensorValue;

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  delay(500);
}
