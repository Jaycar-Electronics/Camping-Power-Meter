#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "<Your-WiFi-SSID>";
const char* password = "<Your-WiFi-Password>";

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

double data = 0;	// to store current reading
String text = ""; // to convert reading -> text

// to include the html file that we've written, plus any other code here.
// note the structure: this will smash it all into one long string called
// indexhtml

#define str(x) #x

const char indexhtml[] PROGMEM = str(#include "index.h");


void setup(void){
	pinMode(A0, INPUT);
	Serial.begin(115200);
	WiFi.begin(ssid, password); //begin WiFi connection
	Serial.println("");
	Serial.println(indexhtml);
	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

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
	Serial.println("Web server started!");
}

void loop(void){
	data = analogRead(A0);
	delay(100);
	server.handleClient();
}
