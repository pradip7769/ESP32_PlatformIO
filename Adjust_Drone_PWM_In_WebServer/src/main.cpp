
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "pwm_vari.h"

#define PWM_FREQUENCY      50     
#define PWM_CHANNEL_0       0
#define PWM_CHANNEL_1       1
#define PWM_CHANNEL_2       2
#define PWM_CHANNEL_3       3
#define PWM_RESOLUTION     16
#define PWM_OUTPUT_PIN_5    5
#define PWM_OUTPUT_PIN_18  18
#define PWM_OUTPUT_PIN_19  19
#define PWM_OUTPUT_PIN_21  21

const char* SSID = "Syslab OFC";
const char* PASS = "Syslab@123";

const char* PARAM_INPUT = "value";
AsyncWebServer server(80);

String sliderValue = "0";
//// Replace %SLIDERVALUE% in HTML
String processor(const String& var) {
  if (var == "SLIDERVALUE") {
    return sliderValue;
  }
  return String();
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Device start...");

  WiFi.begin(SSID,PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("IP Address : " + WiFi.localIP().toString());

  

  // Setup PWM channel
  ledcSetup(PWM_CHANNEL_0, PWM_FREQUENCY, PWM_RESOLUTION);
  // ledcSetup(PWM_CHANNEL_1, PWM_FREQUENCY, PWM_RESOLUTION);
  // ledcSetup(PWM_CHANNEL_2, PWM_FREQUENCY, PWM_RESOLUTION);
  // ledcSetup(PWM_CHANNEL_3, PWM_FREQUENCY, PWM_RESOLUTION);
  
  // // Attach PWM channel to GPIO pin
  ledcAttachPin(PWM_OUTPUT_PIN_5, PWM_CHANNEL_0);
  // ledcAttachPin(PWM_OUTPUT_PIN_18, PWM_CHANNEL_1);
  // ledcAttachPin(PWM_OUTPUT_PIN_19, PWM_CHANNEL_2);
  // ledcAttachPin(PWM_OUTPUT_PIN_21, PWM_CHANNEL_3);
   
  ledcWrite(PWM_CHANNEL_0, 0);

  // Set 25% duty cycle
  // uint32_t duty = (pow(2, PWM_RESOLUTION) - 1) * 0.50; // 25% duty
  // ledcWrite(PWM_CHANNEL_0, duty);
  // ledcWrite(PWM_CHANNEL_1, duty);
  // ledcWrite(PWM_CHANNEL_2, duty);
  // ledcWrite(PWM_CHANNEL_3, duty);

  server.on("/",HTTP_GET,[](AsyncWebServerRequest *request){
    request->send(200,"text/html",pwm_var_index,processor);
  });

    // Handle slider updates
    server.on("/slider", HTTP_GET, [](AsyncWebServerRequest *request) {
      if (request->hasParam(PARAM_INPUT)) {
        sliderValue = request->getParam(PARAM_INPUT)->value();
        Serial.print("Slider: ");
        Serial.println(sliderValue);
  
        // Convert slider value (0–100) to ESC duty (5%–10% of 65535)
        float percent = sliderValue.toFloat() / 100.0;
        uint32_t minDuty = 65535 * 0.05; // 1ms pulse
        uint32_t maxDuty = 65535 * 0.10; // 2ms pulse
        uint32_t duty = minDuty + percent * (maxDuty - minDuty);
  
        ledcWrite(PWM_CHANNEL_0, duty);
        Serial.printf("Mapped Duty = %u / 65535\n", duty);
      }
      request->send(200, "text/plain", "OK");
    });
  

  server.begin();
}

void loop()
{

   
}
