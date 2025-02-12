/*
* This file and associated .cpp file are licensed under the GPLv3 License Copyright (c) 2024 Sam Groveman and Trevor James Durning 
* Contributors: Sam Groveman, Trevor James Durning
* 
* External libraries needed:
* Adafruit_ADS1X15: https://github.com/adafruit/Adafruit_ADS1X15

* ADS1115: https://www.adafruit.com/product/1085
* ADS1015: https://www.adafruit.com/product/1083
*/

#pragma once
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Sensor.h>

class ADS1X15 : public Sensor {
    public:
		enum ADCType {ADS1115, ADS1015};
		enum differentialChannels {channels_0_1, channels_0_3, channels_1_3, channels_2_3};
        ADS1X15(String Name, String Parameter, int Channel, ADCType ADC_Type, int Address = 0x49, TwoWire* I2CBus = &Wire, adsGain_t Gain  = GAIN_TWOTHIRDS);
		ADS1X15(String Name, String Parameter, differentialChannels Channels, ADCType ADC_Type, int Address = 0x49, TwoWire* I2CBus = &Wire, adsGain_t Gain  = GAIN_TWOTHIRDS);
        bool begin();
        bool takeMeasurement();

    protected:
        /// @brief The ADS1115 object
        Adafruit_ADS1115 ads_16;

		/// @brief The ADS1015 object
        Adafruit_ADS1015 ads_12;

		/// @brief The type of ADC connected to the device
		ADCType adc_type;

		/// @brief The gain of the ADC
		adsGain_t gain;
        
		/// @brief The I2C address of the ADC
		int address;

		/// @brief The channel of the sensor on the ADC
		int channel;

		/// @brief The differential channels to use on the ADC
		differentialChannels channels;

        /// @brief The I2C bus
        TwoWire* i2cbus;

		int16_t getMV();
};