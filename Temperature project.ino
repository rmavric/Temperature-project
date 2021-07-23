#include "DHT.h"
#include <ESP8266WiFi.h>
#include <NTPtimeESP.h>

NTPtime NTPhr("hr.pool.ntp.org");   //Choose server pool as required
strDateTime dateTime;

const char* ssid = "xxxxxxxxxx"; //SECRET_SSID; //Enter SSID
const char* password = "xxxxxxxxxx" ; //SECRET_PASS; //Enter Password 
char host[] = "xx.xxx.xxx.xxx"; //IPv4 address
const int port = 8080 ;   //port on which server listens

#define DHTTYPE DHT11
#define dht_dpin D4
DHT dht(dht_dpin, DHTTYPE);
WiFiClient client;    //use for TCP connections


void setup() {
    Serial.begin(9600);
    dht.begin();

    WiFi.begin(ssid, password);   // Connect to WiFi

    while(WiFi.waitForConnectResult() != WL_CONNECTED) //while(WiFi.status() != WL_CONNECTED)
    {
      delay(100);
      if(millis()>25000){         
        break;
      }
    }

    if(WiFi.status() != WL_CONNECTED)
  {
    while(millis()<40000)
    {
      delay(10);
    }
    digitalWrite(12, HIGH);
    delay(500);
    ESP.deepSleep(191850e4);   //379932e4
  }

  while(!(dateTime = NTPhr.getNTPtime(1.0, 1)).valid)
  {
    delay(100);
  }

  while(millis()<30000)
  {
    delay(10);
  }
  
  String timeAndDate = timeCalculation(dateTime);

  while(millis()<40000)
  {
    delay(10);
  }

  String measurements = temperatureMeasurement();

  String finalString = timeAndDate + measurements;

  Serial.println(finalString);

  if(client.connect(host, port))
  {
    client.print(finalString); 
    delay(300);
    client.stop();
  }
  
  else{
    delay(300);
    ESP.deepSleep(191850e4);   //379932e4
  }

  delay(300);

  ESP.deepSleep(191850e4);    //379932e4
}

void loop() 
{
}


String timeCalculation(strDateTime dateAndTime)
{
      strDateTime dateTime = dateAndTime; 
      String message = "";
      if(dateTime.valid)
      {   
        byte actualHour = dateTime.hour;
        byte actualMinute = dateTime.minute;
        byte actualSecond = dateTime.second;
        int actualYear = dateTime.year;
        byte actualMonth = dateTime.month;
        byte actualDay = dateTime.day;
  
        String Hour = actualHour < 10 ? "0" + String(actualHour) : String(actualHour);
        String Minute = actualMinute < 10 ? "0" + String(actualMinute) : String(actualMinute);
        String Second = actualSecond < 10 ? "0" + String(actualSecond) : String(actualSecond);
        String Year = actualYear < 10 ? "0" + String(actualYear) : String(actualYear);
        String Month = actualMonth < 10 ? "0" + String(actualMonth) : String(actualMonth);
        String Day = actualDay < 10 ? "0" + String(actualDay) : String(actualDay);
  
        message = Year + "." + Month + "." + Day + "." + " " + Hour + ":" + Minute + ":" + Second;
        message = "CurTime:" + message;
      }
      else
      {
        Serial.print("dateTime is not valid");
        message = "dateTime is not valid";
      }
      return message;
} 


String temperatureMeasurement()
{
  return "Hum:"+String(dht.readHumidity())+"Temp:"+String(dht.readTemperature())+"END#";
}
