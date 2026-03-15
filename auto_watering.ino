// ============================================================
// AUTO WATERING SYSTEM
// Soil Moisture Sensor + Relay + Pump
// Compatible: Arduino Uno & ESP32
// Penu Djira | github.com/PenuDjira
// ============================================================

#if defined(ESP32)
  const int SOIL_PIN  = 1;
  const int ADC_MAX   = 4095;
  const int RELAY_PIN   = 12;
#else
  const int SOIL_PIN  = A0;
  const int ADC_MAX   = 1023;
  const int RELAY_PIN   = 7;
#endif


// Hysteresis thresholds
// Pump ON  when moisture drops BELOW DRY_THRESHOLD
// Pump OFF when moisture rises ABOVE WET_THRESHOLD
const float DRY_THRESHOLD = 30.0;
const float WET_THRESHOLD = 60.0;

// How long to run the pump before checking again (ms)
// Prevents flooding — pump won't run more than PUMP_DURATION at a time
const unsigned long PUMP_DURATION  = 10000;
const unsigned long READ_INTERVAL  = 2000;

bool pumpRunning = false;
unsigned long pumpStartTime = 0;
unsigned long lastReadTime  = 0;

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Relay off at startup
  Serial.println("Auto Watering System Ready");
}

void loop() {
  unsigned long now = millis();

  // Safety: auto-shutoff after PUMP_DURATION
  if (pumpRunning && (now - pumpStartTime >= PUMP_DURATION)) {
    digitalWrite(RELAY_PIN, LOW);
    pumpRunning = false;
    Serial.println("[PUMP] Auto-shutoff triggered.");
  }

  // Read sensor every READ_INTERVAL
  if (now - lastReadTime >= READ_INTERVAL) {
    lastReadTime = now;

    int raw = analogRead(SOIL_PIN);

    // Capacitive sensor: high raw = dry, low raw = wet
    // We invert so: high % = wet, low % = dry
    float moisture = (1.0 - raw / (float)ADC_MAX) * 100.0;

    Serial.print("[SENSOR] Raw: ");
    Serial.print(raw);
    Serial.print(" | Moisture: ");
    Serial.print(moisture, 1);
    Serial.print("%");

    if (!pumpRunning && moisture < DRY_THRESHOLD) {
      digitalWrite(RELAY_PIN, HIGH);
      pumpRunning = true;
      pumpStartTime = now;
      Serial.println(" → PUMP ON (soil dry)");
    } else if (pumpRunning && moisture > WET_THRESHOLD) {
      digitalWrite(RELAY_PIN, LOW);
      pumpRunning = false;
      Serial.println(" → PUMP OFF (soil wet enough)");
    } else {
      Serial.println(pumpRunning ? " [pumping...]" : " [idle]");
    }
  }
}

