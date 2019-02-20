#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#define custom_ssid "Camping Power Meter"

ESP8266WebServer	server(80);	//instantiate server at port 80 (http port)
IPAddress			apIP(10,0,0,7);	//our access point ip

double data = 0;	// to store current reading
String text = ""; // to convert reading -> text

void setup(void){
	SPIFFS.begin();			//start filesystem
	pinMode(A0, INPUT);
	Serial.begin(115200);

	WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));
	WiFi.softAP(custom_ssid);

	//here we define a server mountpoint; when the user accesses current, we send them the text data
	server.on("/current", [](){
		text = (String)data;
		server.send(200, "text/html", text);
	});

	//otherwise, if we can't find anything ( in our mountpoints) we then call the fileRead function
	server.onNotFound([](){
		if(!fileRead(server.uri())){
			server.send(404, "text/html", "<h1>404 File not found on SPIFFS</h1>");
		}
	});

	server.begin();

	Serial.println();
	Serial.println("Web server started!");

}

void loop(void){
	data = analogRead(A0) / 1024;
	server.handleClient();
}

bool fileRead(String filepath){
	if(filepath.endsWith("/")) filepath += "index.html";
	if( SPIFFS.exists(filepath) ) {

		File f = SPIFFS.open(filepath, "r");
		server.streamFile(f, contentType(filepath));
    f.close();
		return true;
	}
	return false;
}

String contentType(String filepath){
	if(filepath.endsWith(".html")) return "text/html";
	else if(filepath.endsWith(".css")) return "text/css";
	else if(filepath.endsWith(".js")) return "text/javascript";
	else return "text/plain";
}
