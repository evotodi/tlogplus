/** Setup your WiFi credentials */
//#define WIFI_NETWORK "DavisHome" // Uncomment to enable WiFi for streamers
//#define WIFI_PASSWD "CottonDog" // Uncomment to enable WiFi for streamers

/** Choose optional streamers */
//#define TELNET // Uncomment to enable telnet logging
//#define WEBSTREAM // Uncomment to enable web logging
//#define SYSLOG_HOST "loghost.my.domain.com" // Uncomment to enable syslog logging
//#define SYSLOG_PORT 514 // Uncomment to set the syslog port
//#define MQTT_HOST "mqttserver.my.domain.com" // Uncomment to activate mqtt logging

/** The example */
#include <Arduino.h>
#include <TLogPlus.h>

#ifdef WIFI_NETWORK
#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#endif

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#endif
#endif

// Optional to keep from typing TLogPlus::Log a lot
using namespace TLogPlus;

#ifdef TELNET
#include <TelnetSerialStream.h>
using namespace TLogPlusStream;
TelnetSerialStream telnetSerialStream = TelnetSerialStream();
#endif

#ifdef WEBSTREAM
#include <WebSerialStream.h>
using namespace TLogPlusStream;
WebSerialStream webSerialStream = WebSerialStream();
#endif

#ifdef SYSLOG_HOST
#include <SyslogStream.h>
using namespace TLogPlusStream;
SyslogStream syslogStream = SyslogStream();
#endif

#ifdef MQTT_HOST
#include <MqttlogStream.h>
using namespace TLogPlusStream;
// EthernetClient client;
WiFiClient client;
MqttStream mqttStream = MqttStream(&client);
char topic[128] = "log/foo";
#endif

void setup() {
    // Start serial
    Serial.begin(19200);
    while (!Serial) {
        delay(1);
    }

#ifdef TELNET
    Log.addPrintStream(std::make_shared<TelnetSerialStream>(telnetSerialStream));
#endif

#ifdef WEBSTREAM
    Log.addPrintStream(std::make_shared<WebSerialStream>(webSerialStream));
#endif

#ifdef SYSLOG_HOST
    syslogStream.setDestination(SYSLOG_HOST);
    syslogStream.setRaw(false); // wether or not the syslog server is a modern(ish) unix.
#  ifdef SYSLOG_PORT
    syslogStream.setPort(SYSLOG_PORT);
#  endif
    Log.addPrintStream(std::make_shared<SyslogStream>(syslogStream));
#endif

#ifdef MQTT_HOST
    mqttStream.setServer(MQTT_HOST);
    mqttStream.setTopic(topic);
    Log.addPrintStream(std::make_shared<MqttStream>(mqttStream));
#endif

#ifdef WIFI_NETWORK
    // Start WiFi
    WiFi.begin(WIFI_NETWORK, WIFI_PASSWD);
    while (!WiFi.isConnected()) {
        Log.println("No network yet");
        delay(1000);
    }

    // Call mDNS to make our serial-2-telnet service visible and easy to find.
    MDNS.begin("the-works-server");
#endif
    // Begin TLogPlus
    Log.begin();

    // Set the logging level. Defaults to Priority::NOSET which logs all levels
    Log.setLevel(Priority::ERROR);

    // Turn on show level prefix. e.g. ERROR: my big bad error   e.g. INFO: The ship has landed
    Log.showLevel(true);

    // Print the ip address
    Log.print("We have network. IP:");
    Log.println(WiFi.localIP());

    // Use streams to log also
    Log << "Setup is complete" << endl;
}

void loop() {
    Log.loop();

    // do something every 5 seconds.
    static unsigned  long last_report = millis();
    if (millis() - last_report < 5 * 1000)
        return;

    Log.println("Hello from the loop");

    // Stream addition
    Log << "Last report millis: " << last_report;
    Log << "  Current Millis: " << millis() << endl;

    // Update our last report for the loop
    last_report = millis();
};