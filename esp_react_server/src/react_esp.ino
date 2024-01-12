#if defined(ESP8266)
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <FS.h>
#elif defined(ESP32)
#include <BluetoothSerial.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#endif

#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// const char *ssid = "Wave_Avilatek";
// const char *password = "WFKHWGQF9vwG";

const char *ssid = "Sanchez Fuentes";
const char *password = "09305573";
const char *hostname = "lesanpi";

void log(String text, String name)
{
  Serial.print("[");
  Serial.print(name);
  Serial.print("] ");
  Serial.println(text);
}

void startMDNS()
{
  MDNS.addService("http", "tcp", 80);
  log("MDNS Iniciado en http://" + String(hostname) + ".local", "MDNS");
}

void startWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print('.');
  }

  Serial.println("");
  log("Iniciado STA...\t", "WiFi");
  log(ssid, "WiFi.ssid");
  log("IP address:" + WiFi.localIP().toString(), "WiFi.ipAddress");
}

void startServer()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    log("GET index", "Server.get()");
    request->send(SPIFFS, "/index.html", "text/html"); });

  server.on("/demo.js", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    log("GET /demo.js", "Server.get()");
    request->send(SPIFFS, "/demo.js", "text/javascript"); });

  server.onNotFound([](AsyncWebServerRequest *request)
                    {
    log("GET not found", "Server.notFound");
    request->send(400, "text/plain", "Not found"); });

  server.begin();
  log("Server started", "Setup");
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  SPIFFS.begin();
  /// Start wifi
  startWifi();
  /// Begin MDNS Hostname
  MDNS.begin(hostname);
  /// Start server
  startServer();
  /// Start MDNS
  startMDNS();
}

void loop()
{
  // put your main code here, to run repeatedly:
}
