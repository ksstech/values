/*
 * Copyright 2014-18 Andre M Maree / KSS Technologies (Pty) Ltd.
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
 * values_convert.h
 */

#pragma once

#include	"x_complex_vars.h"

#include	<stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t		xValue2sCompToNormal(uint32_t uValue, int32_t iWidth) ;
uint32_t	xValueNormalTo2sComp(int32_t iValue, int32_t iWidth) ;

int32_t		i32ScaleValue(int32_t Val, int32_t R1lo, int32_t R1hi, int32_t R2lo, int32_t R2hi) ;
uint32_t	u32ScaleValue(uint32_t Val, uint32_t R1lo, uint32_t R1hi, uint32_t R2lo, uint32_t R2hi) ;

inline	int32_t		i32Fit2Range(int32_t Val, int32_t Lo, int32_t Hi) { return (Val <= Lo) ? Lo : (Val >= Hi) ? Hi : Val ; }
inline	uint32_t	u32Fit2Range(uint32_t Val, uint32_t Lo, uint32_t Hi) { return (Val <= Lo) ? Lo : (Val >= Hi) ? Hi : Val ; }

void	ValueConvertUnitTest(void) ;

// #################################################################################################

x64_t	xValuesUpscaleXxx_X64(x32_t x32Var, varform_t VarForm, varsize_t VarSize) ;
x64_t	xValuesUpscaleX32_X64(x32_t x32Var, varform_t VarForm) ;
void	vValuesStoreX64_Xxx(x64_t x64Val, p32_t p32Pntr, varform_t VarForm, varsize_t VarSize) ;
void	vValuesStoreF64_Xxx(double f64Val, p32_t p32Pntr, varform_t VarForm, varsize_t VarSize) ;
x32_t	xValuesFetchXxx_X32(p32_t p32Pntr, varform_t VarForm, varsize_t VarSize) ;
x64_t	xValuesFetchXxx_X64(p32_t p32Pntr, varform_t VarForm, varsize_t VarSize) ;
double	dValuesFetchXxx_F64(p32_t p32Pntr, varform_t VarForm, varsize_t VarSize) ;
void	vValuesReportXxx(const char * pMess, p32_t p32Pntr, varform_t VarForm, varsize_t VarSize) ;
x64_t	xValuesScaleX64(x64_t x64Val, varform_t VarForm, varsize_t VarSize) ;

#ifdef __cplusplus
}
#endif
