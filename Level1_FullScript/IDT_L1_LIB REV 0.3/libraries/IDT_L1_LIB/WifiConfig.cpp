#include "WifiConfig.h"

// const char *ssid = "ESP32_AGV";
// const char *password = "00000000";

void WifiConfig::wifiInit(const char *ssid, const char *password)
{

  Serial.println("Setting AP (Access Point)");
  WiFi.softAP(ssid, password);
  Serial.print("SSID: ");
  Serial.println(ssid);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(IP);
}