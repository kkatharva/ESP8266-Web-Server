#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

//Bring an object from Header file (OOPs Concept)

ESP8266WebServer server(80);

bool LEDstatus = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  pinMode(D4, OUTPUT);

  Serial.print("Connecting to ");
  Serial.print("ssid");
  // Wifi credentials
  WiFi.begin("ssid", "password");
  while (WiFi.status() != WL_CONNECTED) {  
  delay(1000);
  Serial.print(".");
}

  Serial.println("WiFi Connected!");
  Serial.println("IP is :");
  Serial.print(WiFi.localIP());

  server.on("/", handle_Connect);
  server.on("/ledON", handle_ledON);
  server.on("/ledOFF", handle_ledOFF);
  server.onNotFound(handle_NOTFOUND);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
 server.handleClient();

  if(LEDstatus == 1){
    digitalWrite(D4, HIGH);
  }
  else{
    digitalWrite(D4,LOW);
  }
}

// ROUTE HANDLER FUNCTIONS

void handle_Connect(){
  Serial.println(LEDstatus ? "LED: ON" : "LED: OFF");
  server.send(200, "text/html", updateWebpage(LEDstatus));
}

void handle_ledON(){
  LEDstatus = HIGH;
  Serial.println("LED: HIGH");
  server.send(200, "text/html",updateWebpage(LEDstatus));
}

void handle_ledOFF(){
  LEDstatus = LOW;
  Serial.println("LED: OFF");
  server.send(200, "text/html",updateWebpage(LEDstatus));
}

void handle_NOTFOUND(){
  server.send(404, "text/plain", "404: NOT FOUND");
}

// HTML Integration

String updateWebpage(uint8_t LEDstatus){
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>ESP8266 Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
    body {
      font-family: Helvetica, sans-serif;
      background-color: #f2f2f2;
      text-align: center;
      padding: 50px;
    }
    h1 {
      color: #333;
    }
    .button {
      padding: 15px 30px;
      font-size: 18px;
      border: none;
      border-radius: 8px;
      color: white;
      cursor: pointer;
      text-decoration: none;
    }
    .on {
      background-color: #00bfff;
    }
    .off {
      background-color: #ff4d4d;
    }
    </style>
  </head>
  <body>
    <h1>ESP8266 Web Server</h1>
    <h3>Using Station(STA) Mode</h3>
    STATUS_HTML
  </body>
  </html>
  )rawliteral";

// Logic for LED Status

  String statusBlock;
    if (LEDstatus) {
      statusBlock += "<p>LED: ON</p><a class=\"button off\" href=\"/ledOFF\">OFF</a>";
    } else {
      statusBlock += "<p>LED: OFF</p><a class=\"button on\" href=\"/ledON\">ON</a>";
    }

  html.replace("STATUS_HTML", statusBlock);
  return html;
}









