


void setup(){
	Serial.begin(9600);
	Serial.println("A0 Test for ESP8266 Camping Battery Meter");
	Serial.println("-----------");
	Serial.println("Connect circuit and ESP, then open this serial monitor while adjusting current");
	Serial.println("-----------");
	//out of 1024 maps to 0 -> 3.3v
	Serial.println("5A = 50mA, so each amp is 10mV on the QP5510 Shunt");

	Serial.println();
	Serial.println();
	Serial.println("Press [Enter] when ready");
}

void loop(){
	int reading = analogRead(A0);

	Serial.print("Reading A0: ");
	Serial.print(reading,DEC);
	Serial.print(", converted to: ");
	Serial.print(round(((float)reading/1024.0)*100), DEC);
	Serial.println("\% of 5A amps");

	delay(100);
}

