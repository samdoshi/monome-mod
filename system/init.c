//ASF
#include <sysclk.h>

#include "gpio.h"
#include "spi.h"

#include "init.h"


extern void init_gpio(void) {
	gpio_enable_gpio_pin(A00);
	gpio_enable_gpio_pin(A01);
	gpio_enable_gpio_pin(A02);
	gpio_enable_gpio_pin(A03);
	gpio_enable_gpio_pin(A04);
	gpio_enable_gpio_pin(A05);
	gpio_enable_gpio_pin(A06);
	gpio_enable_gpio_pin(A07);

	gpio_enable_gpio_pin(B08);
	gpio_enable_gpio_pin(B09);
	gpio_enable_gpio_pin(B10);
	gpio_enable_gpio_pin(B11);
	
	gpio_enable_gpio_pin(NMI);

	gpio_configure_pin(B08, GPIO_DIR_OUTPUT);
	gpio_configure_pin(B09, GPIO_DIR_OUTPUT);
	gpio_configure_pin(B10, GPIO_DIR_OUTPUT);
	gpio_configure_pin(B11, GPIO_DIR_OUTPUT);
}


extern void init_spi (void) {

	sysclk_enable_pba_module(SYSCLK_SPI);

	static const gpio_map_t SPI_GPIO_MAP = {
		{SPI_SCK_PIN,  SPI_SCK_FUNCTION },
		{SPI_MISO_PIN, SPI_MISO_FUNCTION},
		{SPI_MOSI_PIN, SPI_MOSI_FUNCTION},
		{SPI_NPCS0_PIN,  SPI_NPCS0_FUNCTION },
		{SPI_NPCS1_PIN,  SPI_NPCS1_FUNCTION },
		{SPI_NPCS2_PIN,  SPI_NPCS2_FUNCTION },
	};

  // Assign GPIO to SPI.
	gpio_enable_module(SPI_GPIO_MAP, sizeof(SPI_GPIO_MAP) / sizeof(SPI_GPIO_MAP[0]));


	spi_options_t spiOptions = {
		.reg = DAC_SPI,
		.baudrate = 2000000,
		.bits = 8,
		.trans_delay = 0,
		.spck_delay = 0,
		.stay_act = 1,
		.spi_mode = 1,
		.modfdis = 1
	};

  // Initialize as master.
	spi_initMaster(SPI, &spiOptions);
  // Set SPI selection mode: variable_ps, pcs_decode, delay.
	spi_selectionMode(SPI, 0, 0, 0);
  // Enable SPI module.
	spi_enable(SPI);

  // spi_setupChipReg( SPI, &spiOptions, FPBA_HZ );
	spi_setupChipReg(SPI, &spiOptions, sysclk_get_pba_hz() );


  // add ADC chip register
	spiOptions.reg          = ADC_SPI;
	spiOptions.baudrate     = 20000000;
	spiOptions.bits         = 16;
	spiOptions.spi_mode     = 2;
	spiOptions.spck_delay   = 0;
	spiOptions.trans_delay  = 5;
	spiOptions.stay_act     = 0;
	spiOptions.modfdis      = 0;

	spi_setupChipReg( SPI, &spiOptions, FPBA_HZ );


  // add OLED chip register
	spiOptions.reg          = OLED_SPI;
	spiOptions.baudrate     = 40000000;
	spiOptions.bits         = 8;
	spiOptions.spi_mode     = 3;
	spiOptions.spck_delay   = 0;
	spiOptions.trans_delay  = 0;
	spiOptions.stay_act     = 1;
	spiOptions.modfdis      = 1;

	spi_setupChipReg( SPI, &spiOptions, FPBA_HZ );
}

