#include <Arduino.h>
#include "BluetoothA2DPSource.h"
#include <SPI.h>
#include <SD.h>
#include "files.h"
#include "battery_status.h"

const char* Bluetooth_name = "Ambrane Evoke Beam";

#define BAT_LEVEL 10  // play Battery LOW

#define DATA_PIN 15
#define clk 4
#define strobe 17

#define latchswitch 16
#define latchingsignal 21
#define Enable_1401 2 
#define Enable32 32   //speaker and mic high bit
#define RECORD_LED    12 
#define PLAY_CON_BUT 35
#define BUTTON3   34
#define sd_ss_pin  5



 BluetoothA2DPSource a2dp_source;
 File sound_file;
 
int button3;
int databyte[4];

int play_con_but_sta_2 = 1;
int play_con_but_sta_3 = 1;
bool isPlaying = false;
bool endOfFileReached = false;
bool bat_play_once = false;


const int mono_frame_size_bytes = sizeof(int16_t);
const int stereo_frame_size_bytes = sizeof(int16_t) * 2;
float volume_factor = 0.5; // Volume control (0.0 = mute, 1.0 = max volume)
float playback_speed = 0.55; // Playback speed (1.0 = normal, <1.0 = slower, >1.0 = faster)

 int bitno,byteno, num2;
 int bytecount = 4;
 bool isPlaybackStopped = false;
 bool isLatchSwitchHigh = false;
 unsigned long latchSwitchHighTime = 0;
 const unsigned long LATCH_SWITCH_DURATION = 3000;

 bool a2dp_connected = false;
 bool file_is_not_available_condition = false;

 struct WavHeader {
  char riff[4];                // "RIFF"
  uint32_t chunkSize;          // RIFF chunk size
  char wave[4];                // "WAVE"
  char fmt[4];                 // "fmt "
  uint32_t subchunk1Size;      // Size of the fmt chunk
  uint16_t audioFormat;        // Audio format (1 for PCM)
  uint16_t numChannels;        // Number of channels
  uint32_t sampleRate;         // Sampling rate
  uint32_t byteRate;           // Byte rate
  uint16_t blockAlign;         // Block align
  uint16_t bitsPerSample;      // Bits per sample
  char subchunk2ID[4];         // "data"
  uint32_t subchunk2Size;      // Size of the data chunk
};

WavHeader header;

int32_t get_sound_data(Frame* data, int32_t len);
void read_1401();
void rd_dt();
void GENSCL_1();
void playFiles(const char* fileArray[], int arraySize);
void play_bat_level();

 int32_t get_sound_data(Frame* data, int32_t len) 
 {
  if (!sound_file || !isPlaying) {
    for (int i = 0; i < len; i++) {
      data[i].channel1 = 0;  // Assign values correctly
      data[i].channel2 = 0;
    }
    return len;
  }
  int32_t mono_len = (int32_t)(len * playback_speed) + 1; // Ensure mono_len is large enough
  int16_t mono_data[mono_len];
  size_t result_len_bytes = sound_file.read((uint8_t*)mono_data, mono_len * mono_frame_size_bytes);
  int32_t result_len = result_len_bytes / mono_frame_size_bytes;
  // If end of file reached
  if (result_len < mono_len) {
    endOfFileReached = true;
    // endOfFileReached = true;
    isPlaying = false;
    // Serial.println("End of file reached, stopping playback.");
    for (int i = 0; i < len; i++) {
      data[i].channel1 = 0;
      data[i].channel2 = 0;
    }
    return len;
  }
  // Adjust playback speed within the read data
  for (int i = 0; i < len; i++) {
    float index = (float)i * playback_speed; // Adjust index calculation
    int index_int = (int)index;
    float frac = index - index_int;
    if (index_int + 1 < result_len) {
      int16_t sample = (int16_t)((1.0f - frac) * mono_data[index_int] + frac * mono_data[index_int + 1]);
      int16_t adjusted_sample = (int16_t)(sample * volume_factor);
      data[i].channel1 = adjusted_sample;
      data[i].channel2 = adjusted_sample;
    } else {
      // If index exceeds the available data, fill with silence
      data[i].channel1 = 0;
      data[i].channel2 = 0;
    }
  }
  return len; // Returning the number of stereo frames processed
}

