
/*
 Copyright (C)
    2011            J. Coliz <maniacbug@ymail.com>
    2015-2019       TMRh20
    2015            spaniakos <spaniakos@gmail.com>
    2015            nerdralph
    2015            zador-blood-stained
    2016            akatran
    2017-2019       Avamander <avamander@gmail.com>
    2019            IkpeohaGodson
    2021            2bndy5

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
*/

#ifndef __RF24_CONFIG_H__
#define __RF24_CONFIG_H__

/*** USER DEFINES:    ***/


/**
 * User access to internally used delay time (in microseconds) during RF24::powerUp()
 * @warning This default value compensates for all supported hardware. Only adjust this if you
 * know your radio's hardware is, in fact, genuine and reliable.
 */
#if !defined(RF24_POWERUP_DELAY)
#define RF24_POWERUP_DELAY	5000
#endif

/**********************/
#define rf24_max(a, b) (a>b?a:b)
#define rf24_min(a, b) (a<b?a:b)

#define RF24_SPI_SPEED 10000000

#include <Arduino.h>
#include <SPI.h>
#define _SPI SPIClass
#define RF24_SPI_PTR
#ifdef SERIAL_DEBUG
    #define IF_SERIAL_DEBUG(x) ({x;})
#else
    #define IF_SERIAL_DEBUG(x)
#endif
#include <avr/pgmspace.h>
#define PRIPSTR "%S"

#ifndef prog_uint16_t
    typedef uint16_t prog_uint16_t;
#endif
#ifndef PSTR
    #define PSTR(x) (x)
#endif
#ifndef printf_P
    #define printf_P printf
#endif
#ifndef strlen_P
    #define strlen_P strlen
#endif
#ifndef PROGMEM
    #define PROGMEM
#endif
#ifndef pgm_read_word
    #define pgm_read_word(p) (*(p))
#endif
#if !defined pgm_read_ptr || defined ARDUINO_ARCH_MBED
    #define pgm_read_ptr(p) (*(p))
#endif
#ifndef PRIPSTR
    #define PRIPSTR "%s"
#endif

#if defined (SPI_HAS_TRANSACTION) && !defined (SPI_UART) && !defined (SOFTSPI)
    #define RF24_SPI_TRANSACTIONS
#endif // defined (SPI_HAS_TRANSACTION) && !defined (SPI_UART) && !defined (SOFTSPI)

#endif // __RF24_CONFIG_H__
