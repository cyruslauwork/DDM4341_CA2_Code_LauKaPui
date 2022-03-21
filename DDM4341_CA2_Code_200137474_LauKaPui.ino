//Realize Residential Noise Pollution Control and Anti-Burglary Through Arduino with Web
// plotChartWebServer
// Import required libraries
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Wire.h>
// Replace with your network credentials
const char* ssid = "";
const char* password = "";
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
float nA = 0, nB = 0, nC = 0,
      mA = 0, mB = 0, mC = 0;
String readNoiseA() {
  if (isnan(nA)) {
    Serial.println("Failed to read from noise sensor!");
    return "";
  } else {
    Serial.println("readNoiseA: ");
    Serial.print("\t");
    Serial.print(nA);
    return String(nA);
  }
}
String readNoiseB() {
  if (isnan(nB)) {
    Serial.println("Failed to read from noise sensor!");
    return "";
  } else {
    Serial.println("readNoiseB: ");
    Serial.print("\t");
    Serial.print(nB);
    return String(nB);
  }
}
String readNoiseC() {
  if (isnan(nC)) {
    Serial.println("Failed to read from noise sensor!");
    return "";
  } else {
    Serial.println("readNoiseC: ");
    Serial.print("\t");
    Serial.print(nC);
    return String(nC);
  }
}
String readMotionA() {
  if (isnan(mA)) {
    Serial.println("Failed to read from motion sensor!");
    return "";
  } else {
    Serial.println("readMotionA: ");
    Serial.print("\t");
    Serial.print(mA);
    return String(mA);
  }
}
String readMotionB() {
  if (isnan(mB)) {
    Serial.println("Failed to read from motion sensor!");
    return "";
  } else {
    Serial.println("readMotionB: ");
    Serial.print("\t");
    Serial.print(mB);
    return String(mB);
  }
}
String readMotionC() {
  if (isnan(mC)) {
    Serial.println("Failed to read from motion sensor!");
    return "";
  } else {
    Serial.println("readMotionC: ");
    Serial.print("\t");
    Serial.print(mC);
    return String(mC);
  }
}
// plotChartWebServer end

//gSheet
// Replace with your unique IFTTT URL resource
const char* resource = "";
// Maker Webhooks IFTTT
const char* IFTTTserver = "maker.ifttt.com";
//gSheet end

unsigned long now = millis();

// timerForIFTTT
#define timeSecondsIFTTT 5
unsigned long lastTriggerIFTTT;
void IRAM_ATTR IFTTT(){
  lastTriggerIFTTT = millis();
}
// timerForIFTTT end

// ifTriggered
#define timeSeconds 5
boolean triggered = false;
const int micPinFlatA = 13, micPinFlatB = 25, micPinFlatC = 26,
          motionPinFlatA = 27, motionPinFlatB = 14, motionPinFlatC = 12;
// Timer: Auxiliary variables
unsigned long lastTriggerMicPinFlatA = 0, lastTriggerMicPinFlatB = 0, lastTriggerMicPinFlatC = 0,
              lastTriggerMotionPinFlatA = 0, lastTriggerMotionPinFlatB = 0, lastTriggerMotionPinFlatC = 0;
boolean startTimerMicPinFlatA = false, startTimerMicPinFlatB = false, startTimerMicPinFlatC = false,
        startTimerMotionPinFlatA = false, startTimerMotionPinFlatB = false, startTimerMotionPinFlatC = false;
// Sets sth activate and starts a timer
void IRAM_ATTR detectsNoiseFlatA() {
  Serial.println("NOISE DETECTED!!! from FlatA");
  startTimerMicPinFlatA = true;
  lastTriggerMicPinFlatA = millis();
  nA = 1;
  triggered = true;
}
void IRAM_ATTR detectsNoiseFlatB() {
  Serial.println("NOISE DETECTED!!! from FlatB");
  startTimerMicPinFlatB = true;
  lastTriggerMicPinFlatB = millis();
  nB = 1;
  triggered = true;
}
void IRAM_ATTR detectsNoiseFlatC() {
  Serial.println("NOISE DETECTED!!! from FlatC");
  startTimerMicPinFlatC = true;
  lastTriggerMicPinFlatC = millis();
  nC = 1;
  triggered = true;
}
void IRAM_ATTR detectsMovementFlatA() {
  Serial.println("MOTION DETECTED!!! from FlatA");
  startTimerMotionPinFlatA = true;
  lastTriggerMotionPinFlatA = millis();
  mA = 1;
  triggered = true;
}
void IRAM_ATTR detectsMovementFlatB() {
  Serial.println("MOTION DETECTED!!! from FlatB");
  startTimerMotionPinFlatB = true;
  lastTriggerMotionPinFlatB = millis();
  mB = 1;
  triggered = true;
}
void IRAM_ATTR detectsMovementFlatC() {
  Serial.println("MOTION DETECTED!!! from FlatC");
  startTimerMotionPinFlatC = true;
  lastTriggerMotionPinFlatC = millis();
  mC = 1;
  triggered = true;
}
// ifTriggered end





