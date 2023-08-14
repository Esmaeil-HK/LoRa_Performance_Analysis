# LilyGO-TTGO-LoRa32-Performance_Analysis
 

* The sender sends: "Counter" with the counter number 
* The receiver shows: Internal counter, SNR and RSSi for LoRa packets plus received payload (Sender's Counter and number )

The code is derived and simplified from: 
https://github.com/cubapp/LilyGO-TTGO-LoRa32-SenderReceiver

## Arduino studio settings: 
**dependency library files**
* arduino-LoRa from https://github.com/sandeepmistry/arduino-LoRa 
* oled-ssd1306 from https://github.com/ThingPulse/esp8266-oled-ssd1306

**Arduino settings**
1. board: "TTGO LoRa32-OLED V1"
2. upload speed:  921600
3. flash frequency: 80MHz

## Building the code:
At first change the appropriate define in <board_def.h> - whether you compile Sender (LORA_SENDER 1) or Receiver (LORA_SENDER 0). Then change the frequency to 868MHz

```C
#define LORA_SENDER 0
//#define LORA_SENDER 1

#define LORA_PERIOD 868  
// #define LORA_PERIOD 915     
//#define LORA_PERIOD 433  
```


