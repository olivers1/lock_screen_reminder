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
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);   // instantiate display object

// I/O-pins
#define trigPin1                9
#define echoPin1                10
#define trigPin2                6
#define echoPin2                8
#define DISTANCE_THRESHOLD      10
#define HEIGHT_THRESHOLD        170
#define lightSensorPin          A5
#define LIGHT_THRESHOLD         18
#define TIMER_PERIOD            10000
#define nChecks                 5
#define buzzerPin               5
#define ledPin                  11

const unsigned int TIME_INTERVAL_1 = 1000;   // interval for how often (milliseconds) program should loop
unsigned long previousMillis = 0;  


void setup() {
    InitializeDevices();
    PermanentTextToDisplay();
}


void loop() {
    // instantiate senor objects
    static DistanceSensor distanceSensor1(trigPin1, echoPin1, DISTANCE_THRESHOLD);
    static DistanceSensor distanceSensor2(trigPin2, echoPin2, HEIGHT_THRESHOLD);
    static LightSensor lightSensor(lightSensorPin, LIGHT_THRESHOLD);
    static StateRegisterHandler stateRegister(&distanceSensor1, 5, &distanceSensor2, 5, &lightSensor, 5);   // = (.., number of measurements in a row to confirm 'empty workplace', .., number of measurements in a row to confirm 'user is standing', .., number of measurements in a row to confirm 'external monitor on', ..)
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
        PrintVariablesToDisplay(&stateRegister);     // update display with latest variable values
    }
}

void InitializeDevices()
{
    // serial monitor
    Serial.begin(9600);

    unsigned long previousMillis = 0;

    // I/O-pins
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
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

void PermanentTextToDisplay()
{
    // print text to display
    display.setTextSize(1);   // font size
    display.setTextColor(WHITE);
    display.setCursor(0, 0);  // coordinates (x, y) where text should be printed
    display.print("Lock Screen Counter");

    display.setCursor(0, 26);  // coordinates (x, y) where text should be printed
    display.print("Occations:");

    display.setCursor(0, 41);  // coordinates (x, y) where text should be printed
    display.print("Elapsed Time:");

    display.setCursor(0, 55);  // coordinates (x, y) where text should be printed
    display.print("State Reg.:");
    display.display();
}

void PrintVariablesToDisplay(StateRegisterHandler* stateRegisterHandlerObj)
{
    // print occation counter to display
    display.fillRect(85, 18, 30, 20, BLACK);    // clear any redundant digits
    display.display();  // execute command
    
    display.setTextSize(2);   // font size
    display.setCursor(85, 18);  // coordinates (x, y) where text should be printed
    display.setTextColor(WHITE);    // set text colour
    display.print(stateRegisterHandlerObj->GetForgotLockCnt());

    // print elapsed time to display
    display.setTextSize(1);   // font size
    display.fillRect(85, 41, 50, 10, BLACK);    // clear any redundant digits
    display.display();  // execute command
    
    display.setCursor(85, 41);  // coordinates (x, y) where text should be printed
    display.setTextColor(WHITE);    // set text colour
    display.print(stateRegisterHandlerObj->GetElapsedTime());

    // print state register (flag register) to display
    display.setTextSize(1);   // font size

    int i = 0;
    for(int b = 7; b >= 0; b--, i++)
    {
      display.fillRect(75 + 6 * i, 55, 6, 10, BLACK);    // clear any redundant digits. =(x, y, left upper corner coordinate, width, height, colour)
      display.display();  // execute command

      // print from whole binary number MSB to LSB
      display.setTextColor(WHITE);    // set text colour
      display.setCursor((75 + 6 * i), 55);   // coordinates (x, y) where text should be printed
      display.print(bitRead(stateRegisterHandlerObj->GetStateRegister(), b));
      display.display();
    }
}
