#ifndef WARC_H
#define WARC_H

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define UP_LEFT 5
#define UP_RIGHT 6
#define DOWN_LEFT 7
#define DOWN_RIGHT 8
#define TURN_LEFT 9
#define TURN_RIGHT 10
#define STOP 0


class WARC
{
public:
    static void handleRoot(AsyncWebServerRequest *request);
    static void handleNotFound(AsyncWebServerRequest *request);
    static void handleReadSensor(AsyncWebServerRequest *request);
    static void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    const char *getHtmlHomePage();
    static void processCarMovement(int inputValue);
};
#endif