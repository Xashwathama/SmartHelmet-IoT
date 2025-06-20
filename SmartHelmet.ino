// Constants for sensor thresholds and pins
const int IR_THRESHOLD = 1000;  // Threshold for IR helmet detection
const int ALCOHOL_THRESHOLD = 400;  // Threshold for alcohol detection
const int IR_LED_PIN = 8;  // IR LED transmitter pin
const int IR_RECEIVER_PIN = A2;  // IR Receiver pin
const int RED_LED_PIN = 9;  // Red LED for helmet pin
const int GREEN_LED_PIN = 10;  // Green LED for helmet pin
const int IGNITION_RED_LED_PIN = 11;  // New Red LED for ignition
const int IGNITION_GREEN_LED_PIN = 12;  // New Green LED for ignition
const int MQ3_DIGITAL_PIN = 2;  // MQ-3 digital pin
const int MQ3_ANALOG_PIN = A1;  // MQ-3 analog pin
const int BUZZER_PIN = 4;  // Buzzer pin

// Timer variables for non-blocking delays
unsigned long previousMillis = 0;
const long buzzerDuration = 5000;  // Buzzer active duration in milliseconds
bool buzzerActive = false;  // Flag to track buzzer state

// Struct for sensor data
struct SensorData {
  int irReceiverValue;
  int mq3AnalogValue;
  bool mq3DigitalValue;
};

// Function prototypes
void checkHelmet(SensorData &data);
void checkAlcohol(SensorData &data);
void updateLED(int ledPin, bool state);
void triggerBuzzer(bool state);
void debugSensorData(const SensorData &data);
void handleBuzzer();

void setup() {
  // Set pin modes
  pinMode(IR_LED_PIN, OUTPUT);
  pinMode(IR_RECEIVER_PIN, INPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(IGNITION_RED_LED_PIN, OUTPUT);
  pinMode(IGNITION_GREEN_LED_PIN, OUTPUT);
  pinMode(MQ3_DIGITAL_PIN, INPUT);
  pinMode(MQ3_ANALOG_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Start serial communication
  Serial.begin(9600);

  // Ensure all LEDs and buzzer are off at startup
  updateLED(RED_LED_PIN, LOW);
  updateLED(GREEN_LED_PIN, LOW);
  updateLED(IGNITION_RED_LED_PIN, LOW);
  updateLED(IGNITION_GREEN_LED_PIN, LOW);
  triggerBuzzer(false);
}

void loop() {
  // Sensor data struct
  SensorData data;

  // Read IR sensor value for helmet detection
  digitalWrite(IR_LED_PIN, HIGH);  // Keep IR LED on
  data.irReceiverValue = analogRead(IR_RECEIVER_PIN);

  // Read MQ-3 sensor values for alcohol detection
  data.mq3AnalogValue = analogRead(MQ3_ANALOG_PIN);
  data.mq3DigitalValue = digitalRead(MQ3_DIGITAL_PIN);

  // Debug sensor values
  debugSensorData(data);

  // Check helmet and alcohol status
  checkHelmet(data);

  // Handle buzzer timing
  handleBuzzer();
}

void checkHelmet(SensorData &data) {
  if (data.irReceiverValue < IR_THRESHOLD) {
    // Helmet detected
    updateLED(GREEN_LED_PIN, true);  // Turn on green LED for helmet
    checkAlcohol(data);  // Proceed to alcohol detection
  } else {
    // Helmet not detected
    updateLED(RED_LED_PIN, true);  // Turn on red LED for helmet
    updateLED(GREEN_LED_PIN, false);  // Ensure green LED for helmet is off
    updateLED(IGNITION_RED_LED_PIN, true);  // Turn on red LED for ignition (bike stops)
    updateLED(IGNITION_GREEN_LED_PIN, false);  // Ensure green LED for ignition is off
  }
}

void checkAlcohol(SensorData &data) {
  if (data.mq3DigitalValue == LOW || data.mq3AnalogValue > ALCOHOL_THRESHOLD) {
    // Alcohol detected, trigger buzzer and red LED
    Serial.println("Alcohol detected!");
    updateLED(RED_LED_PIN, true);
    updateLED(IGNITION_RED_LED_PIN, true);  // Ignition red LED on (bike stops)
    updateLED(IGNITION_GREEN_LED_PIN, false);  // Ignition green LED off
    triggerBuzzer(true);
    previousMillis = millis();  // Start timer for buzzer duration
    buzzerActive = true;  // Mark the buzzer as active
  } else {
    // No alcohol detected, keep green LED on and start bike (ignition green LED)
    updateLED(GREEN_LED_PIN, true);
    updateLED(RED_LED_PIN, false);
    updateLED(IGNITION_GREEN_LED_PIN, true);  // Ignition green LED on (bike starts)
    updateLED(IGNITION_RED_LED_PIN, false);  // Ignition red LED off
    triggerBuzzer(false);
    buzzerActive = false;  // Ensure buzzer is deactivated
  }
}

void updateLED(int ledPin, bool state) {
  digitalWrite(ledPin, state ? HIGH : LOW);
}

void triggerBuzzer(bool state) {
  digitalWrite(BUZZER_PIN, state ? HIGH : LOW);
}

void debugSensorData(const SensorData &data) {
  Serial.print("IR Receiver Value: ");
  Serial.println(data.irReceiverValue);
  Serial.print("MQ-3 Analog Value: ");
  Serial.println(data.mq3AnalogValue);
  Serial.print("MQ-3 Digital Value: ");
  Serial.println(data.mq3DigitalValue);
}

// Non-blocking delay for buzzer timing
void handleBuzzer() {
  if (buzzerActive) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= buzzerDuration) {
      triggerBuzzer(false);  // Turn off the buzzer after the duration
      buzzerActive = false;  // Reset the buzzer flag
    }
  }
}