void read_1401()
{
  
  digitalWrite(strobe, HIGH);   // Strobe output pin
  delay(1);
  GENSCL_1();
  digitalWrite(strobe, LOW);   // Strobe pin low
  delay(5);
  for(byteno =0; byteno<bytecount; byteno++){
    num2 = 0;
    for (bitno = 0; bitno < 8; bitno++) {
    if (digitalRead(DATA_PIN) == 1) {
      num2 = num2 | 0x01; // DATA_PIN     
    }
    if (bitno < 7) {
      num2 = num2 << 1;
    }
    GENSCL_1();
    delay(5);
  }
    databyte[byteno]=num2;
  }
  Serial.print("card : ");

  for(int i=0;i<bytecount;i++)
  {
     Serial.print(databyte[i],HEX);
     Serial.print(":");
  }
  Serial.println();

}

void rd_dt() 
{

  play_con_but_sta_2 = digitalRead(PLAY_CON_BUT);
  play_con_but_sta_3 = digitalRead(BUTTON3);

  if((play_con_but_sta_2 == LOW) || (play_con_but_sta_3 == LOW))
  {
    // Serial.println("play_con_but_sta_2 : " + String(play_con_but_sta_2));
    // Serial.println("play_con_but_sta_3 : " + String(play_con_but_sta_3));

    digitalWrite(Enable_1401, HIGH);
    read_1401();
    digitalWrite(Enable_1401, LOW);
  }
  
  // if((play_con_but_sta_2 == LOW) || (play_con_but_sta_3 == LOW))
  // {
    // Serial.println("play_con_but_sta_2 : " + String(play_con_but_sta_2));
    // Serial.println("play_con_but_sta_3 : " + String(play_con_but_sta_3));

    // digitalWrite(Enable_1401, HIGH);
    // delay(100);
    read_1401();
  //   digitalWrite(Enable_1401, LOW);
  // }
  
 
      if(((databyte[2] == 0x83) && (databyte[1] == 0x81) && (databyte[0] == 0x94)) && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&CAT[1],arraysizeCAT - 1);
        }
      }else if(((databyte[2] == 0x83) && (databyte[1] == 0x81) && (databyte[0] == 0x94)) && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&CAT[0],1);
        }
      }else if(((databyte[2] == 0x82) && (databyte[1] == 0x81) && (databyte[0] == 0x94))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          // playFiles(BAT,arraysizeBAT);
          playFiles(&BAT[1], arraysizeBAT - 1);
        }
      }else if(((databyte[2] == 0x82) && (databyte[1] == 0x81) && (databyte[0] == 0x94))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&BAT[0],1);
        }
      }else if(((databyte[3] == 0x8E) && (databyte[2] == 0x8F) && (databyte[1] == 0x93) && (databyte[0] == 0x85))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&NOSE[1], arraysizeNOSE - 1);
        }
      }else if(((databyte[3] == 0x8E) && (databyte[2] == 0x8F) && (databyte[1] == 0x93) && (databyte[0] == 0x85))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&NOSE[0],1);
        }
      }else if(((databyte[2] == 0xFF) && (databyte[1] == 0x8F) && (databyte[0] == 0x87))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&DOG[1], arraysizeDOG - 1);
        }
      }else if(((databyte[2] == 0xFF) && (databyte[1] == 0x8F) && (databyte[0] == 0x87))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&DOG[0],1);
        }
      }else if(((databyte[3] == 0x87) && (databyte[2] == 0x8F) && (databyte[1] == 0x81) && (databyte[0] == 0x94))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&GOAT[1], arraysizeGOAT - 1);
        }
      }else if(((databyte[3] == 0x87) && (databyte[2] == 0x8F) && (databyte[1] == 0x81) && (databyte[0] == 0x94))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&GOAT[0],1);
        }
      }else if(((databyte[2] == 0xFF) && (databyte[1] == 0x81) && (databyte[0] == 0x94))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&AT[1], arraysizeAT - 1);
        }
      }else if(((databyte[2] == 0xFF) && (databyte[1] == 0x81) && (databyte[0] == 0x94))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&AT[0],1);
        }
      }else if(((databyte[2] == 0xFF) && (databyte[1] == 0x81) && (databyte[0] == 0x8E))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&AN[1], arraysizeAN - 1);
        }
      }else if(((databyte[2] == 0xFF) && (databyte[1] == 0x81) && (databyte[0] == 0x8E))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&AN[0],1);
        }
      }else if(((databyte[2] == 0x93) && (databyte[1] == 0x81) && (databyte[0] == 0x90))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&SAP[1], arraysizeSAP - 1);
        }
      }else if(((databyte[2] == 0x93) && (databyte[1] == 0x81) && (databyte[0] == 0x90))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&SAP[0],1);
        }
      }else if(((databyte[2] == 0x94) && (databyte[1] == 0x81) && (databyte[0] == 0x90))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&TAP[1], arraysizeTAP - 1);
        }
      }else if(((databyte[2] == 0x94) && (databyte[1] == 0x81) && (databyte[0] == 0x90))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&TAP[0],1);
        }
      }else if(((databyte[2] == 0x90) && (databyte[1] == 0x81) && (databyte[0] == 0x94))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&PAT[1], arraysizePAT - 1);
        }
      }else if(((databyte[2] == 0x90) && (databyte[1] == 0x81) && (databyte[0] == 0x94))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&PAT[0],1);
        }
      }else if(((databyte[2] == 0x90) && (databyte[1] == 0x81) && (databyte[0] == 0x8E))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&PAN[1], arraysizePAN - 1);
        }
      }else if(((databyte[2] == 0x90) && (databyte[1] == 0x81) && (databyte[0] == 0x8E))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&PAN[0],1);
        }
      }else if(((databyte[2] == 0x8E) && (databyte[1] == 0x81) && (databyte[0] == 0x90))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&NAP[1], arraysizeNAP - 1);
        }
      }else if(((databyte[2] == 0x8E) && (databyte[1] == 0x81) && (databyte[0] == 0x90))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&NAP[0],1);
        }
      }else if(((databyte[2] == 0x93) && (databyte[1] == 0x81) && (databyte[0] == 0x94))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&SAT[1], arraysizeSAT - 1);
        }
      }else if(((databyte[2] == 0x93) && (databyte[1] == 0x81) && (databyte[0] == 0x94))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&SAT[0],1);
        }
      }else if(((databyte[2] == 0x94) && (databyte[1] == 0x81) && (databyte[0] == 0x8E))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&TAN[1], arraysizeTAN - 1);
        }
      }else if(((databyte[2] == 0x94) && (databyte[1] == 0x81) && (databyte[0] == 0x8E))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&TAN[0],1);
        }
      }else if(((databyte[2] == 0x8E) && (databyte[1] == 0x81) && (databyte[0] == 0x94))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&NAT[1], arraysizeNAT - 1);
        }
      }else if(((databyte[2] == 0x8E) && (databyte[1] == 0x81) && (databyte[0] == 0x94))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&NAT[0],1);
        }
      }else if(((databyte[2] == 0x93) && (databyte[1] == 0x8E) && (databyte[0] == 0x90))  && (play_con_but_sta_2 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          Serial.println("Starting playback...");   
          playFiles(&SNP[1], arraysizeNAT - 1);
        }
      }else if(((databyte[2] == 0x93) && (databyte[1] == 0x8E) && (databyte[0] == 0x90))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&SNP[0],1);
        }
      }
      else if(((databyte[2] == 0x8E) && (databyte[1] == 0x90) && (databyte[0] == 0x81))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&NPA[0],1);
        }
      }else if(((databyte[2] == 0x81) && (databyte[1] == 0x90) && (databyte[0] == 0x8E))  && (play_con_but_sta_3 == LOW)){
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(&APJ[0],1);
        }
      }else if(((play_con_but_sta_3 == LOW) || (play_con_but_sta_2 == LOW)) && file_is_not_available_condition)
      {
        if(!isPlaying){
          Serial.println("Starting playback...");
          playFiles(File_is_not_available, arrayFile_is_not_available);
        }
      }

      file_is_not_available_condition = true;

  delay(10); // Small delay to avoid flooding the serial output

 }

 void GENSCL_1() 
 {
  delayMicroseconds(500);  
  digitalWrite(clk, HIGH);
  delayMicroseconds(500);  
  digitalWrite(clk, LOW);
  delayMicroseconds(500);  
 }
 
