
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include "S_farmer.h"
#include "S_sd_file_download.h"

#define CS_PIN 5

AsyncWebServer server(80);
File myFile;


const char* SSID = "Syslab OFC";
const char* PASS = "Syslab@123";

void WIFI_Init();
void SD_Init();
String READ_SD_File();
void DELETE_File(AsyncWebServerRequest *request);
void DOWNLOD_File(AsyncWebServerRequest *request);
void Server_On();

void setup() 
{
  Serial.begin(115200);
  WIFI_Init();
  SD_Init();
  Server_On();
}

void loop() 
{
   delay(2000);
}

void WIFI_Init()
{

  WiFi.begin(SSID,PASS);
  while(WiFi.status() != WL_CONNECTED)
  {
     delay(500);
     Serial.print(".");
  }

  Serial.println("IP Address : " + WiFi.localIP().toString());

}

void SD_Init() 
{
  if (!SD.begin(CS_PIN))
  {
    Serial.println("SD card initialization failed!");
    return;
  }
  else
  {
    Serial.println("SD card initialization successfull...");
  }
}

String READ_SD_File()
{
   myFile = SD.open("/");

   String filePayload = "";  // Initialize outside loop

   while(true)
   {
     int numTabs = 0;
     File entry = myFile.openNextFile();

     if(!entry)
     {
      //  Serial.print("no more files...");
       break;  // Stop loop when no more files
     }
     else
     {
       for(uint8_t i = 0; i < numTabs; i++)
       {
          Serial.print("\n");
       }
       
       // Instead of overwriting, append each filename
       filePayload += "\"";
       filePayload += String(entry.name());
       filePayload += "\",";

      //  Serial.print(entry.name());
      //  Serial.print("\t");

       if(entry.isDirectory())
       {
        //  Serial.println("/");
         numTabs++;
       }
     }
     entry.close();
   }
  
   Serial.println();
   
   // Optional: Remove last comma if needed
   if (filePayload.endsWith(",")) {
     filePayload.remove(filePayload.length() - 1);
   }
  //  Serial.println(filePayload);
   return filePayload;
}

void DELETE_File(AsyncWebServerRequest *request)
{
   if(request->hasArg("file"))
   {
      String FileName = request->arg("file");
      Serial.println("FileName : " + FileName);

      if(SD.remove("/" + FileName))
      {
        Serial.println("File deleted Successfully : " + FileName);
        request->send(200, "text/plain", "File deleted Successfully");
      }
      else
      {
        Serial.println("Failed to delte File : " + FileName);
        request->send(400, "text/plain", "Failed to delte File");
      }
   }
   else
   {
      Serial.println("Missing File Parameter");
   }

}

void DOWNLOD_File(AsyncWebServerRequest *request)
{
   if (request->hasParam("file")) {
      String filename = request->getParam("file")->value();
      String path = "/" + filename;

      if (SD.exists(path)) {
        File file = SD.open(path, FILE_READ);
        if (file) {
          request->send(SD, path, String("application/octet-stream"), true);
          file.close();
          return;
        }
      }
      request->send(404, "text/plain", "File not found");
    } else {
      request->send(400, "text/plain", "Missing 'file' parameter");
    }
}

void Server_On()
{
   server.on("/",HTTP_GET,[](AsyncWebServerRequest* request){request->send(200,"text/html",farmer_client);});   // main page
   server.on("/restart_device",HTTP_GET,[](AsyncWebServerRequest* request){ESP.restart();});   // Restart Button is click to ESP is restart
   server.on("/S_sd_file_download",HTTP_GET,[](AsyncWebServerRequest* request){request->send(200,"text/html",sd_file_download);}); // Sensor Data View Page
   server.on("/sd_file",HTTP_GET,[](AsyncWebServerRequest* request){request->send(200,"application/json","[" + READ_SD_File() + "]");});
   server.on("/delete_file", HTTP_DELETE, [](AsyncWebServerRequest *request) {DELETE_File(request);});
   server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request){DOWNLOD_File(request);});

   server.begin();     // Server On
   Serial.println("HTTP Server started...");
 }
