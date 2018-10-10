#include "msCompass.h"

const uint8_t BeginBytes[] = { 0xAA, 0xCD };
const uint8_t EndBytes[] = { 0xBB, 0xFA };

const uint8_t ConnectionQueryBytes[] = { 0xAA, 0xBB };
const uint8_t ConnectionQueryResponseBytes[] = { 0xCC,0xDD };

const uint8_t PackageLength = 30;
uint8_t InBuffer[PackageLength];
uint8_t OutBuffer[PackageLength];

const uint8_t CommandRead[] = { 0xFA,0x01 };

template<typename T>
void SplitBytes(uint8_t dst[sizeof(T)], uint8_t dstOffset, T val) {
    union {
        T typeVal;
        uint8_t bytes[sizeof(T)];
    } thing;
    thing.typeVal = val;
    memcpy(&dst[dstOffset], thing.bytes, sizeof(T));
}

template<typename T>
T MergeBytes(uint8_t src[sizeof(T)], uint8_t srcOffset)
{
    return (*((T *)(&src[srcOffset])));
}

void InitSensor() {
	boolean succeed = false;

	Wire.setClock(100000);
	do {
		delay(100);

		//Serial.println("trying IIC");
		succeed = Compass.Init();

		//if (!succeed)
			//Serial.println("Cannot communicate!");
	} while (!succeed);

	//Serial.println("Init successfully");
}

void setup() {
	delay(3000);

	Serial.begin(115200);
	Wire.begin();
	InitSensor();
	pinMode(PC13, OUTPUT);
}

uint32_t lastTick = 0;
void loop() {
	//Vector3 rawVal;

	/*if (Compass.ReadRaw(rawVal)) {
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
	}*/

	if (millis() - lastTick > 100) {
		lastTick = millis();
		digitalWrite(PC13, !digitalRead(PC13));
	}

    if (!Serial.available())
        return;

    Serial.readBytes((char*)InBuffer, PackageLength);
    if (Compare(InBuffer, 0, ConnectionQueryBytes, 0, sizeof(ConnectionQueryBytes))) {
        for (uint8_t i = 0; i < sizeof(ConnectionQueryResponseBytes); i++)
            Serial.write(ConnectionQueryResponseBytes[i]);
    }
    if (Compare(InBuffer, 0, CommandRead, 0, sizeof(CommandRead))) {
        SendDumpData();
    }
}

void SendDumpData() {
    CompassRawValue rawVal;

    if (!Compass.ReadRaw(rawVal)) {
        return;
    }

    uint16_t checksum = 0;
    BlockCopy(BeginBytes, 0, OutBuffer, 0, sizeof(BeginBytes));
    BlockCopy(EndBytes, 0, OutBuffer, PackageLength - sizeof(EndBytes), sizeof(EndBytes));

    uint8_t dataLength = PackageLength - sizeof(BeginBytes) - sizeof(EndBytes) - sizeof(checksum);
    /*uint8_t dump[PackageLength - sizeof(BeginBytes) - sizeof(EndBytes) - sizeof(checksum)];
    for (uint8_t i = 0; i < sizeof(dump); i++) {
        dump[i] = random(0, 255);
        checksum += dump[i];
    }

    BlockCopy(dump, 0, OutBuffer, 0 + sizeof(BeginBytes), sizeof(dump));*/

    uint8_t currentIndex = 0 + sizeof(BeginBytes);
    uint8_t dataStartIndex = 0 + sizeof(BeginBytes);

    SplitBytes(OutBuffer, currentIndex, rawVal.X);
    currentIndex += sizeof(int16_t);
    SplitBytes(OutBuffer, currentIndex, rawVal.Y);
    currentIndex += sizeof(int16_t);
    SplitBytes(OutBuffer, currentIndex, rawVal.Z);
    currentIndex += sizeof(int16_t);

    for (uint8_t i = dataStartIndex; i < dataLength + dataStartIndex; i++)
        checksum += (uint16_t)OutBuffer[i];
    
    SplitBytes(OutBuffer, 0 + sizeof(BeginBytes) + dataLength, checksum);

    for (uint8_t i = 0; i < PackageLength; i++) {
        Serial.write(OutBuffer[i]);
    }
}



/// <summary>
/// compare 2 arrays
/// </summary>
/// <param name="src">source array</param>
/// <param name="srcOffset">source offset</param>
/// <param name="dst">destination array</param>
/// <param name="dstOffset">destination offset</param>
/// <param name="count">number of bytes to compare</param>
/// <returns></returns>
bool Compare(const uint8_t src[], uint8_t srcOffset, const uint8_t dst[], uint8_t dstOffset, uint8_t count) {
    for (int i = 0; i < count; i++) {
        if (src[srcOffset + i] != dst[dstOffset + i])
            return false;
    }
    return true;
}

bool BlockCopy(const uint8_t src[], uint8_t srcOffset, uint8_t dst[], uint8_t dstOffset, uint8_t count) {
    memcpy(&dst[dstOffset], &src[srcOffset], count);
    return true;
}