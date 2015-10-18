/**
 * libperif
 * Copyright (C) 2015 David Jolly
 * ----------------------
 *
 * libperif is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libperif is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef USART_H_
#define USART_H_

#include "../include/perifdef.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum {
	UMSEL_ASYNC = 0,		// async operation
	UMSEL_SYNC,			// sync operation
};

enum {
	UPM0_DISABLE = 0,		// disable parity
	UPM1_DISABLE = 0,
	UPM0_EVEN_PARITY = 0,		// even parity
	UPM1_EVEN_PARITY = 1,
	UMP1_ODD_PARITY = 1,		// odd parity
	UMP0_ODD_PARITY = 1,
};

enum {
	USBS_1_BIT = 0,			// 1 stop bit
	USBS_2_BIT,			// 2 stop bits
};

enum {
	UCSZ0_5_BIT = 0,		// 5 bit character
	UCSZ1_5_BIT = 0,
	UCSZ2_5_BIT = 0,
	UCSZ0_6_BIT = 1,		// 6 bit character
	UCSZ1_6_BIT = 0,
	UCSZ2_6_BIT = 0,
	UCSZ0_7_BIT = 0,		// 7 bit character
	UCSZ1_7_BIT = 1,
	UCSZ2_7_BIT = 0,
	UCSZ0_8_BIT = 1,		// 8 bit character
	UCSZ1_8_BIT = 1,
	UCSZ2_8_BIT = 0,
	UCSZ0_9_BIT = 1,		// 9 bit character
	UCSZ1_9_BIT = 1,
	UCSZ2_9_BIT = 1,
};

enum {
	UCPOL_RX_FALL_TX_RISE = 0,	// clock polarity
	UCPOL_RX_RISE_TX_FALL,
};

typedef struct __attribute__((packed)) {
	uint16_t baud : 12;		// baud rate
	uint16_t res : 3;
	uint16_t ursel : 1;		// register select
} ubrr_part;

typedef struct __attribute__((packed)) {
	union {
		ubrr_part part;
		uint16_t data;
	} reg;
} ubrr;

typedef struct __attribute__((packed)) {
	uint8_t mpcm : 1;		// multi-processor mode
	uint8_t u2x : 1;		// double transmission speed
	uint8_t pe : 1;			// parity error flag
	uint8_t dor : 1;		// data overrun flag
	uint8_t fe : 1;			// frame error flag
	uint8_t udre : 1;		// data register empty flag
	uint8_t txc : 1;		// transmit complete flag
	uint8_t rxc : 1;		// receive complete flag
} ucsra_part;

typedef struct __attribute__((packed)) {
	union {
		ucsra_part part;
		uint8_t data;
	} reg;
} ucsra;

typedef struct __attribute__((packed)) {
	uint8_t txb8 : 1;		// 9th data bit
	uint8_t rxb8 : 1;		// 9th data bit
	uint8_t ucsz2 : 1;		// character size (1:3)
	uint8_t txen : 1;		// transmit enable flag
	uint8_t rxen : 1;		// receive enable flag
	uint8_t udrie : 1;		// data register empty interrupt enable flag
	uint8_t txcie : 1;		// transmit complete interrupt enable flag
	uint8_t rxcie : 1;		// receieve complete interrupt enable flag
} ucsrb_part;

typedef struct __attribute__((packed)) {
	union {
		ucsrb_part part;
		uint8_t data;
	} reg;
} ucsrb;

typedef struct __attribute__((packed)) {
	uint8_t ucpol : 1;		// clock polarity
	uint8_t ucsz0 : 1;		// character size (1:3)
	uint8_t ucsz1 : 1;		// character size (1:3)
	uint8_t usbs : 1;		// stop bit select
	uint8_t upm0 : 1;		// parity mode (1:2)
	uint8_t upm1 : 1;		// parity mode (1:2)
	uint8_t umsel : 1;		// usart mode
	uint8_t ursel : 1;		// register select
} ucsrc_part;

typedef struct __attribute__((packed)) {
	union {
		ucsrc_part part;
		uint8_t data;
	} reg;
} ucsrc;

typedef struct __attribute__((packed)) {

	// baud rate register
	volatile uint16_t *brr;
	uint16_t brr_prev;

	// serial data register
	volatile uint8_t *udr;
	uint8_t udr_prev;
	uint8_t rxc_pin;
	uint8_t udre_pin;

	// control/status registers
	volatile uint8_t *sra;
	uint8_t sra_prev;
	volatile uint8_t *srb;
	uint8_t srb_prev;
	volatile uint8_t *src;
	uint8_t src_prev;
} usart;

typedef struct __attribute__((packed)) {
	uint8_t high;
	uint8_t low;
} usart_frame_part;

typedef struct __attribute__((packed)) {
	union {
		usart_frame_part part;
		uint16_t data;
	} frm;
} usart_frame;

/*
 * Initialize usart session
 * @param _CONT_ valid pointer to a user supplied context
 * @param _BNK_ usart register bank (typically 0 or 1)
 * @param _SRA_ ucsra control/status register value
 * @param _SRB_ ucsrb control/status register value
 * @param _SRC_ ucsrc control/status register value
 * @param _BRR_ ubrr baud rate register value
 * @return PERIF_ERR_NONE on success
 */
#define usart_init(_CONT_, _BNK_, _SRA_, _SRB_, _SRC_, _BRR_) \
	_usart_init(_CONT_, &DEFINE_REG_EXT(_BNK_, UCSR, A), \
	&DEFINE_REG_EXT(_BNK_, UCSR, B), &DEFINE_REG_EXT(_BNK_, UCSR, C), \
	&DEFINE_REG(_BNK_, UBRR), &DEFINE_REG(_BNK_, UDR), \
	_SRA_, _SRB_, _SRC_, _BRR_, 0, DEFINE_REG(_BNK_, RXC), \
	DEFINE_REG(_BNK_, UDRE))

periferr_t _usart_init(
	__inout usart *cont,
	__in volatile uint8_t *sra,
	__in volatile uint8_t *srb,
	__in volatile uint8_t *src,
	__in volatile uint16_t *brr,
	__in volatile uint8_t *udr,
	__in ucsra sra_cfg,
	__in ucsrb srb_cfg,
	__in ucsrc src_cfg,
	__in ubrr brr_cfg,
	__in uint8_t udr_val,
	__in uint8_t rxc_pin,
	__in uint8_t udre_pin
	);

/*
 * Read frame from a usart session
 * @param cont valid pointer to a user supplied context
 * @param frm valid pointer to a user supplied frame
 * @return PERIF_ERR_NONE on success
 */
periferr_t usart_read(
	__in usart *cont,
	__inout usart_frame *frm
	);

/*
 * Uninitialize usart session
 * @param cont valid pointer to a user supplied context
 * @return PERIF_ERR_NONE on success
 */
periferr_t usart_uninit(
	__inout usart *cont
	);

/*
 * Write frame to a usart session
 * @param cont valid pointer to a user supplied context
 * @param frm frame to write
 * @return PERIF_ERR_NONE on success
 */
periferr_t usart_write(
	__in usart *cont,
	__in usart_frame frm
	);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // USART_H_
