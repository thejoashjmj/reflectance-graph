#include <WiFi.h>
#include <HTTPClient.h>

// --- Configuration ---
const char* ssid = "Android Kunjappan G11+"; // Wi-Fi Hotspot name
const char* password = "jmj_wifi";           // Wi-Fi Hotspot password
const char* serverUrl = "http://10.76.43.147:3000/data"; //Change to IP Address shown in hotspot for this device

// --- Pin Definitions ---
const int trigPin = 5;
const int echoPin = 18;
const int ldrPin = 34;

void setup() {
  Serial.begin(115200);
  pinMode(ldrPin, INPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Timeout added for safety
  long duration = pulseIn(echoPin, HIGH, 25000); 
  float distance = duration * 0.034 / 2;
  
  // Return 99 if out of range or no reading
  if (duration == 0) return 99.0;
  return distance;
}

void loop() {
  float currentDistance = getDistance();
  int ldrValue = analogRead(ldrPin);

  // Instant Serial Output for your debugging
  Serial.print("D:"); Serial.print(currentDistance);
  Serial.print(" L:"); Serial.println(ldrValue);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    // JSON payload
    String json = "{\"value\":" + String(ldrValue) + ", \"distance\":" + String(currentDistance) + "}";
    
    // Send the POST request
    int httpResponseCode = http.POST(json);
    
    // Close the connection
    http.end();
  } else {
    Serial.println("WiFi Disconnected!");
  }
  
  
  delay(100); 
}