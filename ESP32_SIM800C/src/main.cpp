#include <Arduino.h>
#include <HardwareSerial.h>   

#define RX_PIN 16
#define TX_PIN 17
#define PWRKEY 2
#define GSM_PWR 15

HardwareSerial MySim800c(2);

String GSM_Response = "";
String R_GSM_F = "";   // Received GSM_Function Return for checking
bool gsm_flag = true;
bool init_over = false;

// unsigned long previousMillis = 0;
// unsigned long currentMillis = 0;
// const unsigned long interval = 600000;
// bool first_data_send = true;
// String cloud_data_send_ret = "";


void GSM_PWRKEY()
{
  // Serial.println("GSM_PWRKEY");
  digitalWrite(GSM_PWR,LOW);
  delay(100);
  digitalWrite(PWRKEY,HIGH);
  delay(3000);
  digitalWrite(PWRKEY,LOW);
  // Serial.println("GSM_PWRKEY");

}

void GSM_TX_Write(String cmd)
{
   MySim800c.print(cmd); 
}

String GSM_RX_Read()
{
   String data = "";
   unsigned long startTime = millis();
   int duration = 10000;
   while(millis() - startTime < duration)
   {
       if(MySim800c.available() > 0)
       {
         data = MySim800c.readStringUntil('\n');
         data.trim();
         Serial.println("Data : " + data);
         if(strcmp(data.c_str(),"OK") == 0 || strcmp(data.c_str(),"DOWNLOAD") == 0)
         {
            break;
         }
       }
   }
 
    return data;
}

String GSM_HTTPACTION_Read()
{  
   String httpaction = "";
   String data = "";
   unsigned long startTime = millis();
   int duration = 10000;
   while(millis() - startTime < duration)
   {
       if(MySim800c.available() > 0)
       {
         data = MySim800c.readStringUntil('\n');
         data.trim();
         Serial.println("Data : " + data);
         if(strcmp(data.c_str(),"+HTTPACTION: 1,200,14") == 0)
         {
            httpaction = data.substring(15,18);
            Serial.println("httpaction : " + httpaction);
            break;
         }
       }
   }
 
    return httpaction;
}

String GSM_AT()
{
  GSM_TX_Write("AT\r\n");
  GSM_Response = GSM_RX_Read();
  Serial.println("AT : " + GSM_Response);

  return GSM_Response;
}

String GSM_AT_CSPN()
{
  GSM_Response.clear();
  GSM_TX_Write("AT+CSPN?\r\n");
  GSM_Response = GSM_RX_Read();
  Serial.println("_ATCSPN: " + GSM_Response);

  return GSM_Response;
}

String AT_GSN()
{
  GSM_Response.clear();
  GSM_TX_Write("AT+GSN\r\n");
  GSM_Response = GSM_RX_Read();
  Serial.println("AT+GSN: " + GSM_Response);

  return GSM_Response;
}

String AT_SAPBR()
{
  GSM_Response.clear();
  GSM_TX_Write("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n");
  GSM_Response = GSM_RX_Read();
  Serial.println("AT+SAPBR: " + GSM_Response);

  return GSM_Response;
}

