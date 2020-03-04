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
#define lightSensorPin          A5
#define buzzerPin               5
#define ledPin                  11
#define DISTANCE_THRESHOLD      60      // in cm
#define HEIGHT_THRESHOLD        120     // distance in cm, measured from top of work desk to the ceiling. Distance below this value is counted as raised work desk
#define LIGHT_THRESHOLD         650
#define TIMER_PERIOD            10000   // in milliseconds
#define N_CHECKS                5
#define N_LED_ALARM_LOOPS       2 
#define LED_TIME_PERIOD         900     // in milliseconds
#define N_BUZZER_ALARM_LOOPS    10  
#define BUZZER_TIME_PERIOD      20     // in milliseconds
#define ALARM_TYPE              2       // 0 = no alarm, 1 = led light alarm, 2 = buzzer alarm, 3 = both led light and buzzer alarm

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
    static StateRegisterHandler stateRegister(&distanceSensor1, N_CHECKS, &distanceSensor2, N_CHECKS, &lightSensor, N_CHECKS);   
    static Actuator actuator(ledPin, N_LED_ALARM_LOOPS, LED_TIME_PERIOD, buzzerPin, N_BUZZER_ALARM_LOOPS, BUZZER_TIME_PERIOD, ALARM_TYPE, &stateRegister);
    static Timer timer(TIMER_PERIOD, &stateRegister);     // (delay time in milliseconds before activating alarm when requirements are fulfilled, ..)

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

    display.setCursor(0, 8);  // coordinates (x, y) where text should be printed
    display.print("Desk Elevated:");

    display.setCursor(0, 27);  // coordinates (x, y) where text should be printed
    display.print("Left Unlocked:");

    display.setCursor(0, 41);  // coordinates (x, y) where text should be printed
    display.print("Elapsed Time:");

    display.setCursor(0, 55);  // coordinates (x, y) where text should be printed
    display.print("State Reg.:");
    display.display();
}

void PrintVariablesToDisplay(StateRegisterHandler* stateRegisterHandlerObj)
{
    // print number of occations desk was elevated, to display
    display.fillRect(90, 1, 30, 20, BLACK);    // clear any redundant digits
    display.display();  // execute command

    display.setTextSize(2);     // font size
    display.setCursor(90, 1);   // coordinates (x, y) where text should be printed
    display.setTextColor(WHITE);    // set text colour
    display.print(stateRegisterHandlerObj->GetDeskElevatedCnt());


    // print number of occations desk was left with computer unlocked counter, to display
    display.fillRect(90, 20, 30, 20, BLACK);    // clear any redundant digits
    display.display();  // execute command
    
    display.setTextSize(2);   // font size
    display.setCursor(90, 20);  // coordinates (x, y) where text should be printed
    display.setTextColor(WHITE);    // set text colour
    display.print(stateRegisterHandlerObj->GetForgotLockCnt());

    // print elapsed time, to display
    display.setTextSize(1);   // font size
    display.fillRect(90, 41, 50, 10, BLACK);    // clear any redundant digits
    display.display();  // execute command
    
    display.setCursor(90, 41);  // coordinates (x, y) where text should be printed
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
