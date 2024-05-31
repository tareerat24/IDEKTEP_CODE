#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <WiFi.h>

class WifiConfig
{

public:
  void wifiInit(const char *ssid, const char *password);
};
#endif
