/*
	Name:       QMC5883L.ino
	Created:	09/21/2018 22:26:52
	Author:     WILDLIFE\maisonsmd
*/

#include "msCompass.h"

void InitSensor() {
	boolean succeed = false;

	Wire.setClock(100000);
	do {
		delay(100);

		Serial.println("trying IIC");
		succeed = Compass.Init();

		if (!succeed)
			Serial.println("Cannot communicate!");
	} while (!succeed);

	Serial.println("Init successfully");
}

void setup() {
	//delay(3000);

	Serial.begin(115200);
	Wire.begin();
	InitSensor();
}
void loop() {
	Vector3 rawVal;

	if (Compass.ReadRaw(rawVal)) {
		Serial.print(rawVal.X);
		Serial.print('\t');
		Serial.print(rawVal.Y);
		Serial.print('\t');
		Serial.print(rawVal.Z);
		float headingDegrees = atan2(rawVal.Y, rawVal.X) * 180 / 3.14 + 180;
		Serial.print('\t');
		Serial.println(headingDegrees);
	}
	else {
		Serial.println('.');
	}

	delay(5);
}