/*
 * Copyright 2014-19 Andre M Maree / KSS Technologies (Pty) Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/*
 * x_values_to_string.c
 */

#include	"x_definitions.h"
#include	"x_values_to_string.h"

/* ################################ Decimal number conversions ################################## */

/*
 * xU32ToDecStr
 * \brief		convert a single unsigned 32 bit integer to a decimal string representation
 * \param[in]	Value - unsigned byte to convert
 * \param[in]	pBuf - pointer to buffer where to store
 * return		number of bytes stored in buffer (excluding terminating NUL
 */
int32_t		xU32ToDecStr(uint32_t Value, char * pBuf) {
	int32_t	Len = 0 ;
	if (Value) {
		uint32_t	iTemp, Div = 1000000000UL ;
		do {
			iTemp = Value / Div ;
			if (iTemp != 0 || Len > 0) {
				*pBuf++	= iTemp + CHR_0 ;
				Value 	-= iTemp * Div ;
				++Len ;
			}
			Div /= 10 ;
		} while (Div) ;
	} else {
		*pBuf++ = CHR_0 ;								// answer is single '0'
		++Len ;
	}
	*pBuf	= CHR_NUL ;									// terminate
	return Len ;
}
