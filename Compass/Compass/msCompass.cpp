// 
// 
// 
#include "msCompass.h"

boolean MsCompassClass::write(uint8_t _address, byte _value)
{
#if IC_TYPE == IC_QMC5883L
	Wire.beginTransmission(QMC5883L_ADDRESS);
#else
	Wire.beginTransmission(HMC5883L_ADDRESS);
#endif
	Wire.write(_address);
	Wire.write(_value);
	return 	Wire.endTransmission() == 0;
}

boolean MsCompassClass::read(byte * _buff, uint8_t _address, uint8_t _length)
{
#if IC_TYPE == IC_QMC5883L
	Wire.beginTransmission(QMC5883L_ADDRESS);
#else
	Wire.beginTransmission(HMC5883L_ADDRESS);
#endif

	Wire.write(_address);
	if (Wire.endTransmission() != 0)
		return false;

#if IC_TYPE == IC_QMC5883L
	Wire.requestFrom(QMC5883L_ADDRESS, _length);
#else 
	Wire.requestFrom(HMC5883L_ADDRESS, _length);
#endif

	if (Wire.available() == _length)
		for (byte i = 0; i < _length; i++)
			_buff[i] = Wire.read();
	else
		return false;
	return true;
}

boolean MsCompassClass::Init()
{
#if IC_TYPE == IC_QMC5883L
	boolean succeed = write(QMC5883L_REG_SET_RESET, 0x01);
	if (!succeed)
		return false;
	succeed = write(QMC5883L_REG_CONTROL_1, MODE_CONTINUOUS | OUTPUT_DATA_RATE_200_HZ | RANGE_8G | OVER_SAMPLE_RATIO_512);
	return succeed;
#else
	boolean succeed = write(HMC5883L_REG_CONFIG_A, MEASUREMENT_AVERAGE_8 | OUTPUT_DATA_RATE_75_HZ | MEASUREMENT_NORMAL);
	if (!succeed)
		return false;
	succeed = write(HMC5883L_REG_CONFIG_B, RANGE_0p88_GAUSS);
	if (!succeed)
		return false;
	succeed = write(HMC5883L_REG_MODE, MODE_CONTINUOUS);
	return succeed;
#endif
}

boolean MsCompassClass::ReadRaw(Vector3 & _raw)
{
	byte data[6];

#if IC_TYPE == IC_QMC5883L
	if (!read(data, QMC5883L_REG_OUTPUT_DATA, 6))
		return false;

	_raw.X |= (data[1] << 8) | data[0];
	_raw.Y |= (data[3] << 8) | data[2];
	_raw.Z |= (data[5] << 8) | data[4];
#else
	if (!read(data, HMC5883L_REG_OUTPUT_DATA, 6))
		return false;

	_raw.X |= (data[0] << 8) | data[1];
	_raw.Y |= (data[4] << 8) | data[5];
	_raw.Z |= (data[2] << 8) | data[3];
#endif
	return true;
}

MsCompassClass Compass;

