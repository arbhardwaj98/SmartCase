#ifndef STDAFX_H
#define STDAFX_H

#include "stm8s.h"
#include "stm8s_clk.h"
#include "stm8s_gpio.h"
#include "stm8s_spi.h"

void init_spi( void );
void spi_write_data_8t( uint8_t *, uint8_t );

uint8_t SPI_read(uint8_t ReadAddr);
void SPI_write(uint8_t WriteAddr, uint8_t value);
#endif // STDAFX_H
