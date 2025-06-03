#ifndef S_SERVER_H
#define S_SERVER_H

const char farmer_client[] =

R"=====(

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Farmer</title>
  <style>
    /* General styles */
    body {
      font-family: Arial, sans-serif;
      background-color: #f4f4f9;
      margin: 0;
      padding: 0;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }

    .container {
      background: #fff;
      border-radius: 10px;
      box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
      padding: 20px;
      width: 300px;
      text-align: center;
    }

    h2 {
      color: #333;
    }

    /* Form styling */
    .form-group {
      margin-bottom: 20px;
      text-align: left;
    }

    label {
      font-size: 14px;
      color: #555;
      display: block;
      margin-bottom: 5px;
    }

    input[type="text"] {
      width: 100%;
      padding: 10px;
      border: 1px solid #ccc;
      border-radius: 4px;
      box-sizing: border-box;
    }

    /* Button styling */
    .settings-btn {
      display: block;
      width: 100%;
      padding: 12px;
      margin: 10px 0;
      background: linear-gradient(145deg, #4CAF50, #3e8e41);
      color: #fff;
      border: none;
      border-radius: 8px;
      font-size: 18px;
      font-weight: bold;
      cursor: pointer;
      box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.3), inset 0px -2px 5px rgba(0, 0, 0, 0.2);
      transition: transform 0.2s ease, box-shadow 0.2s ease;
    }

    .settings-btn:hover {
      background: linear-gradient(145deg, #3e8e41, #4CAF50);
      box-shadow: 0px 5px 15px rgba(0, 0, 0, 0.4), inset 0px -3px 6px rgba(0, 0, 0, 0.3);
    }

    .settings-btn:active {
      transform: translateY(4px);
      box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.2), inset 0px -2px 5px rgba(0, 0, 0, 0.2);
    }

    /* Device Status container */
    .status-container {
      display: flex;
      align-items: center;
      justify-content: space-between;
      border: 2px solid #ddd;
      padding: 10px;
      border-radius: 8px;
      margin-top: 20px;
      background-color: #ddd;
      box-shadow: 0px 4px 6px rgba(0, 0, 0, 0.2);
      transition: background-color 0.3s ease, box-shadow 0.3s ease;
    }

    .status-label {
      font-size: 16px;
      font-weight: bold;
      color: #333;
      text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.2);
    }

    .status-text {
      font-size: 16px;
      font-weight: bold;
      color: #333;
    }

    /* 3D Slider styling */
    .switch {
      position: relative;
      display: inline-block;
      width: 60px;
      height: 30px;
    }

    .switch input {
      display: none;
    }

    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background: linear-gradient(145deg, #d32f2f, #c62828);
      border-radius: 20px;
      box-shadow: 0px 4px 6px rgba(0, 0, 0, 0.3), inset 0px -2px 4px rgba(0, 0, 0, 0.2);
      transition: 0.4s;
    }

    .slider:before {
      position: absolute;
      content: "";
      height: 20px;
      width: 20px;
      left: 5px;
      bottom: 5px;
      background: linear-gradient(145deg, #fff, #e0e0e0);
      border-radius: 50%;
      transition: 0.4s;
      box-shadow: 0px 4px 6px rgba(0, 0, 0, 0.3), inset 0px -2px 4px rgba(0, 0, 0, 0.2);
    }

    input:checked + .slider {
      background: linear-gradient(145deg, #4CAF50, #388E3C);
    }

    input:checked + .slider:before {
      transform: translateX(30px);
      background: linear-gradient(145deg, #e0ffe0, #b0ffc0);
    }

    input:checked ~ .status-container {
      background-color: #4CAF50; /* Green when ON */
    }

    input:not(:checked) ~ .status-container {
      background-color: #d32f2f; /* Red when OFF */
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>Device id</h2>

    <!-- Buttons -->
    <button class="settings-btn" onclick="device_configuratin()">Device Configuration</button>
    <button class="settings-btn" onclick="view_data()">View Data</button>
    <button class="settings-btn" onclick="sd_file_download()">File Download</button>
    <button class="settings-btn" onclick="Restart_device()">Restart Device</button>
  </div>

  <script>

    // Save Settings
    function device_configuratin() 
    {
        window.location.href = '/S_device_configuration';
    }

    function view_data() 
    {
         window.location.href = '/S_view_data';
    }
    
    function sd_file_download()
    {
        window.location.href = '/S_sd_file_download';
    }
    
    function Restart_device() 
    {
       
     const queryString = `/restart_device`;
      fetch(queryString, {
        method: 'GET'
      })
        .then(response => {
          if (response.ok) {
            alert('Device is normal mode...');
          } else {
            alert('Device is AP mode...');
          }
        })
        .catch(error => console.error('Error:', error));
    }

  </script>
</body>
</html>

)=====";
#endif