String AT_SAPBR_APN()
{
  GSM_Response.clear();
  GSM_TX_Write("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"\r\n");
  GSM_Response = GSM_RX_Read();
  Serial.println("AT+SAPBR=3,1 : " + GSM_Response);

  return GSM_Response;
}

String AT_SAPBR_1()
{
  GSM_Response.clear();
  GSM_TX_Write("AT+SAPBR=1,1\r\n");
  GSM_Response = GSM_RX_Read();
  Serial.println("AT+SAPBR=1,1 : " + GSM_Response);

  return GSM_Response;
}

String AT_SAPBR_2()
{
  GSM_Response.clear();
  GSM_TX_Write("AT+SAPBR=2,1\r\n");
  GSM_Response = GSM_RX_Read();

  if(strcmp(GSM_Response.c_str(),"OK") == 0)
  {
    Serial.println("AT+SAPBR=2: " + GSM_Response);
    init_over = true;
  }

  return GSM_Response;
}

String AT_HTTPINIT()
{
  GSM_Response.clear();
  GSM_TX_Write("AT+HTTPINIT\r\n");
  GSM_Response = GSM_RX_Read();
  Serial.println("AT+HTTPINIT: " + GSM_Response);

  return GSM_Response;
}

String AT_HTTPPARA()
{
 GSM_Response.clear();
  GSM_TX_Write("AT+HTTPPARA=\"CID\",1\r\n");
  GSM_Response = GSM_RX_Read();
  Serial.println("AT+HTTPPARA: " + GSM_Response);

  return GSM_Response;
}

String AT_HTTPPARA_URL()
{
  GSM_Response.clear();
  GSM_TX_Write("AT+HTTPPARA=\"URL\",\"http://ec2-13-233-121-3.ap-south-1.compute.amazonaws.com:8082/devices/putDeviceData\"\r\n");
  GSM_Response = GSM_RX_Read();
  Serial.println("AT+HTTPPARA= URL: " + GSM_Response);

  return GSM_Response;
}

String AT_HTTPPARA_CONTENT()
{
  GSM_Response.clear();
  GSM_TX_Write("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n");
  GSM_Response = GSM_RX_Read();
  Serial.println("AT+HTTPPARA= CONTENT: " + GSM_Response);

  return GSM_Response;
}

String AT_HTTPDATA_LENGTH()
{
  GSM_Response.clear();
  GSM_TX_Write("AT+HTTPDATA=188,90000\r\n");
  GSM_Response = GSM_RX_Read();
  Serial.println("AT+HTTPDATA: " + GSM_Response);

  return GSM_Response;
}

String GSM_DATA_SEND()
{
   String temp_data = "{\"Imei\":\"868064077342282\",\"Date\":\"24-05-25\",\"Time\":\"12:18\",\"Latitude\":\"00.0000000\",\"Longitude\":\"00.000000\",\"PM10\":\"49\",\"PM2.5\":\"46\",\"PM1\":\"29\",\"Temp\":\"36.1\",\"Humi\":\"23.5\",\"Bat_sts\":\"80\"}\r\n";
  //  Serial.println(temp_data.length());

    GSM_Response.clear();
    GSM_TX_Write(temp_data);

    GSM_Response = GSM_RX_Read();
    Serial.println("temp_data : " + GSM_Response);  

  return GSM_Response;
}

String AT_HTTPACTION()
{
    GSM_Response.clear();
    GSM_TX_Write("AT+HTTPACTION=1\r\n");
    GSM_Response = GSM_HTTPACTION_Read();
    Serial.println("AT+HTTPACTION=1 : " + GSM_Response);   

  return GSM_Response;
}

String AT_HTTPTERM()
{
   GSM_Response.clear();
    GSM_TX_Write("AT+HTTPTERM\r\n");
    GSM_Response = GSM_RX_Read();
    Serial.println("AT+HTTPTERM : " + GSM_Response);   

   if(strcmp(GSM_Response.c_str(),"OK") == 0)
   {
    Serial.println("-------------------Termination end-----------------------------------------------------");
   }
  return GSM_Response;
}

void RGSM_ERROR(String error)
{
    if (strcmp(error.c_str(),"ERROR") == 0)
    {
      gsm_flag = true;
      GSM_PWRKEY();
    }

}

String GSM_Data_Send()
{
  if (gsm_flag)
  {
    R_GSM_F = GSM_AT();

      delay(2000);

    if(strcmp(R_GSM_F.c_str(),"OK") == 0)
    {
      R_GSM_F.clear();
      R_GSM_F = GSM_AT_CSPN();
    }
    else if (strcmp(R_GSM_F.c_str(),"ERROR") == 0)
    {
      GSM_PWRKEY();
      return "ERROR";
    }

    delay(100);

    if(strcmp(R_GSM_F.c_str(),"OK") == 0)
    {
      R_GSM_F.clear();
      R_GSM_F = AT_GSN();
    }
    else if (strcmp(R_GSM_F.c_str(),"ERROR") == 0)
    {
      GSM_PWRKEY();
      return "ERROR";
    }

    delay(100);

    if(strcmp(R_GSM_F.c_str(),"OK") == 0)
    {
      R_GSM_F.clear();
      R_GSM_F = AT_SAPBR();
    }
    else if (strcmp(R_GSM_F.c_str(),"ERROR") == 0)
    {
      GSM_PWRKEY();
       return "ERROR";
    }

    delay(100);

    if(strcmp(R_GSM_F.c_str(),"OK") == 0)
    {
      R_GSM_F.clear();
      R_GSM_F = AT_SAPBR_APN();
    }
    else if (strcmp(R_GSM_F.c_str(),"ERROR") == 0)
    {
      GSM_PWRKEY();
       return "ERROR";
    }

    delay(100);

    if(strcmp(R_GSM_F.c_str(),"OK") == 0)
    {
      R_GSM_F.clear();
      R_GSM_F = AT_SAPBR_1();
    }
    else if (strcmp(R_GSM_F.c_str(),"ERROR") == 0)
    {
      GSM_PWRKEY();
       return "ERROR";
    }

    delay(100);

    if(strcmp(R_GSM_F.c_str(),"OK") == 0)
    {
      R_GSM_F.clear();
      R_GSM_F = AT_SAPBR_2();
    }
    else if (strcmp(R_GSM_F.c_str(),"ERROR") == 0)
    {
      GSM_PWRKEY();
       return "ERROR";
    }

    delay(100);
  }

  if(init_over)
  {
    R_GSM_F.clear();
    R_GSM_F = AT_HTTPINIT();
  }

  delay(100);

  if(strcmp(R_GSM_F.c_str(),"OK") == 0)
  {
    R_GSM_F.clear();
    R_GSM_F = AT_HTTPPARA();
  }
  else if (strcmp(R_GSM_F.c_str(),"ERROR") == 0)
  {
    GSM_PWRKEY();
     return "ERROR";
  }

  delay(100);

  if(strcmp(R_GSM_F.c_str(),"OK") == 0)
  {
    R_GSM_F.clear();
    R_GSM_F = AT_HTTPPARA_URL();
  }
  else if (strcmp(R_GSM_F.c_str(),"ERROR") == 0)
  {
    GSM_PWRKEY();
     return "ERROR";
  }

  delay(100);

   if(strcmp(R_GSM_F.c_str(),"OK") == 0)
   {
     R_GSM_F.clear();
     R_GSM_F = AT_HTTPPARA_CONTENT();
   }
   else if (strcmp(R_GSM_F.c_str(),"ERROR") == 0)
   {
    GSM_PWRKEY();
    return "ERROR";
   }

   delay(100);

   if(strcmp(R_GSM_F.c_str(),"OK") == 0)
   {
     R_GSM_F.clear();
     R_GSM_F = AT_HTTPDATA_LENGTH();
   }
   else if (strcmp(R_GSM_F.c_str(),"ERROR") == 0)
   {
    GSM_PWRKEY();
    return "ERROR";
   }

   delay(100);

   if(strcmp(R_GSM_F.c_str(),"DOWNLOAD") == 0)
   {
     R_GSM_F.clear();
     R_GSM_F = GSM_DATA_SEND();
   }
   else if (strcmp(R_GSM_F.c_str(),"ERROR") == 0)
   {
    GSM_PWRKEY();
    return "ERROR";
   }

   delay(100);

   if(strcmp(R_GSM_F.c_str(),"OK") == 0)
   {
     R_GSM_F.clear();
     R_GSM_F = AT_HTTPACTION();
   }
   else if (strcmp(R_GSM_F.c_str(),"ERROR") == 0)
   {
    GSM_PWRKEY();
    return "ERROR";
   }

   delay(100);

   if(strcmp(GSM_Response.c_str(),"200") == 0)
   {
     R_GSM_F = AT_HTTPTERM();
     gsm_flag = false;
     return R_GSM_F;
   }

  delay(100);
  return "";
}


void setup()
{
  Serial.begin(115200);
  Serial.println("Serial");
  MySim800c.begin(9600,SERIAL_8N1,RX_PIN,TX_PIN);
  Serial.println("MySim800c");
  pinMode(PWRKEY,OUTPUT);
  pinMode(GSM_PWR,OUTPUT);
  GSM_PWRKEY();
  Serial.println("Device start...");
  // first_data_send = true;

}

void loop()
{

  GSM_Data_Send();
  delay(1000);
  
  // currentMillis = millis();
  // if(currentMillis - previousMillis >= interval)
  // {
  //   previousMillis = currentMillis;
  //   while (1)
  //   {
  //     cloud_data_send_ret = GSM_Data_Send();
  //     if(strcmp(cloud_data_send_ret.c_str(),"OK") == 0)
  //     {
  //       break;
  //     }
  //   }
    
  //   Serial.println("Data send successfull");

  // }
  // else if(first_data_send)
  // {
  //   first_data_send = false;
  //   cloud_data_send_ret = GSM_Data_Send();
  //   if(strcmp(cloud_data_send_ret.c_str(),"OK") != 0)
  //   {
  //     first_data_send = true;
  //   }
  // }
 
}

