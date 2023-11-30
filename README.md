# TLog Plus
ESP8266/ESP32 Library that works like linux tee allowing you to log to multiple places at one time. 

* Author: Evotodi
* Author of original code: Dirk-Willem van Gulik <dirkx(at)webweaving(dot)org>
* Released under Apache version 2.0
* Telnet inspiration from https://github.com/LennartHennigs/ESPTelnet
* Logging priorities inspiration from log4cpp

## Description
Lets you sent logs to the Serial port, Telnet client, Syslog server, or a local web server that will scroll in a browser window.

## Examples
### Serial only example:
PlatformIO library dependants: evotodi/TLogPlus
```c++
#include <TLogPlus.h>
void setup() {
    Serial.begin(9600);
    TLogPlus::Log.begin();
}

void loop() {
    TLogPlus::Log.loop();
    
    TLogPlus::Log.println("Hello World");
    TLogPlus::Log<<"Hello from the world"<<TLogPlus::endl;
}
```
### Serial and Telnet example:
PlatformIO library dependants: evotodi/TLogPlus, evotodi/TLogPlusTelnetStream
```c++
#include <TLogPlus.h>
#include <TelnetSerialStream.h>

unsigned long previousMillis = 0;
const long interval = 3000;

TLogPlusStream::TelnetSerialStream telnetSerialStream = TLogPlusStream::TelnetSerialStream();

void setup() {
    Serial.begin(9600);
    
    telnetSerialStream.onInputReceived([](String str){
        Serial.print(str);
    });

    telnetSerialStream.onConnect([](IPAddress ipAddress){
        Serial.print("onConnection: Connection from ");
        Serial.println(ipAddress.toString());
    });
    
    telnetSerialStream.onDisconnect([](IPAddress ipAddress){
        Serial.print("onDisconnect: Disconnection from ");
        Serial.println(ipAddress.toString());
    });
    
    TLogPlus::Log.addPrintStream(std::make_shared<TLogPlusStream::TelnetSerialStream>(telnetSerialStream));
    
    WiFi.mode(WIFI_STA);
    Serial.println("Wifi begin");
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }
    
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address:  ");
    Serial.println(WiFi.macAddress());
    Serial.print("Hostname:  ");
    Serial.println(WiFi.getHostname());
    
    TLogPlus::Log.begin();
    
    TLogPlus::Log.begin();
    TLogPlus::Log.print("You can telnet to ");
    TLogPlus::Log.print(WiFi.localIP());
    TLogPlus::Log.println(" to see the logging output");
}

void loop() {
    TLogPlus::Log.loop();
    
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        TLogPlus::Log.println("Hello World");
    }
}   
```

### More Examples
See the examples folder of the repo
