#include <WiFi.h>
#include <ArduinoJson.h>
#include <ESP32Firebase.h>
#include <DHT.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define MQ135_PIN 35       // Pin connected to MQ-135 (analog pin)
#define DHT_PIN 23         // Pin connected to DHT22
#define DHT_TYPE DHT22     // DHT 22 (AM2302)

#define LED_PIN 12         // Pin connected to LED for GPS status indication

// Initialize DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);

// Initialize GPS
TinyGPSPlus gps;
HardwareSerial mySerial(1); // Use UART1 for GPS

#define _SSID "GAC"                              // Your WiFi SSID 
#define _PASSWORD "1234567890"                   // Your WiFi Password 
#define REFERENCE_URL "https://myiot-a57f5-default-rtdb.asia-southeast1.firebasedatabase.app/"  // Your Firebase project reference URL

Firebase firebase(REFERENCE_URL);

bool gpsLocationFetched = false;   // Flag to check if GPS location is fetched

// Function to blink LED
void blinkLED(int delayMs) {
    digitalWrite(LED_PIN, HIGH);
    delay(delayMs);
    digitalWrite(LED_PIN, LOW);
    delay(delayMs);
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);     // Set the LED pin as an output
    dht.begin();                  // Initialize the DHT sensor

    // Initialize GPS Serial
    mySerial.begin(9600, SERIAL_8N1, 16, 17); // RX on GPIO 16, TX on GPIO 17

    // Connect to WiFi
    WiFi.begin(_SSID, _PASSWORD);
    Serial.println("Connecting to WiFi...");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");

    firebase.json(true);          // Enable JSON parsing

    // Allow GPS module time to initialize and establish a connection
    Serial.println("Initializing GPS...");
    delay(2000);                  // Initial delay for GPS module setup

    // Fetch GPS location only once after startup
    while (!gpsLocationFetched) {
        while (mySerial.available() > 0) {
            gps.encode(mySerial.read());
        }
        
        // Check if valid location data is available
        if (gps.location.isValid() && gps.location.lat() != 0.0 && gps.location.lng() != 0.0) {
            gpsLocationFetched = true;   // Mark as fetched
            digitalWrite(LED_PIN, HIGH); // Turn on LED to indicate success
            Serial.println("Valid GPS data fetched");

            // Save GPS location data to Firebase
            float latitude = gps.location.lat();
            float longitude = gps.location.lng();
            
            if (firebase.setString("/sensorData/latitude", String(latitude)) &&
                firebase.setString("/sensorData/longitude", String(longitude))) {
                Serial.println("GPS data updated to Firebase");
            } else {
                Serial.println("Failed to update GPS data to Firebase");
            }
            digitalWrite(LED_PIN, LOW); // Turn off LED after saving GPS data
        } else {
            Serial.println("Waiting for valid GPS data...");
            blinkLED(500);              // Blink LED while waiting for GPS fix
        }
    }
}

void loop() {
    // Read MQ-135 sensor
    int sensorValue = analogRead(MQ135_PIN);   // Read the raw analog value (0-4095 for ESP32)
    int airQualityIndex = map(sensorValue, 0, 4095, 0, 1000);  // Map value to AQI

    // Read DHT22 sensor
    float temperature = dht.readTemperature();    // Read temperature in Celsius
    float humidity = dht.readHumidity();          // Read humidity in percentage
    float pressure = random(1000, 1050);          // Random pressure between 950 and 1050 hPa

    // Ensure that the other sensor data keys are created in Firebase
    if (firebase.setString("/sensorData/temperature", String(temperature)) &&
        firebase.setString("/sensorData/pressure", String(pressure)) &&
        firebase.setString("/sensorData/humidity", String(humidity)) &&
        firebase.setString("/sensorData/aqi", String(airQualityIndex))) {
        Serial.println("Sensor data updated successfully");
    } else {
        Serial.print("Failed to update sensor data");
    }

    delay(3000); // Wait for 3 seconds before the next reading
}
