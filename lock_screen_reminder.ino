#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Core.h"
#include "LightSensor.h"


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH    128     // OLED display width, in pixels
#define SCREEN_HEIGHT   64      // OLED display height, in pixels
#define OLED_RESET      4       // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// I/O-pins
#define trigPin                 9
#define echoPin                 10
#define DISTANCE_THRESHOLD      10
#define lightSensorPin          A5
#define LIGHT_THRESHOLD         18
#define TIMER_PERIOD            10000
#define nChecks                 5
#define buzzerPin               6
#define ledPin                  11

enum class States : byte {  // enum with own namespace
    RESET = 0,
    WORKPLACE_EMPTY = 1 << 0,   // binary 0000'0001
    MONITOR_ON = 1 << 1,        // binary 0000'0010
    TIMER_ENABLED = 1 << 2,     // binary 0000'0100
    TIMER_FINISHED = 1 << 3,    // binary 0000'1000
    ALARM_DISABLED = 1 << 4,    // binary 0001'0000
    AUDIO_VISUAL_ON = 1 << 5,   // binary 0010'0000
    MASK = B11111111            // binary 1111'1111
};

void setup() {
    InitializeDevices();
}

void loop() {
    unsigned long currentMillis = millis();
}

void InitializeDevices()
{
    // serial monitor
    Serial.begin(9600);

    States stateReg{ States::RESET };
    //States stateReg = States::RESET;
    unsigned long previousMillis = 0;

    // I/O-pins
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(lightSensorPin, INPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(ledPin, OUTPUT);

    // display setup
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))     // address 0x3C for 128x64
    { 
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }
    display.clearDisplay();     // clear display
    display.display();          // execute command
}