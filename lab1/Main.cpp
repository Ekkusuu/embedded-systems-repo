#include <Arduino.h>
#include <string.h>
#include <stdio.h>
#include "Led.h"
#include "SerialComm.h"

#define LED_PIN    13
#define BAUD_RATE  9600

Led        myLed(LED_PIN);
SerialComm serialComm;

void processCommand(const char* cmd) {
    if (strcasecmp(cmd, "led on") == 0) {
        myLed.turnOn();
        printf("LED is now ON\r\n");
    } else if (strcasecmp(cmd, "led off") == 0) {
        myLed.turnOff();
        printf("LED is now OFF\r\n");
    } else {
        printf("[ERROR] Unknown command: \"%s\"\r\n", cmd);
        printf("        Use: \"led on\" or \"led off\"\r\n");
    }
}

void setup() {
    myLed.begin();
    serialComm.begin(BAUD_RATE);
}

void loop() {
    if (serialComm.readLine()) {
        processCommand(serialComm.getCommand());
    }
}