// void playFiles(const char* fileArray[], int arraySize)
void playFiles(const char **fileArray, int arraySize)
{
  for (int i = 0; i < arraySize; i++) 
  {
    // if(a2dp_source.is_connected())    
    // {
      String filePath = "/";
      filePath += fileArray[i];
      
      if (SD.exists(filePath.c_str())) 
      {
        sound_file = SD.open(filePath.c_str(), FILE_READ);
        if (!sound_file) {
          Serial.println("Failed to open file: " + String(fileArray[i]));
          continue;
        }

        if (sound_file.read((uint8_t*)&header, sizeof(WavHeader)) != sizeof(WavHeader)) {
          Serial.println("Failed to read WAV header for file: " + String(fileArray[i]));
          sound_file.close();
          continue;
        }

        if (strncmp(header.riff, "RIFF", 4) != 0 || strncmp(header.wave, "WAVE", 4) != 0 || strncmp(header.fmt, "fmt ", 4) != 0) {
          Serial.println("Invalid WAV file format for file: " + String(fileArray[i]));
          sound_file.close();
          continue;
        }

        // Ensure the WAV file is 16-bit PCM
        if (header.audioFormat != 1 || header.bitsPerSample != 16) 
        {
          Serial.println("Unsupported WAV file format for file: " + String(fileArray[i]));
          sound_file.close();
          continue;
        }  

        Serial.println("Playing file: " + String(fileArray[i]));
        endOfFileReached = false;
        isPlaying = true;

        while (isPlaying && !endOfFileReached) {
          delay(10);  // Adjust delay as needed
        }

        sound_file.close();
        // endOfFileReached = false;
        isPlaying = true;

        while (isPlaying)
        {
          delay(1);  // Adjust delay as needed
        }

        sound_file.close();
        // Serial.println("Sound file close...");
      } else {
        Serial.println("File does not exist: " + String(fileArray[i]));
      }
  //  }
 }
}

