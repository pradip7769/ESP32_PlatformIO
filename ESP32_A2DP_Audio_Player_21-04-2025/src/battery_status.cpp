#include "battery_status.h"
#include <Arduino.h>

// 2700 (min ADC value) → 0% battery

// 3700 (max ADC value) → 100% battery

// below 10 % battery low

int bat_analog_value = 0;
int bat_percentage = 0;

int bat_level()
{   
    bat_analog_value = analogRead(BAT_PIN);
    // Serial.print("Battery value : ");
    // Serial.println(bat_analog_value);

    bat_analog_value = constrain(bat_analog_value,2700,3700);

    bat_percentage = ((bat_analog_value - 2700) * 100) / (3700 - 2700);
    // Serial.print("Bat Percentage : ");
    // Serial.println(bat_percentage);

    return bat_percentage;
}