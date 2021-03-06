/*
 * Copyright (c) 01/2018, Blueberry Tech srl.
 * All rights reserved.
 */

#include "clock.h"
#include "ti-lib.h"
#include "board.h"

#undef SPI_CLK_PORT
#ifndef CC26XX_ENC28J60_CONF_CLK_PORT
#define SPI_CLK_PORT   BOARD_IOID_SPI_SCK
#else
#define SPI_CLK_PORT   CC26XX_ENC28J60_CONF_CLK_PORT
#endif
#ifndef CC26XX_ENC28J60_CONF_CLK_PIN
#define SPI_CLK_BIT    BOARD_SPI_SCK
#else
#define SPI_CLK_BIT    (1 << CC26XX_ENC28J60_CONF_CLK_PIN)
#endif

#undef SPI_MOSI_PORT
#ifndef CC26XX_ENC28J60_CONF_MOSI_PORT
#define SPI_MOSI_PORT   BOARD_IOID_SPI_MOSI
#else
#define SPI_MOSI_PORT   CC26XX_ENC28J60_CONF_MOSI_PORT
#endif
#ifndef CC26XX_ENC28J60_CONF_MOSI_PIN
#define SPI_MOSI_BIT    BOARD_SPI_MOSI
#else
#define SPI_MOSI_BIT    (1 << CC26XX_ENC28J60_CONF_MOSI_PIN)
#endif

#undef SPI_MISO_PORT
#ifndef CC26XX_ENC28J60_CONF_MISO_PORT
#define SPI_MISO_PORT   BOARD_IOID_SPI_MISO
#else
#define SPI_MISO_PORT   CC26XX_ENC28J60_CONF_MISO_PORT
#endif
#ifndef CC26XX_ENC28J60_CONF_MISO_PIN
#define SPI_MISO_BIT    BOARD_SPI_MISO
#else
#define SPI_MISO_BIT    (1 << CC26XX_ENC28J60_CONF_MISO_PIN)
#endif

#undef SPI_CS_PORT
#ifndef CC26XX_ENC28J60_CONF_CS_PORT
#define SPI_CS_PORT   IOID_0
#else
#define SPI_CS_PORT   CC26XX_ENC28J60_CONF_CS_PORT
#endif
#ifndef CC26XX_ENC28J60_CONF_CS_PIN
#define SPI_CS_BIT    (1 << SPI_CS_PORT)
#else
#define SPI_CS_BIT    (1 << CC26XX_ENC28J60_CONF_CS_PIN)
#endif

/* Delay in us */
#define DELAY 10
/*---------------------------------------------------------------------------*/
static void
delay(void)
{
  //  clock_delay_usec(DELAY);
}
/*---------------------------------------------------------------------------*/
void
enc28j60_arch_spi_init(void)
{
  /* Set all pins to GPIO mode */

  /* CS, MOSI, CLK are output pins */
  ti_lib_ioc_pin_type_gpio_output(SPI_CS_PORT);
  ti_lib_ioc_pin_type_gpio_output(SPI_MOSI_PORT);
  ti_lib_ioc_pin_type_gpio_output(SPI_CLK_PORT);

  /* MISO is an input pin */
  ti_lib_ioc_pin_type_gpio_input(SPI_MISO_PORT);

  /* The CS pin is active low, so we set it high when we haven't
     selected the chip. */
  ti_lib_gpio_pin_write(SPI_CS_BIT, 1);

  /* The CLK is active low, we set it high when we aren't using it. */
  ti_lib_gpio_pin_clear(SPI_CLK_BIT);
}
/*---------------------------------------------------------------------------*/
void
enc28j60_arch_spi_select(void)
{
  ti_lib_gpio_pin_write(SPI_CS_BIT, 0);
  /* SPI delay */
  delay();
}
/*---------------------------------------------------------------------------*/
void
enc28j60_arch_spi_deselect(void)
{
  ti_lib_gpio_pin_write(SPI_CS_BIT, 1);
}
/*---------------------------------------------------------------------------*/
uint8_t
enc28j60_arch_spi_write(uint8_t output)
{
  int i;
  uint8_t input;

  input = 0;

  for(i = 0; i < 8; i++) {

    /* Write data on MOSI pin */
    if(output & 0x80) {
      ti_lib_gpio_pin_write(SPI_MOSI_BIT, 1);
    } else {
      ti_lib_gpio_pin_clear(SPI_MOSI_BIT);
    }
    output <<= 1;

    /* Set clock high  */
    ti_lib_gpio_pin_write(SPI_CLK_BIT, 1);

    /* SPI delay */
    delay();

    /* Read data from MISO pin */
    input <<= 1;
    if(ti_lib_gpio_pin_read(SPI_MISO_BIT) != 0) {
      input |= 0x1;
    }

    /* Set clock low */
    ti_lib_gpio_pin_clear(SPI_CLK_BIT);

    /* SPI delay */
    delay();

  }
  return input;
}
/*---------------------------------------------------------------------------*/
uint8_t
enc28j60_arch_spi_read(void)
{
  return enc28j60_arch_spi_write(0);
}
/*---------------------------------------------------------------------------*/
