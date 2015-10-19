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


#include <util/delay.h>
#define BAUD 9600
#include <util/setbaud.h>
#include "../lib/include/hc595.h"
#include "../lib/include/usart.h"

#define DBG_DDR DEFINE_DDR(B)
#define DBG_PIN _BV(DEFINE_PIN(B, 5))
#define DBG_PORT DEFINE_PORT(B)

#define HC595_BNK F
#define HC595_CLK_PIN 5
#define HC595_DATA_PIN 0
#define HC595_LAT_PIN 1

#define USART_BNK 1
#define USART_ENAB_RX 1
#define USART_ENAB_TX 1
#define USART_LOOPBACK_DEL 100			// ms

void 
debug_exception(
	__in periferr_t err
	)
{
	FLAG_SET_COND(PERIF_ERR(err), &DBG_DDR, DBG_PIN);
	FLAG_SET_COND(PERIF_ERR(err), &DBG_PORT, DBG_PIN);
}

periferr_t 
test_hc595(void)
{
	hc595 cont = { 0 };
	hc595_frame frm = { 0 };
	periferr_t result = PERIF_ERR_NONE;

	result = hc595_init(&cont, HC595_BNK, HC595_CLK_PIN, 
		HC595_DATA_PIN, HC595_LAT_PIN);
	if(PERIF_ERR(result)) {
		goto exit;
	}

	for(;;) {

		result = hc595_write(&cont, &frm);
		if(PERIF_ERR(result)) {
			goto exit;
		}

		++frm.data;
	}

	result = hc595_uninit(&cont);
	if(PERIF_ERR(result)) {
		goto exit;
	}

exit:
	return result;
}

periferr_t 
test_usart(void)
{
	ubrr brr = { 0 };
	ucsra sra = { 0 };
	ucsrb srb = { 0 };
	ucsrc src = { 0 };
	usart cont = { 0 };
	usart_frame frm = { 0 };
	periferr_t result = PERIF_ERR_NONE;

	brr.reg.data = UBRR_VALUE;
#if USE_2X
	sra.reg.part.u2x = 1;
#else
	sra.reg.part.u2x = 0;
#endif // USE_2X
	srb.reg.part.rxen = USART_ENAB_RX;	// enable rx
	srb.reg.part.txen = USART_ENAB_TX;	// enable tx
	src.reg.part.ucsz0 = UCSZ0_8_BIT;	// set character size to 8 bits
	src.reg.part.ucsz1 = UCSZ1_8_BIT;
	srb.reg.part.ucsz2 = UCSZ2_8_BIT;

	result = usart_init(&cont, USART_BNK, sra, srb, src, brr);
	if(PERIF_ERR(result)) {
		goto exit;
	}

	for(;;) {

		result = usart_read(&cont, &frm);
		if(PERIF_ERR(result)) {
			goto exit;
		}

		_delay_ms(USART_LOOPBACK_DEL);

		result = usart_write(&cont, &frm);
		if(PERIF_ERR(result)) {
			goto exit;
		}
	}

	result = usart_uninit(&cont);
	if(PERIF_ERR(result)) {
		goto exit;
	}

exit:
	return result;
}

int 
main(void)
{
	periferr_t result = PERIF_ERR_NONE;

	debug_exception(result);

	// TODO
	result = test_hc595();
	//result = test_usart();
	// ---

	debug_exception(result);

	return result;
}