void play_bat_level(int value)
{
  int file_1_value = value;

  String file_1 = "";
  String file_2 = "";

  const char* file_name[2];

  file_1 = String("Battery Percentage is") + ".wav";
  file_2 = String(file_1_value) + ".wav";

  file_name[0] = file_1.c_str();
  file_name[1] = file_2.c_str();

  Serial.printf("\nfile_1 : %s ", file_name[0]);
  Serial.printf("\nfile_2 : %s \n", file_name[1]);

  playFiles(file_name,2);
   
  // delay(100);
}

 void GPIO_Init()
 {
  pinMode(BAT_PIN,INPUT);
  pinMode(RECORD_LED, OUTPUT);
  pinMode(PLAY_CON_BUT, INPUT);
  pinMode(BUTTON3,INPUT);
  digitalWrite(RECORD_LED, HIGH);
  pinMode(DATA_PIN, INPUT);
  pinMode(clk, OUTPUT);
  pinMode(strobe, OUTPUT);
  pinMode(strobe, OUTPUT); 
  pinMode(Enable_1401,OUTPUT);
  pinMode(Enable32,OUTPUT);
  pinMode(latchswitch, INPUT);
  pinMode(latchingsignal, OUTPUT);
  digitalWrite(latchingsignal, HIGH); 

  digitalWrite(Enable_1401, HIGH);
  digitalWrite(Enable32, HIGH);

 }
  
