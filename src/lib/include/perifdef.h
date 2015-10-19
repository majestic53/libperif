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

#ifndef PERIFDEF_H_
#define PERIFDEF_H_

#define NDEBUG

#include <stddef.h>
#include <stdint.h>
#include <avr/io.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef __in
#define __in
#endif // __in
#ifndef __inout
#define __inout
#endif // __inout
#ifndef __out
#define __out
#endif // __out

#define FRM_8_BIT 8
#define FMR_8_BIT_LSB 0x1
#define FMR_8_BIT_MSB 0x80

/*
 * Define ddr register (DDR<B>)
 * @param _BNK_ ddr bank (typically A - F)
 */
#define DEFINE_DDR(_BNK_) DDR ## _BNK_

/*
 * Define port register (PORT<B>)
 * @param _BNK_ port bank (typically A - F)
 */
#define DEFINE_PORT(_BNK_) PORT ## _BNK_

/*
 * Define pin (P<B><P>)
 *   NOTE: When using pin definitions in masks, wrap pin 
 *   in _BV() macro
 * @param _BNK_ pin bank (typically A - F)
 * @param _PIN_ pin offset (0 - 7)
 */
#define DEFINE_PIN(_BNK_, _PIN_) P ## _BNK_ ## _PIN_

/*
 * Define register (<R><B>)
 * @param _BNK_ register bank (typically 0 - N)
 * @param _REG_ register name
 */
#define DEFINE_REG(_BNK_, _REG_) _REG_ ## _BNK_

/*
 * Define extended register (<R><B><E>)
 * @param _BNK_ register bank (typically 0 - N)
 * @param _REG_ register name
 * @param _EXT_ register extension (typcially A - F)
 */
#define DEFINE_REG_EXT(_BNK_, _REG_, _EXT_) _REG_ ## _BNK_ ## _EXT_

/*
 * Check register flag value
 * @param _REG_ register name
 * @param _FLG_ flag offset/mask (0 - 0xff)
 */
#define FLAG_CHECK(_REG_, _FLG_) (*(_REG_) & (_FLG_))

/*
 * Clear register flag value
 * @param _REG_ register name
 * @param _FLG_ flag offset/mask (0 - 0xff)
 */
#define FLAG_CLEAR(_REG_, _FLG_) (*(_REG_) &= ~(_FLG_))

/*
 * Set register flag value
 * @param _REG_ register name
 * @param _FLG_ flag offset/mask (0 - 0xff)
 */
#define FLAG_SET(_REG_, _FLG_) (*(_REG_) |= (_FLG_))

/*
 * Conditionally set register flag value
 * @param _COND_ condition
 * @param _REG_ register name
 * @param _FLG_ flag offset/mask (0 - 0xff)
 */
#define FLAG_SET_COND(_COND_, _REG_, _FLG_) { \
	if(_COND_) { \
		FLAG_SET(_REG_, _FLG_); \
	} else { \
		FLAG_CLEAR(_REG_, _FLG_); \
	}\
	}

typedef enum {
	PERIF_ERR_NONE = 0,
	PERIF_ERR_INVARG,

	// add additional error codes here
} periferr_t;

/*
 * library error check
 * @param _ERR_ error code
 * @return true if an error occurred, false otherwise
 */
#define PERIF_ERR(_ERR_) ((_ERR_) != PERIF_ERR_NONE)

#define PERIF_VER_MAJ 0
#define PERIF_VER_MIN 1
#define PERIF_VER_REV 2
#define PERIF_VER_WEEK 1542

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // PERIFDEF_H_
