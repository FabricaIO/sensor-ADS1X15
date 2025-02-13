#include "ADS1X15.h"

/// @brief Creates a single ended ADC interface
/// @param Name The name of the attached sensor
/// @param Parameter The parameter measured by the sensor
/// @param Channel The channel of the sensor on the ADC
/// @param ADC_Type The type of ADC connected to the device
/// @param Address The I2C address of the ADC
/// @param I2CBus The I2C bus attached to the ADC
/// @param Gain The gain of the ADC
ADS1X15::ADS1X15(String Name, String Parameter, int Channel, ADCType ADC_Type, int Address, TwoWire* I2CBus, adsGain_t Gain) {
	adc_type = ADC_Type;
	address = Address;
	i2cbus = I2CBus;
	channel = Channel;
	gain = Gain;
	Description.parameters = {Parameter};
	Description.name = Name;
}

/// @brief Creates a double ended ADC interface
/// @param Name The name of the attached sensor
/// @param Parameter The parameter measured by the sensor
/// @param Channels The differential channel pair to use
/// @param ADC_Type The type of ADC connected to the device
/// @param Address The I2C address of the ADC
/// @param I2CBus The I2C bus attached to the ADC
/// @param Gain The gain of the ADC
ADS1X15::ADS1X15(String Name, String Parameter, differentialChannels Channels, ADCType ADC_Type, int Address, TwoWire* I2CBus, adsGain_t Gain) {
	adc_type = ADC_Type;
	address = Address;
	i2cbus = I2CBus;
	channels = Channels;
	channel = -1;
	gain = Gain;
	Description.parameters = {Parameter};
	Description.name = Name;
}

bool ADS1X15::begin() {
	Description.parameterQuantity = 1;
	Description.type = "ADC Sensor";
	Description.units = {"mV"};
	Description.version = 1.0;
	values.resize(Description.parameterQuantity);

	// Use the begin() method with the I2C address
	if (adc_type == ADS1115) {
		ads_16.setGain(gain);
		return ads_16.begin(address, i2cbus);
	} else {
		ads_12.setGain(gain);
		return ads_12.begin(address, i2cbus);
	}
}

bool ADS1X15::takeMeasurement() {
	uint16_t mv = getMV();
	if (mv == 0) {
		return false;
	}
	values[0] = mv;
	return true;
}

/// @brief Gets the mV reading from the ADC
/// @return The mV measured (could be negative if differential)
int16_t ADS1X15::getMV() {
	int16_t raw_value;
	if (channel != -1) {
		if (adc_type == ADS1115) {
			return ads_16.computeVolts(ads_16.readADC_SingleEnded(channel));
		} else {
			return ads_12.computeVolts(ads_12.readADC_SingleEnded(channel));
		}
	} else {
		if (adc_type == ADS1115) {
			switch (channels) {
				case differentialChannels::channels_0_1:
					raw_value = ads_16.readADC_Differential_0_1();
					break;
				case differentialChannels::channels_0_3:
					raw_value = ads_16.readADC_Differential_0_3();
					break;
				case differentialChannels::channels_1_3:
					raw_value = ads_16.readADC_Differential_1_3();
					break;
				case differentialChannels::channels_2_3:
					raw_value = ads_16.readADC_Differential_2_3();
					break;
			}
			return ads_16.computeVolts(raw_value);
		} else {
			switch (channels) {
				case differentialChannels::channels_0_1:
					raw_value = ads_12.readADC_Differential_0_1();
					break;
				case differentialChannels::channels_0_3:
					raw_value = ads_12.readADC_Differential_0_3();
					break;
				case differentialChannels::channels_1_3:
					raw_value = ads_12.readADC_Differential_1_3();
					break;
				case differentialChannels::channels_2_3:
					raw_value = ads_12.readADC_Differential_2_3();
					break;
			}
			return ads_12.computeVolts(raw_value);
		}
	}
}
