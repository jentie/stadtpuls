/*
    ESPpost - http post json data set

    2022-01-05, Jens, MIT License

    based on 
    library example PostHTTPClient.ino
    https://randomnerdtutorials.com/esp8266-nodemcu-http-get-post-arduino/
    https://maakbaas.com/esp8266-iot-framework/logs/https-requests/
    
    additional info:
    https://stadtpuls.com/docs/http-requests

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "credentials.h"
//#define SSID "..."                    // wlan name
//#define PASS "..."                    // wlan passkey
//#define TOKEN "eyJ.."                 // token from sensor creation at stadtpuls.com


// helpful test server, responds with decoded request and data
// #define SERVER "https://httpbin.org/post"

// http://stadtpuls.com redirects to https://...
#define SERVER "https://api.stadtpuls.com/api/v3/sensors/68/records"
// attention! change sensor id!


void setup() {

  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("ESPpost - http post json");
  Serial.println();

  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {                     // connect to wlan
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("ip address: ");                               // connected
  Serial.println(WiFi.localIP());
}


void loop() {

  if ((WiFi.status() == WL_CONNECTED)) {                      // only with internet access

    // WiFiClient client;                                        // client w/o TLS

    WiFiClientSecure client;                                  // client using TLS
    client.setInsecure();                                     // w/o validating certificate
    client.connect(SERVER, 443);

    HTTPClient http;

    Serial.println("[http] begin...");
    http.begin(client, SERVER);
    http.addHeader("Content-Type", "application/json");       // add header fields
    http.addHeader("Authorization", "Bearer " TOKEN);         // header field "Authorization: Bearer eyJ..."

    const char* json = "{\"measurements\": [4711]}";          // payload, stadtpuls.com

    Serial.print("[http] post...\n");
    int httpCode = http.POST(json);                           // http post with with json

    if (httpCode < 0)
      Serial.println("[http] error");                         // errors before server response
    else {
      Serial.printf("[http] code: %d\n", httpCode);           // server response

      if (httpCode != 201)                                    // expected response code, stadtpuls.com
        Serial.println("Fehler beim Senden der Daten");
      else
        Serial.println("Daten wurden erfolgreich gesendet");

      const String& payload = http.getString();
      Serial.println("payload:");
      Serial.println(payload);
    }

    http.end();
    Serial.println("[http] end.");
  }

  delay(60000);
}