void setup()
{
  Serial.begin(115200);
  delay(1000);

  GPIO_Init();

  if (!SD.begin(sd_ss_pin)) 
  {
    Serial.println("Failed to initialize SD card");
    return;
  }
  else
  {
    Serial.println("SD card initialized succesfull...");
  }

  Serial.println("Starting A2DP...");
  a2dp_source.set_auto_reconnect(true);
  a2dp_source.start(Bluetooth_name, (music_data_frames_cb_t)get_sound_data);
  Serial.println("Connecting to : " + String(Bluetooth_name) + "...");
  file_is_not_available_condition = false;

}

void loop()
{
 
  while(!a2dp_source.is_connected())
  {
    bat_play_once = false;
    a2dp_connected = false;
    file_is_not_available_condition = false;
    Serial.println("Starting A2DP...");
    a2dp_source.set_auto_reconnect(true);
    a2dp_source.start(Bluetooth_name, (music_data_frames_cb_t)get_sound_data);
    Serial.println("Connecting to : " + String(Bluetooth_name) + "...");
    delay(1000);
  }

  if(a2dp_source.is_connected() && !a2dp_connected)
  {
    delay(4000);
    // delay(1000);
    file_is_not_available_condition = false;
    a2dp_connected = true;
    bat_play_once = false;
  }

  if(a2dp_source.is_connected())
  {
    if(!bat_play_once)
    {
      
      bat_play_once = true;
      play_bat_level(bat_level());
      file_is_not_available_condition = false;
    }
  }
  
  if(bat_level() <= BAT_LEVEL)
  {
    playFiles(Battery_LOW,1);
  }

  rd_dt();   //read DATA_PIN
  // digitalWrite(Enable_1401, HIGH);
  digitalWrite(Enable32, HIGH);

  int latchswitchread = digitalRead(latchswitch);

  //  Serial.println("latchswitchread : " + String(latchswitchread));
   
   if (latchswitchread != isLatchSwitchHigh) {
     isLatchSwitchHigh = latchswitchread;

      if (isLatchSwitchHigh) {
        if (!isPlaybackStopped) {
          isPlaying = false;
          isPlaybackStopped = true;
          latchSwitchHighTime = millis();
        }
      } else {
         if (isPlaybackStopped) {
         isPlaybackStopped = false;

         unsigned long elapsedTime = millis() - latchSwitchHighTime;
         if (elapsedTime >= LATCH_SWITCH_DURATION) 
         {
            playFiles(POWER_OFF,1);
            digitalWrite(latchingsignal, LOW);
            Serial.println("Setting latchingsignal to LOW...");
            Serial.print("latchingsignal : ");
            Serial.println(digitalRead(latchingsignal));

            latchSwitchHighTime = 0; // Reset the timer
         }
         } else {
          latchSwitchHighTime = 0; // Reset the timer
         }
        }
   } else if (isLatchSwitchHigh) {
    // Continue checking the duCARion if the latch switch is still HIGH
   if (latchswitchread != isLatchSwitchHigh) 
   {
     isLatchSwitchHigh = latchswitchread;
   } 
   else if (isLatchSwitchHigh)
    {
     unsigned long elapsedTime = millis() - latchSwitchHighTime;
     if (elapsedTime >= LATCH_SWITCH_DURATION) 
     {
        playFiles(POWER_OFF,1);
        digitalWrite(latchingsignal, LOW);
        Serial.println("Setting latchingsignal to LOW...");
        Serial.print("latchingsignal : ");
        Serial.println(digitalRead(latchingsignal));

        latchSwitchHighTime = 0; // Reset the timer
    }
  }

 }
 delay(100); 
}