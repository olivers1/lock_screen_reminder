#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "LightSensor.h"
#include "DistanceSensor.h"
#include "StateRegisterHandler.h"
#include "Actuator.h"
#include "Timer.h"


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

const unsigned int TIME_INTERVAL_1 = 1000;   // interval for how often (milliseconds) program should loop
unsigned long previousMillis = 0;  

/*
    enum class States : byte {  // enum with its own namespace
        RESET = 0,
        WORKPLACE_EMPTY = 1 << 0,   // binary 0000'0001
        MONITOR_ON = 1 << 1,        // binary 0000'0010
        TIMER_ENABLED = 1 << 2,     // binary 0000'0100
        TIMER_FINISHED = 1 << 3,    // binary 0000'1000
        ALARM_ENABLED = 1 << 4,     // binary 0001'0000
        LED_ALARM_ON = 1 << 5,		// binary 0010'0000
        BUZZER_ALARM_ON = 1 << 6,	// binary 0100'0000
        MASK = B11111111            // binary 1111'1111
    };

States stateReg{ States::RESET };
//States stateReg = States::RESET;

inline constexpr States
operator&(States x, States y)
{
    return static_cast<States>
        (static_cast<byte>(x) & static_cast<byte>(y));
}

inline constexpr States
operator|(States x, States y)
{
    return static_cast<States>
        (static_cast<byte>(x) | static_cast<byte>(y));
}

inline constexpr States
operator^(States x, States y)
{
    return static_cast<States>
        (static_cast<byte>(x) ^ static_cast<byte>(y));
}

inline constexpr States
operator~(States x)
{
    return static_cast<States>(~static_cast<int>(x));
}

inline States&
operator&=(States& x, States y)
{
    x = x & y;
    return x;
}

inline States&
operator|=(States& x, States y)
{
    x = x | y;
    return x;
}

inline States&
operator^=(States& x, States y)
{
    x = x ^ y;
    return x;
}
*/

void setup() {
    InitializeDevices();
}


void loop() {
    // instantiate senor objects
    static DistanceSensor distanceSensor(trigPin, echoPin, DISTANCE_THRESHOLD);
    static LightSensor lightSensor(lightSensorPin, LIGHT_THRESHOLD);
    static StateRegisterHandler stateRegister(5, 5, &distanceSensor, &lightSensor);    // = (number of measurements in a row to confirm 'empty workplace', external monitor, .., ..)
    static Actuator actuator(ledPin, 2, 900, buzzerPin, 3, 30, 3, &stateRegister);     // = (.., number of led alarm loops, time in milliseconds between each led flash, .., number of buzzer alarm loops, time in milliseconds between each sound alarm, setting which alarm types that are active, ..)
    static Timer timer(10000, &stateRegister);     // (delay time in milliseconds before activating alarm when requirements are fulfilled, ..)

    actuator.AlarmActivationHandler();  // check if any alarm should be activated and if any alarm should be activated. it handles activation and lenght of alarm signals

    // check workplace according a preset time interval set by 'time interval 1' variable
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= TIME_INTERVAL_1)
    {
        previousMillis = currentMillis;

        stateRegister.CheckWorkplace();     // check workplace and update flags in state register if workplace is empty, external monitor is turned on etc.
        timer.TimerActivationHandler();     // activate timer if certain flags in state register is set
    }
}

void InitializeDevices()
{
    // serial monitor
    Serial.begin(9600);

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