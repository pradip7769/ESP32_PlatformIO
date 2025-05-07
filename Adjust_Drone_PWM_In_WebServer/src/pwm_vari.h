#ifndef PWM_VAR_H
#define PWM_VAR_H


// HTML Web Page stored in PROGMEM
const char pwm_var_index[] = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP32 Motor PWM</title>
    <style>
      html { font-family: Arial; display: inline-block; text-align: center; }
      h2 { font-size: 2.3rem; }
      p { font-size: 1.9rem; }
      body { max-width: 400px; margin: auto; padding-bottom: 25px; }
      .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C;
        outline: none; transition: opacity .2s; }
      .slider::-webkit-slider-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; }
      .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; }
    </style>
  </head>
  <body>
    <h2>ESP32 BLDC Motor Control</h2>
    <p><span id="textSliderValue">%SLIDERVALUE%</span></p>
    <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="100" value="%SLIDERVALUE%" step="1" class="slider"></p>
  <script>
  function updateSliderPWM(element) {
    var sliderValue = document.getElementById("pwmSlider").value;
    document.getElementById("textSliderValue").innerHTML = sliderValue;
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/slider?value=" + sliderValue, true);
    xhr.send();
  }
  </script>
  </body>
  </html>
  )rawliteral";

  #endif