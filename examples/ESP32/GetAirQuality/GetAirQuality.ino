#include <AirQualityApi.h>


#include <WiFi.h>
#include <WiFiClientSecure.h>

//------- Install From Library Manager -------
#include <ArduinoJson.h>


//------- Replace the following! ------
char ssid[] = "SSID";       // your network SSID (name)
char password[] = "PASSWORD";  // your network key


WiFiClientSecure client;
AirQualityApi api(client);

unsigned long api_mtbs = 60000; //mean time between api requests
unsigned long api_due_time = 0;

void setup() {

  Serial.begin(115200);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void printAirQualityData(String Cityid) {
  Serial.println("---------------------------------");
  Serial.println("Getting ticker data for " + Cityid);

  // Go to AirQuality.com and select the city you would like to check
  // Need to obtain a Token

  AirQualityResponse response = api.GetAirQualityInfo(Cityid);
  if (response.error == "") {
    Serial.print("City ID: ");
    Serial.println(response.data_idx);
    Serial.print("AQI: ");
    Serial.println(response.data_aqi);


    Serial.print("Time v: ");
    Serial.println(response.data_time_v);
    Serial.print("Time: ");
    Serial.println(response.data_time_s);
    Serial.print("Time Zone: ");
    Serial.println(response.data_time_tz);

    Serial.print("City name: ");
    Serial.println(response.data_city_name);
    Serial.print("City URL: ");
    Serial.println(response.data_city_url);

    Serial.print("City Long: ");
    Serial.println(response.data_city_geo0);
    Serial.print("City Lat: ");
    Serial.println(response.data_city_geo1);

    Serial.print("PM2.5: ");
    Serial.println(response.data_iaqi_pm25_v);
    Serial.print("PM10: ");
    Serial.println(response.data_iaqi_pm10_v);
    Serial.print("O3: ");
    Serial.println(response.data_iaqi_o3_v);
    Serial.print("SO2: ");
    Serial.println(response.data_iaqi_so2_v);
    Serial.print("CO: ");
    Serial.println(response.data_iaqi_co_v);
    Serial.print("Temp: ");
    Serial.println(response.data_iaqi_t_v);
	Serial.print("Pression: ");
    Serial.println(response.data_iaqi_p_v);
    Serial.print("H: ");
    Serial.println(response.data_iaqi_h_v);
    Serial.print("W: ");
    Serial.println(response.data_iaqi_w_v);
	


  } else {
    Serial.print("Error getting data: ");
    Serial.println(response.error);
  }
  Serial.println("---------------------------------");
}

void loop() {
  unsigned long timeNow = millis();
  if ((timeNow > api_due_time))  {
    printAirQualityData("@5722");
    api_due_time = timeNow + api_mtbs;
  }
}
