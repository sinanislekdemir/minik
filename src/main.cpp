#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <kernel.hpp>

WiFiMulti WiFiMulti;

void setup() {
  kmain();
  // We start by connecting to a WiFi network
  // WiFiMulti.addAP("Islekdemir", "kensingtonstationlondon1809");

  // Serial.println();
  // Serial.println();
  // Serial.print("Waiting for WiFi... ");

  // while (WiFiMulti.run() != WL_CONNECTED) {
  //   Serial.print(".");
  //   delay(500);
  // }

  // Serial.println("");
  // Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());

  // register_statements();

  // command c = parse("XOR 34 21 testing \"something\"", 1);
  // log_command(&c);

  // delay(500);
}

void loop() { delay(60 * 1000); }
