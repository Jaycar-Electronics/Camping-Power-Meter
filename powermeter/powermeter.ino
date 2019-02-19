#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "./html.h"				//html page

#define custom_ssid "Camping Power Meter"

ESP8266WebServer	server(80);	//instantiate server at port 80 (http port)
IPAddress			apIP(10,0,0,7);	//our access point ip

double data = 0;	// to store current reading
String text = ""; // to convert reading -> text


void setup(void){
	pinMode(A0, INPUT);
	Serial.begin(115200);
	WiFi.setOutputPower(10);
	WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));
	WiFi.softAP(custom_ssid);
	
	//here we define what different web-routes are
	//notice the website will get up-to-date data from "/current"
	//so here we define that we send back a reading of the data.

	server.on("/current", [](){
		text = (String)data;
		server.send(200, "text/html", text);
	});

	server.on("/", [](){
		server.send(200, "text/html", indexhtml);
	});

	server.begin();
	Serial.println();
	Serial.println("Web server started!");
}

void loop(void){
	data = analogRead(A0);
	Serial.println(data,DEC);
	delay(50);
	server.handleClient();
}
