/* Copyright 2008, 2012-2022 Dirk-Willem van Gulik <dirkx(at)webweaving(dot)org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Library that provides a fanout, or T-flow; so that output or logs do 
 * not just got to the serial port; but also to a configurable mix of a
 * telnetserver, a webserver, syslog or MQTT.
 */


#ifndef _H_MqttStream
#define _H_MqttStream

# ifdef MQTT

#include <TLogPlus.h>
#include <Print.h>
#include <PubSubClient.h>

class MqttStream : public TLog {
  public:
    MqttStream(Client * client, char * mqttServer = NULL, char * mqttTopic = NULL, const uint16_t mqttPort = 1883) :
      _client(client), _mqttPort(mqttPort) {
      if (mqttServer) _mqttServer = strdup(mqttServer);
      if (mqttTopic) _mqttTopic = strdup(mqttTopic);
    };
    MqttStream(PubSubClient * pubsub, char * mqttTopic = NULL) :
      _mqtt(pubsub) {
      if (mqttTopic) _mqttTopic = strdup(mqttTopic);
      _client = NULL; // used to detect the case where we're not resposible for the connection.
    };

    void setPort(uint16_t port) {
      _mqttPort = port;
    }

    void setTopic(const char * topic) {
      if (topic)
        _mqttTopic = strdup(topic);
    }

    void setServer(const char * server) {
      if (server)
        _mqttServer = strdup(server);
    }

    virtual size_t write(uint8_t c);
    virtual void begin();
    virtual void loop();
    virtual void reconnect();

  private:
    Client * _client = NULL;
    PubSubClient * _mqtt = NULL;
    const char * _mqttServer = NULL, * _mqttTopic = NULL;
    uint16_t _mqttPort = 0;
    char logbuff[512];
    size_t at = 0;
  protected:
};
# endif
#endif
