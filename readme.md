# Auto Watering System
**Soil Moisture Sensor + Relay + Pump | Arduino Uno & ESP32**

A minimal but production-minded automatic watering system. The sensor reads soil moisture, the microcontroller decides, the relay acts. No button. No manual input.

Built as a companion to the YouTube video series by [Penu Djira](https://youtube.com/@PenuDjira).

---

## How It Works

Most tutorials give you a sensor that prints numbers. This gives you a sensor that **does something**.

The logic is simple:
- Moisture drops below **30%** → pump turns ON
- Moisture rises above **60%** → pump turns OFF
- Pump runs longer than **10 seconds** → auto-shutoff regardless

The gap between 30% and 60% is intentional. This is called **hysteresis** — it prevents the pump from rapidly switching on and off when moisture hovers around a single threshold. Your fridge and thermostat use the same principle.

---

## Wiring

| Component | Pin/Terminal | Arduino Uno | ESP32 |
|-----------|-------------|-------------|-------|
| Soil Sensor | VCC | 5V | 3.3V |
| Soil Sensor | GND | GND | GND |
| Soil Sensor | AOUT | A0 | GPIO 1 |
| Relay Module | VCC | 5V | 5V |
| Relay Module | GND | GND | GND |
| Relay Module | IN | D7 | GPIO 12 |
| Relay | COM | Power (+) | Power (+) |
| Relay | NO | Pump (+) | Pump (+) |

> ⚠️ Use the **NO (Normally Open)** terminal on the relay — not NC. NO means pump is off by default and only activates when relay triggers. Using NC will run your pump constantly.

> ⚠️ If your pump runs on 12V, use a separate external power supply. Never pull 12V from Arduino.

---

## Sensor Note — Inverted Output

If you're using a **capacitive soil moisture sensor**, the raw ADC output is inverted on most cheap modules:
- Dry soil → high raw value
- Wet soil → low raw value

This code handles it with:
```cpp
float moisture = (1.0 - raw / (float)ADC_MAX) * 100.0;
```

If you're using a **resistive sensor** (two exposed metal probes), use the standard formula instead:
```cpp
float moisture = (raw / (float)ADC_MAX) * 100.0;
```

Full explanation → [Video #1: Why Your Soil Moisture Sensor Reads Backwards](https://youtu.be/7y6n6QiSPww)

---

## Configuration

All tunable parameters are at the top of the file:

```cpp
const float DRY_THRESHOLD  = 30.0;   // % — pump turns ON below this
const float WET_THRESHOLD  = 60.0;   // % — pump turns OFF above this
const unsigned long PUMP_DURATION = 10000; // ms — safety shutoff
const unsigned long READ_INTERVAL = 2000;  // ms — how often to read sensor
```

Adjust these for your specific plant, pot size, and pump flow rate.

---

## Board Compatibility

The code auto-detects your board:

```cpp
#if defined(ESP32)
  const int SOIL_PIN = 1;
  const int ADC_MAX  = 4095;   // 12-bit ADC
  const int RELAY_PIN = 12;
#else
  const int SOIL_PIN = A0;
  const int ADC_MAX  = 1023;   // 10-bit ADC
  const int RELAY_PIN = 7;
#endif
```

No manual changes needed when switching boards.

---

## Serial Monitor Output

Baud rate: `115200`

```
Auto Watering System Ready
[SENSOR] Raw: 3100 | Moisture: 24.3% → PUMP ON (soil dry)
[SENSOR] Raw: 3100 | Moisture: 24.3% [pumping...]
[PUMP] Auto-shutoff triggered.
[SENSOR] Raw: 1800 | Moisture: 56.0% [idle]
[SENSOR] Raw: 1200 | Moisture: 70.7% → PUMP OFF (soil wet enough)
```

---

## What's Next

This is Part 2 of a series:

| Part | Topic | Status |
|------|-------|--------|
| 1 | Why your sensor reads backwards + fix | ✅ Live |
| 2 | Connect sensor to relay + auto pump | ✅ Live |
| 3 | Add RTC + time-based watering schedule | 🔜 Coming |

---

## Author

**Penu Djira** — Agriculture Embedded Systems Engineer & Educator  
Kupang, NTT, Indonesia

- YouTube: [@PenuDjira](https://youtube.com/@PenuDjira)
- GitHub: [DeprisonArianto](https://github.com/PenuDjira)