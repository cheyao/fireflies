#include "ch32fun.h"

#include <stdio.h>

volatile uint16_t adc_buffer[1];

int main() {
	SystemInit();
	funGpioInitAll();

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

	while (1) {
		while (!(DMA1->INTFR & DMA1_FLAG_TC1))
			;
		DMA1->INTFCR = DMA1_FLAG_TC1;

		printf("%d\n", adc_buffer[0]);

		Delay_Ms(100);
	}
}
