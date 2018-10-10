// msCompass.h

#ifndef _MSCOMPASS_h
#define _MSCOMPASS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#define IC_QMC5883L 0
#define IC_HMC5883L 1

#define IC_TYPE	IC_QMC5883L

#if (IC_TYPE == IC_QMC5883L)

#define QMC5883L_ADDRESS			uint8_t(0x0D)
#define QMC5883L_REG_OUTPUT_DATA	uint8_t(0x00)	// 16bits for each axis
#define QMC5883L_REG_STATUS			uint8_t(0x06)	//
#define QMC5883L_REG_TEMPERATURE	uint8_t(0x07)	// 16bits
#define QMC5883L_REG_CONTROL_1		uint8_t(0x09)	//
#define QMC5883L_REG_CONTROL_2		uint8_t(0x0A)	//
#define QMC5883L_REG_SET_RESET		uint8_t(0x0B)	//
#define QMC5883L_REG_CHIP_ID		uint8_t(0x0D)	//

#define MODE_STANBY			0x00		//Mode Control, Set Continuous Measuring Mode
#define MODE_CONTINUOUS		0x01		//Mode Control, Set Continuous Measuring Mode

#define OUTPUT_DATA_RATE_10_HZ		0x00<<2
#define OUTPUT_DATA_RATE_50_HZ		0x01<<2
#define OUTPUT_DATA_RATE_100_HZ		0x02<<2
#define OUTPUT_DATA_RATE_200_HZ		0x03<<2

#define RANGE_2G					0x00<<4		//Set Scale as 2G
#define RANGE_8G					0x01<<4		//Set Scale as 8G

#define OVER_SAMPLE_RATIO_512		0x00<<6		//Over Sample Ratio
#define OVER_SAMPLE_RATIO_256		0x01<<6
#define OVER_SAMPLE_RATIO_128		0x02<<6
#define OVER_SAMPLE_RATIO_64		0x03<<6

#else

#define HMC5883L_ADDRESS			uint8_t(0x1E)
#define HMC5883L_REG_OUTPUT_DATA	uint8_t(0x03)
#define HMC5883L_REG_STATUS			uint8_t(0x09)
#define HMC5883L_REG_CONFIG_A		uint8_t(0x00)
#define HMC5883L_REG_CONFIG_B		uint8_t(0x01)
#define HMC5883L_REG_MODE			uint8_t(0x02)

#define HMC5883L_REG_ID_A			uint8_t(0x0A)
#define HMC5883L_REG_ID_B			uint8_t(0x0B)
#define HMC5883L_REG_ID_C			uint8_t(0x0C)

//(Control Reg A) Select number of samples averaged (1 to 8) per measurement output
#define MEASUREMENT_AVERAGE_1		0x00<<5
#define MEASUREMENT_AVERAGE_2		0x01<<5
#define MEASUREMENT_AVERAGE_4		0x02<<5
#define MEASUREMENT_AVERAGE_8		0x03<<5

//(Control Reg A) Data Output Rate Bits.
//These bits set the rate at which data is written to all three data output registers.
#define OUTPUT_DATA_RATE_0p75_HZ	0x00<<2
#define OUTPUT_DATA_RATE_1p5_HZ		0x01<<2
#define OUTPUT_DATA_RATE_3_HZ		0x02<<2
#define OUTPUT_DATA_RATE_7p5_HZ		0x03<<2
#define OUTPUT_DATA_RATE_15_HZ		0x04<<2
#define OUTPUT_DATA_RATE_30_HZ		0x05<<2
#define OUTPUT_DATA_RATE_75_HZ		0x06<<2

//(Control Reg A) Measurement Mode
#define MEASUREMENT_NORMAL					0x00<<5
#define MEASUREMENT_POSITIVE_BIAS			0x01<<5
#define MEASUREMENT_NEGATIVE_BIAS			0x02<<5

//(Control Reg B) Gain Configuration
#define RANGE_0p88_GAUSS			0x00<<5	//Gain = 1370 LSb/Gauss
#define RANGE_1p3_GAUSS				0x01<<5	//Gain = 1090 LSb/Gauss
#define RANGE_1p9_GAUSS				0x02<<5	//Gain = 820 LSb/Gauss
#define RANGE_2p5_GAUSS				0x03<<5	//Gain = 660 LSb/Gauss
#define RANGE_4_GAUSS				0x04<<5	//Gain = 440 LSb/Gauss
#define RANGE_4p7_GAUSS				0x05<<5	//Gain = 390 LSb/Gauss
#define RANGE_5p6_GAUSS				0x06<<5	//Gain = 330 LSb/Gauss
#define RANGE_8p1_GAUSS				0x07<<5	//Gain = 230 LSb/Gauss

//(Mode Reg) Operating Mode
#define HIGHSPEED_I2C_ENABLE		0x01<<7
#define MODE_CONTINUOUS				0x00
#define MODE_SINGLE					0x01
#define MODE_STANBY					0x10

#endif

struct SensorValue {
	double X;
	double Y;
	double Z;

	SensorValue operator + (const SensorValue & v) {
		SensorValue out;
		out.X = this->X + v.X;
		out.Y = this->Y + v.Y;
		out.Z = this->Z + v.Z;
		return out;
	}
	SensorValue operator - (const SensorValue & v) {
		SensorValue out;
		out.X = this->X - v.X;
		out.Y = this->Y - v.Y;
		out.Z = this->Z - v.Z;
		return out;
	}
	SensorValue operator * (const SensorValue & v) {
		SensorValue out;
		out.X = this->X * v.X;
		out.Y = this->Y * v.Y;
		out.Z = this->Z * v.Z;
		return out;
	}
	SensorValue operator / (const SensorValue & v) {
		SensorValue out;
		out.X = this->X / v.X;
		out.Y = this->Y / v.Y;
		out.Z = this->Z / v.Z;
		return out;
	}
};

class MsCompassClass
{
protected:
	SensorValue scale = { 1.0,1.0,1.0 };
	SensorValue offset = { 0.0,0.0,0.0 };

	bool write(uint8_t _address, byte _value);
	bool read(byte * _buff, uint8_t _address, uint8_t _length);
public:
	bool Init();
	bool ReadRaw(SensorValue & _raw);
	bool ReadScaled(SensorValue & _scaled);
	void SetScale(SensorValue _scale);
	SensorValue GetScale();
	void SetOffset(SensorValue _offset);
	SensorValue GetOffset();
};

extern MsCompassClass Compass;

#endif
