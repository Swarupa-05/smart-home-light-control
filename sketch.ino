#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create OLED object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pin connections
#define LDR_PIN     34      // LDR Sensor
#define PIR_PIN     14      // PIR Motion Sensor
#define LED_PIN     23      // LED
#define BUTTON_PIN  18      // Push Button

// Variables
bool manualMode = false;        // Start in AUTO mode
bool ledState = false;          // LED initially OFF
bool lastButtonState = HIGH;    // Store previous button state

void setup() {

  // Start Serial Monitor
  Serial.begin(115200);

  // Start I2C communication
  Wire.begin(21,22);

  // Set pin modes
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Turn LED OFF initially
  digitalWrite(LED_PIN, LOW);

  // Initialize OLED Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println("OLED Failed");
    while(1);
  }

  // Welcome message
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,20);
  display.println("SMART");
  display.display();

  delay(2000);
}

void loop() {

  // Read LDR value
  int ldrValue = analogRead(LDR_PIN);

  // Read PIR sensor
  bool motion = digitalRead(PIR_PIN);

  // Read push button
  bool buttonState = digitalRead(BUTTON_PIN);

  // Change AUTO/MANUAL mode when button is pressed
  if(lastButtonState == HIGH && buttonState == LOW){

    manualMode = !manualMode;

    // Keep previous LED state in Manual Mode
    if(manualMode)
      ledState = !ledState;

    delay(200);   // Button debounce
  }

  // Save current button state
  lastButtonState = buttonState;

  // AUTO Mode
  if(!manualMode){

    // Turn ON light only if it is dark and motion is detected
    if(ldrValue < 1800 && motion){
      ledState = true;
    }
    else{
      ledState = false;
    }

  }

  // Update LED
  digitalWrite(LED_PIN, ledState);

  // OLED Display
  display.clearDisplay();

  display.setTextSize(1);

  // Project title
  display.setCursor(18,0);
  display.println("SMART HOME");

  display.drawLine(0,10,127,10,SSD1306_WHITE);

  // Show Light Status
  display.setCursor(0,15);
  display.print("Light : ");

  if(ldrValue < 1800)
    display.println("Dark");
  else
    display.println("Bright");

  // Show Motion Status
  display.setCursor(0,27);
  display.print("Motion: ");

  if(motion)
    display.println("Detected");
  else
    display.println("No Motion");

  // Show LED Status
  display.setCursor(0,39);
  display.print("LED   : ");

  if(ledState)
    display.println("ON");
  else
    display.println("OFF");

  // Show Current Mode
  display.setCursor(0,51);
  display.print("Mode  : ");

  if(manualMode)
    display.println("MANUAL");
  else
    display.println("AUTO");

  // Display everything on OLED
  display.display();

  // Print data in Serial Monitor
  Serial.println("-------------------------");

  Serial.print("LDR Value : ");
  Serial.println(ldrValue);

  Serial.print("Motion    : ");
  if(motion)
    Serial.println("Detected");
  else
    Serial.println("No Motion");

  Serial.print("LED       : ");
  if(ledState)
    Serial.println("ON");
  else
    Serial.println("OFF");

  Serial.print("Mode      : ");
  if(manualMode)
    Serial.println("MANUAL");
  else
    Serial.println("AUTO");

  // Refresh every 300 ms
  delay(300);
}