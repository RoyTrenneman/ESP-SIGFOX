# ESP-SIGFOX
ESP8266 and SIGFOX module integration

## Framework used: Arduino

pio lib dependancy: PubSubClient_ID89 c99-sigfox-wisol-WSSFM10R1AT_ID1893

## IDE used:
[PlatformIO Core](https://docs.platformio.org/en/latest/quickstart.html)

Use platform = espressif8266@2.3.0 due to issue with tha latest version.

On my side, I use MQTT message .

Depending to your Sigfox Backend configuration, I set a http2Mqtt bridge (thanks to https://github.com/petkov/http_to_mqtt)

Here my Sigfox callback configuration :

![Alt text](./sigfox.JPG)
