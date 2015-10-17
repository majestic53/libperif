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

#define BAUD 9600
#include <util/delay.h>
#include <util/setbaud.h>
#include "../lib/include/usart.h"

int 
main(void)
{
	uint16_t val;
	ubrr brr = { 0 };
	ucsra sra = { 0 };
	ucsrb srb = { 0 };
	ucsrc src = { 0 };
	usart cont = { 0 };
	periferr_t result = PERIF_ERR_NONE;

	brr.reg.data = UBRR_VALUE;
#if USE_2X
	sra.reg.part.u2x = 1;
#else
	sra.reg.part.u2x = 0;
#endif // USE_2X
	srb.reg.part.rxen = 1;			// enable rx
	srb.reg.part.txen = 1;			// enable tx
	src.reg.part.ucsz0 = UCSZ0_8_BIT;	// set character size to 8 bits
	src.reg.part.ucsz1 = UCSZ1_8_BIT;
	srb.reg.part.ucsz2 = UCSZ2_8_BIT;

	// TODO: DEBUG
	DDRB &= ~_BV(PB5);
	PORTB &= ~_BV(PB5);
	// ---

	result = usart_init(&cont, 1, sra, srb, src, brr, 0);
	if(PERIF_ERR(result)) {
		goto exit;
	}

	for(;;) {

		result = usart_read(&cont, &val);
		if(PERIF_ERR(result)) {
			goto exit;
		}

		_delay_ms(100);

		result = usart_write(&cont, val);
		if(PERIF_ERR(result)) {
			goto exit;
		}
	}

	result = usart_uninit(&cont);
	if(PERIF_ERR(result)) {
		goto exit;
	}

exit:

	// TODO: DEBUG
	if(PERIF_ERR(result)) {
		DDRB |= _BV(PB5);
		PORTB |= _BV(PB5);
	}
	// ---

	return result;
}
