/*
 * Copyright 2014-21 Andre M. Maree / KSS Technologies (Pty) Ltd.
 */

/*
 * values_convert.h
 */

#pragma once

#include	"x_complex_vars.h"

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
void	vValuesStoreX64_Xxx(x64_t x64Val, px_t px, varform_t VarForm, varsize_t VarSize) ;
void	vValuesStoreF64_Xxx(double f64Val, px_t px, varform_t VarForm, varsize_t VarSize) ;
x32_t	xValuesFetchXxx_X32(px_t px, varform_t VarForm, varsize_t VarSize) ;
x64_t	xValuesFetchXxx_X64(px_t px, varform_t VarForm, varsize_t VarSize) ;
double	dValuesFetchXxx_F64(px_t px, varform_t VarForm, varsize_t VarSize) ;
void	vValuesReportXxx(const char * pMess, px_t px, varform_t VarForm, varsize_t VarSize) ;
x64_t	xValuesScaleX64(x64_t x64Val, varform_t VarForm, varsize_t VarSize) ;

#ifdef __cplusplus
}
#endif
