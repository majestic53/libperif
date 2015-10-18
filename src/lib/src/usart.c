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

#include "../include/usart.h"

#define FLAG_RXB8 _BV(1)
#define FLAG_TXB8 _BV(0)
#define FLAG_UCSZ2 _BV(2)

periferr_t 
_usart_init(
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
	)
{
	periferr_t result = PERIF_ERR_NONE;

#ifndef NDEBUG
	if(!cont || !sra || !srb || !src || !brr || !udr) {
		result = PERIF_ERR_INVARG;
		goto exit;
	}
#endif // NDEBUG

	// cache registers
	cont->brr = brr;
	cont->sra = sra;
	cont->srb = srb;
	cont->src = src;
	cont->udr = udr;

	// cache pins
	cont->rxc_pin = rxc_pin;
	cont->udre_pin = udre_pin;

	// cache previous register configuration
	cont->brr_prev = *cont->brr;
	cont->sra_prev = *cont->sra;
	cont->srb_prev = *cont->srb;
	cont->src_prev = *cont->src;
	cont->udr_prev = *cont->udr;

	// assign register configurations
	*cont->brr = brr_cfg.reg.data;
	*cont->sra = sra_cfg.reg.data;
	*cont->srb = srb_cfg.reg.data;
	*cont->src = src_cfg.reg.data;
	*cont->udr = udr_val;

#ifndef NDEBUG
exit:
#endif // NDEBUG
	return result;
}

periferr_t 
usart_read(
	__in usart *cont,
	__inout usart_frame *frm
	)
{
	periferr_t result = PERIF_ERR_NONE;

#ifndef NDEBUG
	if(!cont || !cont->sra || !cont->srb || !cont->src 
			|| !cont->brr || !cont->udr || !frm) {
		result = PERIF_ERR_INVARG;
		goto exit;
	}
#endif // NDEBUG

	frm->frm.data = 0;
	loop_until_bit_is_set(*cont->sra, cont->rxc_pin);

	if(FLAG_CHECK(cont->srb, FLAG_UCSZ2)) {

		// read 9th bit (if applicable)
		frm->frm.data 
			+= (FLAG_CHECK(cont->srb, FLAG_RXB8) ? (UINT8_MAX + 1) : 0);
	}

	// read received bits
	frm->frm.data += *cont->udr;

#ifndef NDEBUG
exit:
#endif // NDEBUG
	return result;
}

periferr_t 
usart_uninit(
	__inout usart *cont
	)
{
	periferr_t result = PERIF_ERR_NONE;

#ifndef NDEBUG
	if(!cont) {
		result = PERIF_ERR_INVARG;
		goto exit;
	}
#endif // NDEBUG

	// restore previous register configurations
	*cont->brr = cont->brr_prev;
	*cont->sra = cont->sra_prev;
	*cont->srb = cont->srb_prev;
	*cont->src = cont->src_prev;
	*cont->udr = cont->udr_prev;

	// cleanup context
	cont->brr = NULL;
	cont->sra = NULL;
	cont->srb = NULL;
	cont->src = NULL;
	cont->udr = NULL;
	cont->brr_prev = 0;
	cont->sra_prev = 0;
	cont->srb_prev = 0;
	cont->src_prev = 0;
	cont->udr_prev = 0;
	cont->rxc_pin = 0;
	cont->udre_pin = 0;

#ifndef NDEBUG
exit:
#endif // NDEBUG
	return result;
}

periferr_t 
usart_write(
	__in usart *cont,
	__in usart_frame frm
	)
{
	periferr_t result = PERIF_ERR_NONE;

#ifndef NDEBUG
	if(!cont || !cont->sra || !cont->srb || !cont->src 
			|| !cont->brr || !cont->udr) {
		result = PERIF_ERR_INVARG;
		goto exit;
	}
#endif // NDEBUG

	loop_until_bit_is_set(*cont->sra, cont->udre_pin);

	if(FLAG_CHECK(cont->srb, FLAG_UCSZ2)) {

		// write 9th bit (if applicable)
		FLAG_SET_COND(frm.frm.data & (UINT8_MAX + 1), 
			cont->srb, FLAG_TXB8);
	}

	// write bits
	*cont->udr = (frm.frm.data & UINT8_MAX);

#ifndef NDEBUG
exit:
#endif // NDEBUG
	return result;
}
