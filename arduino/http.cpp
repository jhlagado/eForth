#include <Arduino.h>
#include <WebServer.h>

#include "index_html.h"

WebServer server(80);
String HTTPin;
String HTTPout;

void setupHttp(void (*handleInput)())
{
    server.begin();
    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", index_html);
    });
    server.on("/input", HTTP_POST, handleInput);
    server.begin();
}
