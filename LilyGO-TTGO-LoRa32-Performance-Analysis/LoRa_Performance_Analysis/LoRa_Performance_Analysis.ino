/*
  LoRaWAN IoT Network Performance Project
  Authors: Esmaeil Alkhazmi, Assem Aldarrat, Mohammed Abbas
  University of Benghazi - Faculty of Engineering - Electrical and Electronic Engineering Department

  This code is part of the Performance Analysis and Implementation of The LoRaWAN IoT Network project
  conducted at the University of Benghazi. It implements a LoRa communication system with sender and
  receiver functionality, displaying relevant information on an OLED screen.

  Note: The arduino-LoRa Library (https://github.com/sandeepmistry/arduino-LoRa) was modified for this project.
  In the "arduino-LoRa/src/LoRa.h" path, a private function "int getSpreadingFactor();" was moved to public classes.
  This modification allows us to access and utilize the spreading factor information for our performance analysis.

  Code last modified: [14-8-2023]

*/

#include <SPI.h>
#include <LoRa.h>
#include "board_def.h"

OLED_CLASS_OBJ display(OLED_ADDRESS, OLED_SDA, OLED_SCL);

// Display size for better string placement
int width;
int height;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // If OLED_RST is defined, perform OLED reset sequence
  if (OLED_RST > 0) {
    pinMode(OLED_RST, OUTPUT);
    digitalWrite(OLED_RST, HIGH);
    delay(100);
    digitalWrite(OLED_RST, LOW);
    delay(100);
    digitalWrite(OLED_RST, HIGH);
  }

  // Initialize OLED display
  display.init();
  width = display.getWidth() / 2;
  height = display.getHeight() / 2;
  display.flipScreenVertically();
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  // Show device type on the display (Sender or Receiver)
  display.drawString(width - 50, height, LORA_SENDER ? "LoRa++ Sender" : "LoRa++ Receiver");
  display.display();
  delay(2000);

  // Initialize SPI and LoRa communication pins
  SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
  LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);

  // Start LoRa communication
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // If this device is a Receiver, display readiness message
  if (!LORA_SENDER) {
    display.clear();
    display.drawString(width - 50, height, "LoraRecv++ Ready");
    display.display();
  }

  // Set LoRa spreading factor and coding rate
  LoRa.setSpreadingFactor(8);
  // LoRa.setCodingRate4(6);
}

int count = 0;
void loop() {
#if LORA_SENDER
  // LORA SENDER PART -
  // Sending random number and counter to the LoRa Receiver
  count++;
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(width - 50, height, String(count) + ". Counter: " + String(count));
  display.display();
  
  // Begin LoRa packet transmission
  LoRa.beginPacket();
  LoRa.print(count);
  LoRa.print(". Counter = ");
  LoRa.print(count);
  LoRa.endPacket();
  delay(5000);
#else
  // LORA RECEIVER PART -
  // Displaying number and counter received from the LoRa Sender

  if (LoRa.parsePacket()) {
    String recv = "";
    while (LoRa.available()) {
      recv += (char)LoRa.read();
    }

    // Display LoRa packet information
    count++;
    display.clear();
    String rssi = "RSSI: " + String(LoRa.packetRssi());
    String snr = "SNR: " + String(LoRa.packetSnr());
    String offset = "OFFSET: " + String(float(LoRa.packetFrequencyError() / 1000)) + "kHz";
    String sf = "SF: " + String(LoRa.getSpreadingFactor());

    display.drawString(width - 50, height -25 , rssi);
    display.drawString(width - 50, height -15 , snr);
    display.drawString(width - 50, height -5, offset);
    display.drawString(width - 50, height  +5, sf);
    display.drawString(width - 50, height +15, recv);

    display.display();

    // Print packet information to Serial monitor
    Serial.println(String(count));
    Serial.println(rssi);
    Serial.println(snr);
    Serial.println(offset);
    Serial.println(sf);
  }
#endif
}