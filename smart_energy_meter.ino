

#include <LiquidCrystal.h>

// LCD connections: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Analog input pins
#define VOLTAGE_PIN A0
#define CURRENT_PIN A1

// LED connections
#define GREEN_LED 2
#define RED_LED 3

// ADC settings
const float ADC_REFERENCE = 5.0;
const float ADC_MAX = 1023.0;

// Measurement settings
const float VOLTAGE_FACTOR = 2.0;
const float SHUNT_RESISTANCE = 100.0;

// Project settings
const float TARIFF = 8.0;
const float OVERLOAD_LIMIT = 0.04;

// Measured values
float voltage = 0.0;
float current = 0.0;
float power = 0.0;
float energy = 0.0;
float bill = 0.0;

unsigned long previousTime = 0;
unsigned long previousDisplay = 0;

int displayPage = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("SMART ENERGY");

  lcd.setCursor(0, 1);
  lcd.print("METER SYSTEM");

  delay(2000);

  previousTime = millis();
  previousDisplay = millis();
}

void loop()
{
  // Read analog inputs
  int voltageADC = analogRead(VOLTAGE_PIN);
  int currentADC = analogRead(CURRENT_PIN);

  // Convert ADC readings into voltage
  float voltageInput =
    voltageADC * ADC_REFERENCE / ADC_MAX;

  float currentInput =
    currentADC * ADC_REFERENCE / ADC_MAX;

  // Calculate actual voltage and current
  voltage = voltageInput * VOLTAGE_FACTOR;

  current = currentInput / SHUNT_RESISTANCE;

  // Calculate power
  power = voltage * current;

  // Calculate elapsed time in hours
  unsigned long currentTime = millis();

  float elapsedTime =
    (currentTime - previousTime) / 3600000.0;

  previousTime = currentTime;

  // Calculate energy in kWh
  energy += (power * elapsedTime) / 1000.0;

  // Calculate electricity bill
  bill = energy * TARIFF;

  // Overload detection
  if (current > OVERLOAD_LIMIT)
  {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }
  else
  {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }

  // Change LCD page every 2 seconds
  if (currentTime - previousDisplay >= 2000)
  {
    previousDisplay = currentTime;
    displayPage = 1 - displayPage;

    lcd.clear();

    if (displayPage == 0)
    {
      lcd.setCursor(0, 0);
      lcd.print("V:");
      lcd.print(voltage, 1);
      lcd.print("V I:");
      lcd.print(current, 3);

      lcd.setCursor(0, 1);
      lcd.print("Power:");
      lcd.print(power, 1);
      lcd.print("W");
    }
    else
    {
      lcd.setCursor(0, 0);
      lcd.print("Energy:");
      lcd.print(energy, 4);

      lcd.setCursor(0, 1);
      lcd.print("Bill:Rs");
      lcd.print(bill, 2);
    }
  }

  // Serial Monitor
  static unsigned long previousSerial = 0;

  if (currentTime - previousSerial >= 1000)
  {
    previousSerial = currentTime;

    Serial.println("--------------------");

    Serial.print("Voltage: ");
    Serial.print(voltage, 2);
    Serial.println(" V");

    Serial.print("Current: ");
    Serial.print(current, 4);
    Serial.println(" A");

    Serial.print("Power: ");
    Serial.print(power, 2);
    Serial.println(" W");

    Serial.print("Energy: ");
    Serial.print(energy, 6);
    Serial.println(" kWh");

    Serial.print("Electricity Bill: Rs ");
    Serial.println(bill, 4);

    Serial.print("Meter Status: ");

    if (current > OVERLOAD_LIMIT)
      Serial.println("OVERLOAD");
    else
      Serial.println("NORMAL");
  }
}
