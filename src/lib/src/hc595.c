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


#include "../include/hc595.h"

periferr_t 
_hc595_init(
	__inout hc595 *cont,
	__in volatile uint8_t *ddr,
	__in volatile uint8_t *port,
	__in uint8_t clk_pin,
	__in uint8_t data_pin,
	__in uint8_t lat_pin
	)
{
	uint8_t mask;
	periferr_t result = PERIF_ERR_NONE;

#ifndef NDEBUG
	if(!cont || !ddr || !port) {
		result = PERIF_ERR_INVARG;
		goto exit;
	}
#endif // NDEBUG

	// cache registers
	cont->ddr = ddr;
	cont->port = port;

	// cache pins
	cont->clk_pin = _BV(clk_pin);
	cont->data_pin = _BV(data_pin);
	cont->lat_pin = _BV(lat_pin);

	// cache previous register configuration
	cont->ddr_prev = *cont->ddr;
	cont->port_prev = *cont->port;

	// assign register configuration
	mask = (cont->clk_pin | cont->data_pin 
		| cont->lat_pin);
	FLAG_SET(cont->ddr, mask);
	FLAG_CLEAR(cont->port, mask);

#ifndef NDEBUG
exit:
#endif // NDEBUG
	return result;
}

periferr_t 
hc595_uninit(
	__in hc595 *cont
	)
{
	periferr_t result = PERIF_ERR_NONE;

#ifndef NDEBUG
	if(!cont || !cont->ddr || !cont->port) {
		result = PERIF_ERR_INVARG;
		goto exit;
	}
#endif // NDEBUG

	// restore previous register configuration
	*cont->ddr = cont->ddr_prev;
	*cont->port = cont->port_prev;

	// cleanup context
	cont->ddr = NULL;
	cont->ddr_prev = 0;
	cont->port = NULL;
	cont->port_prev = 0;
	cont->clk_pin = 0;
	cont->data_pin = 0;
	cont->lat_pin = 0;

#ifndef NDEBUG
exit:
#endif // NDEBUG
	return result;
}

periferr_t 
hc595_write(
	__in hc595 *cont,
	__in hc595_frame *frm
	)
{
	uint8_t data, iter = 0;
	periferr_t result = PERIF_ERR_NONE;

#ifndef NDEBUG
	if(!cont || !cont->ddr || !cont->port || !frm) {
		result = PERIF_ERR_INVARG;
		goto exit;
	}
#endif // NDEBUG

	data = frm->data;

	for(; iter < HC595_FRM_LEN; ++iter, data >>= 1) {

		// write single bit to data pin
		FLAG_SET_COND(data & FMR_8_BIT_MSB, cont->port, 
			cont->data_pin);

		// clock out single bit on data pin
		FLAG_SET(cont->port, cont->clk_pin);
		FLAG_CLEAR(cont->port, cont->clk_pin);
	}

	// latch data
	FLAG_SET(cont->port, cont->lat_pin);
	FLAG_CLEAR(cont->port, cont->lat_pin);

#ifndef NDEBUG
exit:
#endif // NDEBUG
	return result;
}
