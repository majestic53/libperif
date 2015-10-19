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

#ifndef HC595_H_
#define HC595_H_

#include "../include/perifdef.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define HC595_FRM_LEN FRM_8_BIT

/*
 * Holds hc595 session information
 */
typedef struct __attribute__((packed)) {

	// control/data registers
	volatile uint8_t *ddr;
	uint8_t ddr_prev;
	volatile uint8_t *port;
	uint8_t port_prev;

	// data pins
	uint8_t clk_pin;
	uint8_t data_pin;
	uint8_t lat_pin;
} hc595;

/*
 * Holds hc595 frame data
 */
typedef struct __attribute__((packed)) {
	uint8_t data;
} hc595_frame;

/*
 * Initialize hc595 session
 * @param _CONT_ valid pointer to a user supplied context
 * @param _BNK_ hc595 register bank (typically A - F)
 * @param _CLK_ clock pin
 * @param _DATA_ data pin
 * @param _LAT_ latch pin
 * @return PERIF_ERR_NONE on success
 */
#define hc595_init(_CONT_, _BNK_, _CLK_, _DATA_, _LAT_) \
	_hc595_init(_CONT_, &DEFINE_DDR(_BNK_), \
	&DEFINE_PORT(_BNK_), _CLK_, _DATA_, _LAT_)

periferr_t _hc595_init(
	__inout hc595 *cont,
	__in volatile uint8_t *ddr,
	__in volatile uint8_t *port,
	__in uint8_t clk_pin,
	__in uint8_t data_pin,
	__in uint8_t lat_pin
	);

/*
 * Uninitialize hc595 session
 * @param cont valid pointer to a user supplied context
 * @return PERIF_ERR_NONE on success
 */
periferr_t hc595_uninit(
	__in hc595 *cont
	);

/*
 * Write frame to hc595 session
 * @param cont valid pointer to a user supplied context
 * @param frm frame to write
 * @return PERIF_ERR_NONE on success
 */
periferr_t hc595_write(
	__in hc595 *cont,
	__in hc595_frame *frm
	);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HC595_H_