void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  // timerForIFTTT
  IFTTT();
  // timerForIFTTT end

  // plotChartWebServer
  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.serveStatic("/", SPIFFS, "/");
  server.on("/noiseA", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readNoiseA().c_str());
  });
  server.on("/noiseB", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readNoiseB().c_str());
  });
  server.on("/noiseC", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readNoiseC().c_str());
  });
  server.on("/motionA", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readMotionA().c_str());
  });
  server.on("/motionB", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readMotionB().c_str());
  });
  server.on("/motionC", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readMotionC().c_str());
  });
  // Start server
  server.begin();
  // plotChartWebServer end
  
  // KY-038
  pinMode(micPinFlatA, INPUT);
  pinMode(micPinFlatB, INPUT);
  pinMode(micPinFlatC, INPUT);
  // KY-038 end

  // PIRMotionSensor
  pinMode(motionPinFlatA, INPUT);
  pinMode(motionPinFlatB, INPUT);
  pinMode(motionPinFlatC, INPUT);
  // PIRMotionSensor end
  
}





void loop() {
  // Current time
  now = millis();
  
  // afterEvent
  // Turn off sth after the number of seconds defined in the timeSeconds variable
  if(startTimerMicPinFlatA && (now - lastTriggerMicPinFlatA > (timeSeconds*1000))) {
    Serial.println("Noise sensor from FlatA auto reset...");
    startTimerMicPinFlatA = false;
    nA = 0;
  }
  if(startTimerMicPinFlatB && (now - lastTriggerMicPinFlatB > (timeSeconds*1000))) {
    Serial.println("Noise sensor from FlatB auto reset...");
    startTimerMicPinFlatB = false;
    nB = 0;
  }
  if(startTimerMicPinFlatC && (now - lastTriggerMicPinFlatC > (timeSeconds*1000))) {
    Serial.println("Noise sensor from FlatC auto reset...");
    startTimerMicPinFlatC = false;
    nC = 0;
  }
  if(startTimerMotionPinFlatA && (now - lastTriggerMotionPinFlatA > (timeSeconds*1000))) {
    Serial.println("Motion sensor from FlatA auto reset...");
    startTimerMotionPinFlatA = false;
    mA = 0;
  }
  if(startTimerMotionPinFlatB && (now - lastTriggerMotionPinFlatB > (timeSeconds*1000))) {
    Serial.println("Motion sensor from FlatB auto reset...");
    startTimerMotionPinFlatB = false;
    mB = 0;
  }
  if(startTimerMotionPinFlatC && (now - lastTriggerMotionPinFlatC > (timeSeconds*1000))) {
    Serial.println("Motion sensor from FlatC auto reset...");
    startTimerMotionPinFlatC = false;
    mC = 0;
  }
  if (triggered && (now - lastTriggerIFTTT > (timeSecondsIFTTT*1000))){
    makeIFTTTRequest();
    triggered = false;
    IFTTT();
  }
  // afterEvent end

  int val_digital = digitalRead(micPinFlatA);
  /*
  Serial.print("micPinFlatA");
  Serial.print("\t");
  Serial.println(val_digital);
  */
  if (val_digital == HIGH){
    detectsNoiseFlatA();
  }
  int val_digital2 = digitalRead(micPinFlatB);
  /*
  Serial.print("micPinFlatB");
  Serial.print("\t");
  Serial.println(val_digital2);
  */
  if (val_digital2 == HIGH){
    detectsNoiseFlatB();
  }
  int val_digital3 = digitalRead(micPinFlatC);
  /*
  Serial.print("micPinFlatC");
  Serial.print("\t");
  Serial.println(val_digital3);
  */
  if (val_digital3 == HIGH){
    detectsNoiseFlatC();
  }
  int val_digital4 = digitalRead(motionPinFlatA);
  /*
  Serial.print("motionPinFlatA");
  Serial.print("\t");
  Serial.println(val_digital4);
  */
  if (val_digital4 == HIGH){
    detectsMovementFlatA();
  }
  int val_digital5 = digitalRead(motionPinFlatB);
  /*
  Serial.print("motionPinFlatB");
  Serial.print("\t");
  Serial.println(val_digital5);
  */
  if (val_digital5 == HIGH){
    detectsMovementFlatB();
  }
  int val_digital6 = digitalRead(motionPinFlatC);
  /*
  Serial.print("motionPinFlatC");
  Serial.print("\t");
  Serial.println(val_digital6);
  */
  if (val_digital6 == HIGH){
    detectsMovementFlatC();
  }
  
}




//gSheet
// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest() {
  Serial.print("Connecting to "); 
  Serial.print(IFTTTserver);
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(IFTTTserver, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  Serial.print("Request resource: "); 
  Serial.println(resource);
  String jsonObject = String("{\"value1\":\"") + nA + "\",\"value2\":\"" + nB
                      + "\",\"value3\":\"" + nC + "\"}";
  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + IFTTTserver); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
  int timeout = 5 * 10; // 5 seconds
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  Serial.println("\nclosing connection");
  client.stop(); 
}
//gSheet end
