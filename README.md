# ESP-SIGFOX
ESP8266 and SIGFOX module integration

pio install lib 1893 89

Use platform = espressif8266@2.3.0 due to issue with tha latest version.

On my side, I use MQTT message .

Depending to your Sigfox Backend configuration, I set a http2Mqtt bridge (thanks to https://github.com/petkov/http_to_mqtt)

Here my Sigfox callback configuration :

