#include "ch32fun.h"

#include <stdio.h>

volatile uint16_t adc_buffer[1];

void setup_opa(void);
void setup_i2c(void);

void i2c_send(uint8_t addr, uint8_t* data, uint8_t size);
uint8_t i2c_event(uint32_t event_mask);

// I2C2
#define SDA PC6
#define SCL PC5

int main() {
	SystemInit();
	funGpioInitAll();

	setup_i2c();

	while (1) {
		/*
		// Wait till DMA recieves a value
			while (!(DMA1->INTFR & DMA1_FLAG_TC1))
				;
			DMA1->INTFCR = DMA1_FLAG_TC1;

			printf("%d\n", adc_buffer[0]);
		*/

		Delay_Ms(5);
	}
}

uint8_t i2c_event(uint32_t event_mask) {
	/* read order matters here! STAR1 before STAR2!! */
	uint32_t status = I2C1->STAR1 | (I2C1->STAR2 << 16);
	return (status & event_mask) == event_mask;
}

#define TIMEOUT_MAX 100000
void i2c_send(uint8_t addr, uint8_t* data, uint8_t size) {
	int32_t timeout;

	timeout = TIMEOUT_MAX;
	// We are busy
	while ((I2C1->STAR2 & I2C_STAR2_BUSY) && (timeout--))
		;
	if (timeout == -1) {
		printf("I2C Busy");
		return;
	}

	I2C1->CTLR1 |= I2C_CTLR1_START;
	timeout = TIMEOUT_MAX;
	while ((!i2c_event(SSD1306_I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--))
		;
	if (timeout == -1) {
		printf("I2C not in master mode");
		return;
	}

	I2C1->DATAR = addr << 1;

	timeout = TIMEOUT_MAX;
	while ((!i2c_event(SSD1306_I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (timeout--))
		;
	if (timeout == -1) {
		printf("I2C not in master transmit mode");
		return;
	}

	while (size--) {
		// wait for TX Empty
		timeout = TIMEOUT_MAX;
		while (!(I2C1->STAR1 & I2C_STAR1_TXE) && (timeout--))
			;
		if (timeout == -1) {
			printf("I2C not in tx empty");
			return;
		}

		I2C1->DATAR = *data++;
	}

	// wait for tx complete
	timeout = TIMEOUT_MAX;
	while ((!i2c_event(SSD1306_I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--))
		;
	if (timeout == -1) {
        printf("I2C not transmit completing")
    }

	// set STOP condition
	I2C1->CTLR1 |= I2C_CTLR1_STOP;
}

void setup_i2c(void) {
	// AFIO enabled by funinitall
	RCC->APB1PCENR |= RCC_APB1Periph_I2C1;
	AFIO->PCFR1 &= AFIO_PCFR1_I2C1_HIGH_BIT_REMAP; // PC5 & PC6

	GPIOC->CFGLR &= (0xf << (4 * 5)) | (0xf << (4 * 6));
	GPIOC->CFGLR |= ((GPIO_Speed_30MHz | GPIO_CNF_OUT_OD_AF) << (4 * 5)) |
			((GPIO_Speed_30MHz | GPIO_CNF_OUT_OD_AF) << (4 * 6));

	RCC->APB1PRSTR |= RCC_APB1Periph_I2C1;
	RCC->APB1PRSTR &= ~RCC_APB1Periph_I2C1;

	I2C1->CTLR2 |= (FUNCONF_SYSTEM_CORE_CLOCK / 2000000) & I2C_CTLR2_FREQ;
	I2C1->CKCFGR = (1 << 15) | (1 << 14) | (FUNCONF_SYSTEM_CORE_CLOCK / (25 * 1000000)) & I2C_CKCFGR_CCR;

	I2C1->CTLR1 |= I2C_CTLR1_PE;
	I2C1->CTLR1 |= I2C_CTLR1_ACK;
}

void setup_opa(void) {
	RCC->CFGR0 &= ~(0x1F << 11);
	RCC->APB2PCENR |= RCC_APB2Periph_ADC1;

	// PD4 is analog input chl 7
	GPIOD->CFGLR &= ~(0xf << (4 * 4));

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
