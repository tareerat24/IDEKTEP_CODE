#include "WARC.h"
#include "Motor.h"
#include "Sensors.h"
#include <Arduino_JSON.h>

WARC warcInstance;
Sensors sensorsInstance;

JSONVar readings;

const char *WARC::getHtmlHomePage()
{
  const char *htmlHomePage = R"HTMLHOMEPAGE(
        <!DOCTYPE html>
        <html>
        <head>
            <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
            <style>
                .arrows {
                    font-size: 70px;
                    color: AliceBlue;
                }
        
                .circularArrows {
                    font-size: 80px;
                    color: Cornsilk;
                }
        
                td {
                    background-color: black;
                    border-radius: 35%;
                    box-shadow: 5px 5px #888888;
                }
        
                td:active {
                    transform: translate(5px, 5px);
                    box-shadow: none;
                }
        
                .noselect {
                    -webkit-touch-callout: none;
                    -webkit-user-select: none;
                    -khtml-user-select: none;
                    -moz-user-select: none;
                    -ms-user-select: none;
                    user-select: none;
                    touch-action: manipulation;
                }
            </style>
        </head>
        
        <body class="noselect" align="center" style="background-color:white">
            <h1 style="color: teal;text-align:center;">AGV &#128663; Web Controller</h1>
            <h2 style="color: teal;text-align:center;">Wi-Fi &#128732; Mode</h2>
            <table id="mainTable" style="width:400px;margin:auto;table-layout:fixed" CELLSPACING=10>
                <tr>
                    <td ontouchstart='onTouchStartAndEnd("5")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#11017;</span></td>
                    <td ontouchstart='onTouchStartAndEnd("1")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#8679;</span></td>
                    <td ontouchstart='onTouchStartAndEnd("6")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#11016;</span></td>
                </tr>
                <tr>
                    <td ontouchstart='onTouchStartAndEnd("3")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#8678;</span></td>
                    <td style="background-color:white;box-shadow:none"></td>
                    <td ontouchstart='onTouchStartAndEnd("4")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#8680;</span></td>
                </tr>
                <tr>
                    <td ontouchstart='onTouchStartAndEnd("7")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#11019;</span></td>
                    <td ontouchstart='onTouchStartAndEnd("2")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#8681;</span></td>
                    <td ontouchstart='onTouchStartAndEnd("8")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows">&#11018;</span></td>
                </tr>
                <tr>
                    <td ontouchstart='onTouchStartAndEnd("9")' ontouchend='onTouchStartAndEnd("0")'><span class="circularArrows">&#8634;</span></td>
                    <td style="background-color:white;box-shadow:none">
                        <h2 style="color: DarkMagenta;text-align:center;">Via &#128640; IDektep</h2>
                    </td>
                    <td ontouchstart='onTouchStartAndEnd("10")' ontouchend='onTouchStartAndEnd("0")'><span class="circularArrows">&#8635;</span></td>
                </tr>
            </table>
              <h2 style="color: DarkMagenta;text-align:center;">Distance: <span id="displayText"></span></h2>
        
            <script>
                // Get current sensor readings when the page loads  
                window.addEventListener('load', getReadings);

                var webSocketUrl = "ws:\/\/" + window.location.hostname + "/ws";
                var websocket;
        
                function initWebSocket() {
                    websocket = new WebSocket(webSocketUrl);
                    websocket.onopen = function (event) { };
                    websocket.onclose = function (event) {setTimeout(initWebSocket, 2000);};
                    websocket.onmessage = function (event) { };
                }
        
                function onTouchStartAndEnd(value) {
                    websocket.send(value);
                }
        
                window.onload = initWebSocket;
                document.getElementById("mainTable").addEventListener("touchend", function (event) {
                    event.preventDefault()
                });
                
                function getReadings(){
                  var xhr = new XMLHttpRequest();
                  var textElement = document.getElementById('displayText');


                  xhr.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                      const myObj = JSON.parse(this.responseText);
                      textElement.textContent = myObj.distance;
                    }
                  }; 
                  xhr.open("GET", "/readings", true);
                  xhr.send();
                }

                setInterval(getReadings, 500);

        
            </script>
        </body>    
        </html>
    )HTMLHOMEPAGE";
  return htmlHomePage;
}

void WARC::handleRoot(AsyncWebServerRequest *request)
{
  request->send_P(200, "text/html", warcInstance.getHtmlHomePage());
}

void WARC::handleReadSensor(AsyncWebServerRequest *request)
{
  long distance = Sensors::ultraGetDistance();
  String json = "{\"distance\":" + String(distance) + "}";
  request->send(200, "application/json", json);
}

void WARC::handleNotFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not Found");
}

void WARC::onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected from %s\n", client->id(), client->remoteIP().toString().c_str());
    WARC::processCarMovement(0);
    break;
  case WS_EVT_DATA:
    AwsFrameInfo *info;
    info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
      std::string myData = "";
      myData.assign((char *)data, len).c_str();
      Serial.println(atoi(myData.c_str()));
      WARC::processCarMovement(atoi(myData.c_str()));
    }
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  default:
    break;
  }
}

void WARC::processCarMovement(int inputValue) {
  switch (inputValue) {
    case UP:
      Motor::forward();
      break;
    case DOWN:
      Motor::backward();
      break;
    case LEFT:
      Motor::slide_left();
      break;
    case RIGHT:
      Motor::slide_right();
      break;
    case UP_LEFT:
      Motor::slide_left_front();
      break;
    case UP_RIGHT:
      Motor::slide_right_front();
      break;
    case DOWN_LEFT:
      Motor::slide_left_back();
      break;
    case DOWN_RIGHT:
      Motor::slide_right_back();
      break;
    case TURN_LEFT:
      Motor::turn_left();
      break;
    case TURN_RIGHT:
      Motor::turn_right();
      break;
    case STOP:
      Motor::stop();
      break;
    default:
      Motor::stop();
      break;
  }
}

