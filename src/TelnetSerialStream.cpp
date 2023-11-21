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

#include "TLogPlus.h"
#include "TelnetSerialStream.h"

size_t TelnetSerialStream::write(uint8_t c) {
  if (!_server)
    return 1;
  for (int i = 0; i < _maxClients; i++) {
    if (_serverClients[i] && _serverClients[i]->connected()) {
      _serverClients[i]->write(c);
    };
  };
  return 1;
}

size_t TelnetSerialStream::write(uint8_t * c, size_t s) {
  if (!_server)
    return 1;
  for (int i = 0; i < _maxClients; i++) {
    if (_serverClients[i] && _serverClients[i]->connected()) {
      _serverClients[i]->write(c,s);
    };
  };
  return s;
}

TelnetSerialStream::~TelnetSerialStream() {
  stop();
  free(_serverClients);
}

void TelnetSerialStream::begin() {
  if (_server != NULL)
    return;
  _server = new WiFiServer(_telnetPort);
  _server->begin();
  _serverClients = (WiFiClient **)malloc(sizeof(WiFiClient *) * _maxClients);
  if (_serverClients == NULL) {
      _maxClients = 0;
      Log.println("Disabling telnet logging -- insufficient memory.");
      stop();
  };
  for (int i = 0; i < _maxClients; i++)
    _serverClients[i] = NULL;

  Log.printf("Opened serial server on telnet://%s:%d\n", WiFi.localIP().toString().c_str(), _telnetPort);
#ifdef MDNS
  MDNS.addService("telnet", "tcp", _telnetPort);
#endif
};

void TelnetSerialStream::stop() {
  if (!_server)
    return;
  for (int i = 0; i < _maxClients; i++) {
    if (_serverClients[i]) {
      _serverClients[i]->println("Connection closed from remote end");
      _serverClients[i]->stop();
      delete _serverClients[i];
      _serverClients[i] = NULL;
    }
  }
  _server->stop();
  delete _server;
  _server = NULL;
}

void TelnetSerialStream::loop() {
  if (!_server)
    return;

  if (_server->hasClient()) {
    int i;
    for (i = 0; i < _maxClients; i++) {
      if (!_serverClients[i] || !_serverClients[i]->connected()) {
        if (_serverClients[i]) {
          _serverClients[i]->stop();
          delete _serverClients[i];
          _serverClients[i] = NULL;
        };

        _serverClients[i] = new WiFiClient(_server->available());

        _serverClients[i]->print("Telnet connection");
        if (identifier().length()) {
            _serverClients[i]->print(" to ");
            _serverClients[i]->print(identifier());
    };
        _serverClients[i]->println();

        Log.print(_serverClients[i]->remoteIP());
        Log.print(":");
        Log.print(_serverClients[i]->remotePort());
        Log.println(" connected by telnet");

        break;
      };
    };
    if (i >= _maxClients) {
      //no free/disconnected spot so reject
      Log.println("Too many log/telnet clients. rejecting.");
      _server->available().stop();
    }
  }

  //check clients for data & silently ditch their data
  for (int i = 0; i < _maxClients; i++) {
    if (!_serverClients[i])
      continue;
    if (!_serverClients[i]->connected()) {
      Log.print(_serverClients[i]->remoteIP());
      Log.print(":");
      Log.print(_serverClients[i]->remotePort());
      Log.println(" closed the connection.");
      _serverClients[i]->stop();
      delete _serverClients[i];
      _serverClients[i] = NULL;
      continue;
    }

    if (!_serverClients[i]->available())
      continue;
    if(on_input) {
        while (_serverClients[i]->available()) {
            char c = _serverClients[i]->read();
            handleInput(c);
        }
    } else {
        char c = _serverClients[i]->read();
        if (c > 0 && c < 32) {
             Serial.println("Ignoring telnet input");
        };
    }
  }
}

bool TelnetSerialStream::isLineModeSet() {
    return _lineMode;
}

void TelnetSerialStream::setLineMode(bool value /* = true */) {
    _lineMode = value;
}

void TelnetSerialStream::onInputReceived(CallbackFunction f) {
    on_input = f;
}

void TelnetSerialStream::handleInput(char c) {

    // collect string
    if (_lineMode) {
        if (c != '\n') {
            if (c >= 32 && c < 127) {
                input += c;
            }
            // EOL -> send input
        } else {
            on_input(input);
            input = "";
        }
        // send individual characters
    } else {
        if (input.length()) {
            on_input(input + String(c));
            input = "";
        } else {
            on_input(String(c));
        }
    }
}