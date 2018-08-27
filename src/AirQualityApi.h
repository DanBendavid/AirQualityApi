
//Air Quality Open Data Platform  http://aqicn.org

#ifndef AirQualityApi_h
#define AirQualityApi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>

#define AirQuality_HOST "api.waqi.info"
#define AirQuality_TOKEN "xxxxxxxxxxxxxxxxxxxxxx"

struct AirQualityResponse{
  
//const char* status ;
  
int data_idx;
int data_aqi;
  
long data_time_v; // 1535320800
const char* data_time_s; // "2018-08-26 22:00:00"
const char* data_time_tz; // "+02:00"
const char* data_city_name; // "Paris"
const char* data_city_url; // "http://aqicn.org/city/paris/"
const char* data_city_geo0; // "48.856614"
const char* data_city_geo1; // "2.3522219"
const char* data_attributions0_name; // "AirParif - Association de surveillance de la qualité de l'air en Île-de-France"
const char* data_attributions0_url; // "http://www.airparif.asso.fr/"
const char* data_attributions1_name; // "World Air Quality Index Project"
const char* data_attributions1_url; // "http://waqi.info/"  

int data_iaqi_pm25_v;
int data_iaqi_pm10_v; // 12
int data_iaqi_o3_v; // 28
int data_iaqi_no2_v; // 9
int data_iaqi_so2_v; // 1
int data_iaqi_co_v; // 3
int data_iaqi_t_v; // 23
int data_iaqi_p_v; // 1011
int data_iaqi_h_v; // 41
int data_iaqi_w_v; // 3

String error;
};


class AirQualityApi
{
  public:
    AirQualityApi (Client &client);
    String SendGetToAirQuality(String command);
    AirQualityResponse GetAirQualityInfo(String CityId);
    int Port = 80;

  private:
    Client *client;
    void closeClient();
};

#endif
