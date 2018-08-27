#include "AirQualityApi.h"

AirQualityApi::AirQualityApi(Client &client)	{
  this->client = &client;
}

String AirQualityApi::SendGetToAirQuality(String command) {
  String body="";
  body.reserve(700);
  bool finishedHeaders = false;
  bool currentLineIsBlank = true;
	long now;
	bool avail;

	if (client->connect(AirQuality_HOST, Port)) {
		Serial.println(".... connected to server");
		String a="";
		char c;
		int ch_count=0;
		client->println("GET " + command + " HTTP/1.1");
    client->println("Host: " AirQuality_HOST);
		client->println(F("User-Agent: arduino/1.0.0"));
		client->println();
		now=millis();
		avail=false;
		while (millis()-now<1500) {
			while (client->available()) {
				char c = client->read();
	//			Serial.write(c);

        if(!finishedHeaders){
          if (currentLineIsBlank && c == '\n') {
            finishedHeaders = true;
          }
        } else {
          body=body+c;
          ch_count++;
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        }else if (c != '\r') {
          currentLineIsBlank = false;
        }

				avail=true;
			}
			if (avail) {
//				Serial.println("Body:");
//				Serial.println(body);
//				Serial.println("END");
				break;
			}
		}
	}
  closeClient();
  return body;
}

AirQualityResponse AirQualityApi::GetAirQualityInfo(String Cityid) {

  String command = "/feed/";
  command += Cityid;
  command +="/?token=";
  command += AirQuality_TOKEN;

 
  Serial.println(command);
  String response = SendGetToAirQuality(command);
  AirQualityResponse responseObject = AirQualityResponse();
  DynamicJsonBuffer jsonBuffer;
JsonObject& root = jsonBuffer.parseObject(response);
  if (root.success()) {

JsonObject& data = root["data"];
responseObject.data_idx = data["idx"]; // 5722
responseObject.data_aqi = data["aqi"]; // 31

JsonObject& data_time = data["time"];
responseObject.data_time_v = data_time["v"]; // 1535320800
responseObject.data_time_s = data_time["s"]; // "2018-08-26 22:00:00"
responseObject.data_time_tz = data_time["tz"]; // "+02:00"

JsonObject& data_city = data["city"];
responseObject.data_city_name = data_city["name"]; // "Paris"
responseObject.data_city_url = data_city["url"]; // "http://aqicn.org/city/paris/"
responseObject.data_city_geo0 = data_city["geo"][0]; // "48.856614"
responseObject.data_city_geo1 = data_city["geo"][1]; // "2.3522219"
responseObject.data_attributions0_name = data["attributions"][0]["name"]; // "AirParif - Association de surveillance de la qualité de l'air en Île-de-France"
responseObject.data_attributions0_url = data["attributions"][0]["url"]; // "http://www.airparif.asso.fr/"
responseObject.data_attributions1_name = data["attributions"][1]["name"]; // "World Air Quality Index Project"
responseObject.data_attributions1_url = data["attributions"][1]["url"]; // "http://waqi.info/"

JsonObject& data_iaqi = data["iaqi"];
responseObject.data_iaqi_pm25_v = data_iaqi["pm25"]["v"]; // 31
responseObject.data_iaqi_pm10_v = data_iaqi["pm10"]["v"]; // 12
responseObject.data_iaqi_o3_v = data_iaqi["o3"]["v"]; // 28
responseObject.data_iaqi_no2_v = data_iaqi["no2"]["v"]; // 9
responseObject.data_iaqi_so2_v = data_iaqi["so2"]["v"]; // 1
responseObject.data_iaqi_co_v = data_iaqi["co"]["v"]; // 3
responseObject.data_iaqi_t_v = data_iaqi["t"]["v"]; // 23
responseObject.data_iaqi_p_v = data_iaqi["p"]["v"]; // 1011
responseObject.data_iaqi_h_v = data_iaqi["h"]["v"]; // 41
responseObject.data_iaqi_w_v = data_iaqi["w"]["v"]; // 3data_idx = data["idx"]; // 5722
   
  } else {
    JsonObject& rootObject = jsonBuffer.parseObject(response);
    if (rootObject.containsKey("error")) {
       responseObject.error = rootObject["error"].as<String>();
    } else {
      responseObject.error = "Failed to parse JSON";
    }
  }

  return responseObject;
}

void AirQualityApi::closeClient() {
  if(client->connected()){
    client->stop();
  }
}