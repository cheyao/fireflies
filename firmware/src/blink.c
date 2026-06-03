#include "ch32fun.h"

#include <stdio.h>

// I2C2 (PC)
#define PIN_SDA PC6
#define PIN_SCL PC5
#define ADDR_READ

#define DELAY1 Delay_Us(1);
#define DELAY2 Delay_Us(2);
#define DSCL_IHIGH                                                                                                     \
	{                                                                                                              \
		funPinMode(PIN_SCL, GPIO_CFGLR_IN_PUPD);                                                               \
		funDigitalWrite(PIN_SCL, 1);                                                                           \
	}
#define DSDA_IHIGH                                                                                                     \
	{                                                                                                              \
		funPinMode(PIN_SDA, GPIO_CFGLR_IN_PUPD);                                                               \
		funDigitalWrite(PIN_SDA, 1);                                                                           \
	}
#define DSDA_INPUT                                                                                                     \
	{                                                                                                              \
		funPinMode(PIN_SDA, GPIO_CFGLR_IN_PUPD);                                                               \
		funDigitalWrite(PIN_SDA, 1);                                                                           \
	}
#define DSCL_OUTPUT                                                                                                    \
	{                                                                                                              \
		funDigitalWrite(PIN_SCL, 0);                                                                           \
		funPinMode(PIN_SCL, GPIO_CFGLR_OUT_2Mhz_PP);                                                           \
	}
#define DSDA_OUTPUT                                                                                                    \
	{                                                                                                              \
		funDigitalWrite(PIN_SDA, 0);                                                                           \
		funPinMode(PIN_SDA, GPIO_CFGLR_OUT_2Mhz_PP);                                                           \
	}
#define READ_DSDA funDigitalRead(PIN_SDA)
#define I2CNEEDGETBYTE 1
#define I2CNEEDSCAN 0

#include "static_i2c.h"

volatile uint16_t adc_buffer[1];

void setup_opa(void);

#define SENSOR_WRITE 0xA6
#define SENSOR_READ 0xA7

#define MAIN_CTRL 0x00
#define ALS_MEAS_RATE 0x04
#define ALS_GAIN 0x05
#define PART_ID 0x06
#define MAIN_STATUS 0x07
#define ALS_DATA_0 0x0D
#define ALS_DATA_1 0x0E
#define ALS_DATA_2 0x0F
#define INT_CFG 0x19
#define INT_PST 0x1A
#define ALS_THRES_UP_0 0x21
#define ALS_THRES_UP_1 0x22
#define ALS_THRES_UP_2 0x23
#define ALS_THRES_LOW_0 0x24
#define ALS_THRES_LOW_1 0x25
#define ALS_THRES_LOW_2 0x26

static uint8_t ReadByte(const uint8_t reg) {
	SendStart();
	SendByte(SENSOR_WRITE);
	SendByte(reg);
	SendStart();
	SendByte(SENSOR_READ);
	const uint8_t data = GetByte(1);
	SendStop();

	return data;
}

int main() {
	SystemInit();
	funGpioInitAll();

	ConfigI2C();

	// Example: 0xA6: 10100110
	// ...SD...............E.R
	// ---____--___--___--__--_
	// --___-----_____----x-zz-

	// Set resolution to 25ms
	SendStart();
	SendByte(SENSOR_WRITE);
	SendByte(ALS_MEAS_RATE);
	SendByte(0b01000000);
	SendStop();
    
    // Gain
	SendStart();
	SendByte(SENSOR_WRITE);
	SendByte(ALS_GAIN);
	SendByte(0b00000100);
	SendStop();

	// Enable
	SendStart();
	SendByte(SENSOR_WRITE);
	SendByte(MAIN_CTRL);
	SendByte(0b00000010);
	SendStop();

	// Read status
	while (1) {
		const uint8_t status = ReadByte(MAIN_STATUS);

		Delay_Ms(1);

		if (!(status & (1 << 3))) {
			// No new data
			continue;
		}

		const uint32_t value =
			(ReadByte(ALS_DATA_2) << 16) | (ReadByte(ALS_DATA_1) << 8) | (ReadByte(ALS_DATA_0) << 0);
		printf("0x%lx\n", value);
	}
}

void setup_opa(void) {
	RCC->CFGR0 &= ~(0x1F << 11);
	RCC->APB2PCENR |= RCC_APB2Periph_ADC1;

	// PD4 is analog input chl 7
	GPIOD->CFGLR &= ~(0xF << (4 * 4));

	// Reset the ADC to init all regs
	RCC->APB2PRSTR |= RCC_APB2Periph_ADC1;
	RCC->APB2PRSTR &= ~RCC_APB2Periph_ADC1;

	// Set up four conversions on chl 7, 4, 3, 2
	ADC1->RSQR1 = 0 << 20; // four chls in the sequence
	ADC1->RSQR2 = 0;
	ADC1->RSQR3 = 7 << (5 * 0);

	// set sampling time for chl 7, 4, 3, 2
	// 0:7 => 3/9/15/30/43/57/73/241 cycles
	ADC1->SAMPTR2 = 7 << (3 * 7);

	// turn on ADC
	ADC1->CTLR2 |= ADC_ADON;

	// Reset calibration
	ADC1->CTLR2 |= ADC_RSTCAL;
	while (ADC1->CTLR2 & ADC_RSTCAL)
		;

	// Calibrate
	ADC1->CTLR2 |= ADC_CAL;
	while (ADC1->CTLR2 & ADC_CAL)
		;

	// Turn on DMA
	RCC->AHBPCENR |= RCC_AHBPeriph_DMA1;

	// DMA1_Channel1 is for ADC
	DMA1_Channel1->PADDR = (uint32_t)&ADC1->RDATAR;
	DMA1_Channel1->MADDR = (uint32_t)adc_buffer;
	DMA1_Channel1->CNTR = 1;
	DMA1_Channel1->CFGR = DMA_M2M_Disable | DMA_Priority_VeryHigh | DMA_MemoryDataSize_HalfWord |
			      DMA_PeripheralDataSize_HalfWord | DMA_MemoryInc_Enable | DMA_Mode_Circular |
			      DMA_DIR_PeripheralSRC;

	// Turn on DMA channel 1
	DMA1_Channel1->CFGR |= DMA_CFGR1_EN;

	// enable scanning
	ADC1->CTLR1 |= ADC_SCAN;

	// Enable continuous conversion and DMA
	ADC1->CTLR2 |= ADC_CONT | ADC_DMA | ADC_EXTSEL;

	// start conversion
	ADC1->CTLR2 |= ADC_SWSTART;

	// turn on the op-amp
	EXTEN->EXTEN_CTR |= EXTEN_OPA_EN;

	EXTEN->EXTEN_CTR &= ~EXTEN_OPA_PSEL;
	EXTEN->EXTEN_CTR &= ~EXTEN_OPA_NSEL;
}
