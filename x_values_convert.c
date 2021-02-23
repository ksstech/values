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
 * x_values_convert.c
 */

#include	"x_values_convert.h"
#include	"printfx.h"									// +x_definitions +stdarg +stdint +stdio
#include	"syslog.h"

#include	"hal_config.h"

#define	debugFLAG					0xC000

#define	debugTWO_COMP				(debugFLAG & 0x0001)
#define	debugSCALE					(debugFLAG & 0x0002)

#define	debugTIMING					(debugFLAG_GLOBAL & debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG_GLOBAL & debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG_GLOBAL & debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG_GLOBAL & debugFLAG & 0x8000)

/*	https://en.wikipedia.org/wiki/One%27s_complement
 *	https://en.wikipedia.org/wiki/Two%27s_complement
 *	https://en.wikipedia.org/wiki/Sign_extension
 */

/**
 * xValue2sCompToNormal() - convert an 8/16/32 bit 2's complement number into signed value
 * @brief		Width 0->8 will be treated as 8bit, 9->16 as 16bit and > 16 as 32bit
 * @brief		Width INCLUDES the sign bit position
 * @brief		Values MUST be left justified, ie sign in MSBit of the given size
 * @param[in]	uValue - unsigned 2's complement value
 * @param[in]	iWidth - integer width to interpret uValue as
 * @return		signed converted value
 */
int32_t	xValue2sCompToNormal(uint32_t uValue, int32_t iWidth) {
	IF_myASSERT(debugPARAM, iWidth <= 32) ;
#if 1
	uint32_t	uMask1	= 2 ^ (iWidth - 1) ;
	uint32_t	uMask2	= 0x80000000 >> (32 - iWidth) ;
	int32_t		iNewVal = -(uValue & uMask1) + (uValue & ~uMask1) ;
	bool		bSign	= uValue & uMask2 ? true : false ;
	IF_PRINT(debugTWO_COMP, "2's Comp: uV=0x%08X  iW=%d  uM1=0x%X  uM2=0x%X  bS=%d  iN=%d\n", uValue, iWidth, uMask1, uMask2, bSign, iNewVal) ;
	return iNewVal ;
#elif 0
	uint32_t	uMask1	= 0x7FFFFFFF >> (32 - iWidth) ;
	uint32_t	uMask2	= 0x80000000 >> (32 - iWidth) ;
	uint32_t	uNewVal	= uValue & uMask1 ;
	bool		bSign	= uValue & uMask2 ? true : false ;
	int32_t		iNewVal = bSign ? -(~uNewVal + 1) : uNewVal ;
	IF_PRINT(debugTWO_COMP, "2s Comp: uV=0x%08X  iW=%d  S=%d  uM1=0x%X  uM2=0x%X  uN=0x%08X  iN=%d\n", uValue, iWidth, bSign, uMask1, uMask2, uNewVal, iNewVal) ;
	IF_myASSERT(debugRESULT, (bSign == 1 && (iNewVal < 0)) || (bSign == 0 && (iNewVal >= 0))) ;
	return iNewVal ;
#elif 0
	uint32_t	uMask1	= 0xFFFFFFFF >> (32 - iWidth) ;
	uint32_t	uMask2	= 0x80000000 >> (32 - iWidth) ;
	bool		Sign	= uValue & uMask2 ? true : false ;
	uint32_t	uNewVal	= uValue & uMask1 ;
	int32_t		iNewVal = (uNewVal & uMask2) ? ((uNewVal & (uMask2 - 1)) - uMask2) : uNewVal ;
	IF_PRINT(debugTWO_COMP, "2s Comp: uV=0x%08X  iW=%d  S=%d  uM1=0x%X  uM2=0x%X  uN=0x%08X  iN=%d\n", uValue, iWidth, Sign, uMask1, uMask2, uNewVal, iNewVal) ;
	IF_myASSERT(debugRESULT, ((Sign == 1) && (iNewVal < 0)) || ((Sign == 0) && (iNewVal >= 0))) ;
	return iNewVal ;
#elif 0
	IF_PRINT(debugTWO_COMP, "uVal=0x%08X  iWid=%d", uValue, iWidth) ;
	uValue	= ~uValue ;
	IF_PRINT(debugTWO_COMP, "  uVal=0x%08X", uValue) ;
	uValue	+= 1 ;
	IF_PRINT(debugTWO_COMP, "  uVal=0x%08X", uValue) ;
	uValue	&= (0xFFFFFFFF >> (32 - iWidth)) ;
	IF_PRINT(debugTWO_COMP, "  uVal=0x%08X\n", uValue) ;
	return uValue ;
#endif
}

//uint32_t	xValueNormalTo2sComp(int32_t iValue, int32_t iWidth) { IF_myASSERT(debugPARAM, iWidth <= 32) ; }

int32_t	i32ScaleValue(int32_t Val, int32_t R1lo, int32_t R1hi, int32_t R2lo, int32_t R2hi) {
	IF_myASSERT(debugPARAM, (R1hi > R1lo) && (R1lo <= Val) && (Val <= R1hi) && (R2lo != R2hi)) ;
	double Factor = ((Val - R1lo) == 0ULL) ? 0.0 : ((double)R1hi - (double)R1lo ) / ( (double)Val - (double)R1lo) ;
	double Newval = (Factor == 0.0) ? (double)R2lo :  (double)R2lo + ((double)R2hi - (double)R2lo ) / Factor ;
	IF_PRINT(debugSCALE, "int32_t Val=%'d R1: %'d -> %'d  R2: %'d -> %'d Fact=%'f New=%'d\n", Val, R1lo, R1hi, R2lo, R2hi, Factor, (int32_t) Newval) ;
	return (int32_t) Newval ;
}

uint32_t u32ScaleValue(uint32_t Val, uint32_t R1lo, uint32_t R1hi, uint32_t R2lo, uint32_t R2hi) {
	IF_myASSERT(debugPARAM, (R1hi > R1lo) && (R1lo <= Val) && (Val <= R1hi) && (R2lo != R2hi)) ;
	double Factor = ((Val - R1lo) == 0ULL) ? 0.0 : ((double)R1hi - (double)R1lo ) / ( (double)Val - (double)R1lo ) ;
	double Newval = (Factor == 0.0) ? (double)R2lo : (double)R2lo + ((double)R2hi - (double)R2lo ) / Factor ;
	IF_PRINT(debugSCALE, "uint32_t Val=%'u R1: %'u -> %'u  R2: %'u -> %'u Fact=%'f New=%'u\n", Val, R1lo, R1hi, R2lo, R2hi, Factor, (uint32_t) Newval) ;
	return (uint32_t) Newval ;
}

// #################################################################################################

x64_t	xValuesUpscaleXxx_X64(x32_t x32Var, varform_t VarForm, varsize_t VarSize) {
	x64_t	x64Var = { 0 } ;
	switch(VarForm) {
	case vfUXX:
		switch(VarSize) {
		case vs08B:			x64Var.u64 = x32Var.x8[0].u8 ;		break ;
		case vs16B:			x64Var.u64 = x32Var.x16[0].u16 ;	break ;
		case vs32B:			x64Var.u64 = x32Var.u32 ;			break ;
		default:			IF_myASSERT(debugPARAM, 0) ;
		}
		break ;
	case vfIXX:
		switch(VarSize) {
		case vs08B:			x64Var.i64 = x32Var.x8[0].i8 ;		break ;
		case vs16B:			x64Var.i64 = x32Var.x16[0].i16 ;	break ;
		case vs32B:			x64Var.i64 = x32Var.i32 ;			break ;
		default:			IF_myASSERT(debugPARAM, 0) ;
		}
		break ;
	case vfFXX:
		switch(VarSize) {
		case vs32B:			x64Var.f64 = x32Var.f32 ;			break ;
		default:			IF_myASSERT(debugPARAM, 0) ;
		}
		break ;
	default:
		myASSERT(0) ;
	}
	return x64Var ;
}

x64_t	xValuesUpscaleX32_X64(x32_t x32Var, varform_t VarForm) {
	IF_myASSERT(debugPARAM, VarForm != vfSXX) ;
	x64_t	x64Var = { 0 } ;
	switch(VarForm) {
	case vfUXX:		x64Var.u64 = x32Var.u32 ;		break ;
	case vfIXX:		x64Var.i64 = x32Var.i32 ;		break ;
	case vfFXX:		x64Var.f64 = x32Var.f32 ;		break ;
	default:		myASSERT(0) ;
	}
	return x64Var ;
}

bool	xValuesVerifyForm_Size(varform_t VarForm, varsize_t VarSize) {
	/* OK = vfFXX if size is 					vs32B or vs64B
	 * OK = vfUXX if size is vs08B or vs16B or	vs32B or vs64B
	 * OK = vfIXX if size is vs08B or vs16B or	vs32B or vs64B
	 */
	return (VarForm == vfFXX && (VarSize == vs08B || VarSize == vs16B)) ? false : true ;
}

void	vValuesStoreX64_Xxx(x64_t x64Val, p32_t p32Pntr, varform_t VarForm, varsize_t VarSize) {
	IF_myASSERT(debugPARAM, (VarForm != vfSXX) && xValuesVerifyForm_Size(VarForm, VarSize)) ;
	switch(VarSize) {
	case vs08B:
		if (VarForm == vfUXX)
			*p32Pntr.pu8	= x64Val.u64 ;
		else if (VarForm == vfIXX)
			*p32Pntr.pi8	= x64Val.i64 ;
		break ;
	case vs16B:
		if (VarForm == vfUXX)
			*p32Pntr.pu16	= x64Val.u64 ;
		else if (VarForm == vfIXX)
			*p32Pntr.pi16	= x64Val.i64 ;
		break ;
	case vs32B:
		if (VarForm == vfUXX)
			*p32Pntr.pu32	= x64Val.u64 ;
		else if (VarForm == vfIXX)
			*p32Pntr.pi32	= x64Val.i64 ;
		else if (VarForm == vfFXX)
			*p32Pntr.pf32	= x64Val.f64 ;
		break ;
	case vs64B:
		if (VarForm == vfUXX)
			*p32Pntr.pu64	= x64Val.u64 ;
		else if (VarForm == vfIXX)
			*p32Pntr.pi64	= x64Val.i64 ;
		else if (VarForm == vfFXX)
			*p32Pntr.pf64	= x64Val.f64 ;
		break ;
	}
}

void	vValuesStoreF64_Xxx(double f64Val, p32_t p32Pntr, varform_t VarForm, varsize_t VarSize) {
	IF_myASSERT(debugPARAM, (VarForm != vfSXX) && xValuesVerifyForm_Size(VarForm, VarSize)) ;
	switch(VarSize) {
	case vs08B:
		if (VarForm == vfUXX)
			*p32Pntr.pu8	= f64Val ;
		else if (VarForm == vfIXX)
			*p32Pntr.pi8	= f64Val ;
		break ;
	case vs16B:
		if (VarForm == vfUXX)
			*p32Pntr.pu16	= f64Val ;
		else if (VarForm == vfIXX)
			*p32Pntr.pi16	= f64Val ;
		break ;
	case vs32B:
		if (VarForm == vfUXX)
			*p32Pntr.pu32	= f64Val ;
		else if (VarForm == vfIXX)
			*p32Pntr.pi32	= f64Val ;
		else if (VarForm == vfFXX)
			*p32Pntr.pf32	= f64Val ;
		break ;
	case vs64B:
		if (VarForm == vfUXX)
			*p32Pntr.pu64	= f64Val ;
		else if (VarForm == vfIXX)
			*p32Pntr.pi64	= f64Val ;
		else if (VarForm == vfFXX)
			*p32Pntr.pf64	= f64Val ;
		break ;
	}
}

x32_t	xValuesFetchXxx_X32(p32_t p32Pntr, varform_t VarForm, varsize_t VarSize) {
	IF_myASSERT(debugPARAM, halCONFIG_inSRAM(p32Pntr.pu32) && INRANGE(vs08B, VarSize, vs32B, varsize_t)) ;
	IF_myASSERT(debugPARAM, (VarForm != vfSXX) && xValuesVerifyForm_Size(VarForm, VarSize)) ;
	x32_t x32Val = { 0 } ;
	switch(VarSize) {
	case vs08B:
		if (VarForm == vfUXX)
			x32Val.u32		= *p32Pntr.pu8 ;
		else if (VarForm == vfIXX)
			x32Val.i32		= *p32Pntr.pi8 ;
		break ;
	case vs16B:
		if (VarForm == vfUXX)
			x32Val.u32		= *p32Pntr.pu16 ;
		else if (VarForm == vfIXX)
			x32Val.i32		= *p32Pntr.pi16 ;
		break ;
	case vs32B:
		if (VarForm == vfUXX)
			x32Val.u32		= *p32Pntr.pu32 ;
		else if (VarForm == vfIXX)
			x32Val.i32		= *p32Pntr.pi32 ;
		else if (VarForm == vfFXX)
			x32Val.f32		= *p32Pntr.pf32 ;
		break ;
	case vs64B:
		break ;
	}
	return x32Val ;
}

x64_t	xValuesFetchXxx_X64(p32_t p32Pntr, varform_t VarForm, varsize_t VarSize) {
 	IF_myASSERT(debugPARAM, halCONFIG_inSRAM(p32Pntr.pu32) && (VarForm != vfSXX) && xValuesVerifyForm_Size(VarForm, VarSize)) ;
	x64_t	x64Val  = { 0 } ;
	switch(VarSize) {
	case vs08B:
		if (VarForm == vfUXX)
			x64Val.u64		= *p32Pntr.pu8 ;
		else if (VarForm == vfIXX)
			x64Val.i64		= *p32Pntr.pi8 ;
		break ;
	case vs16B:
		if (VarForm == vfUXX)
			x64Val.u64		= *p32Pntr.pu16 ;
		else if (VarForm == vfIXX)
			x64Val.i64		= *p32Pntr.pi16 ;
		break ;
	case vs32B:
		if (VarForm == vfUXX)
			x64Val.u64		= *p32Pntr.pu32 ;
		else if (VarForm == vfIXX)
			x64Val.i64		= *p32Pntr.pi32 ;
		else if (VarForm == vfFXX)
			x64Val.f64		= *p32Pntr.pf32 ;
		break ;
	case vs64B:
		if (VarForm == vfUXX)
			x64Val.u64		= *p32Pntr.pu64 ;
		else if (VarForm == vfIXX)
			x64Val.i64		= *p32Pntr.pi64 ;
		else if (VarForm == vfFXX)
			x64Val.f64		= *p32Pntr.pf64 ;
		break ;
	}
	return x64Val ;
}

double	dValuesFetchXxx_F64(p32_t p32Pntr, varform_t VarForm, varsize_t VarSize) {
	x64_t	x64Value = xValuesFetchXxx_X64(p32Pntr, VarForm, VarSize) ;
	if (VarForm == vfUXX) {
		return x64Value.u64 ;
	} else if (VarForm == vfIXX) {
		return x64Value.i64 ;
	} else if (VarForm == vfFXX) {
		return x64Value.f64 ;
	} else {
		IF_myASSERT(debugPARAM, 0) ;
	}
	return 0.0 ;
}

void	vValuesReportXxx(const char * pMess, p32_t p32Pntr, varform_t VarForm, varsize_t VarSize) {
	x64_t	x64Val = xValuesFetchXxx_X64(p32Pntr, VarForm, VarSize) ;
	pMess = pMess == NULL ? "" : pMess ;
	PRINT(" %s %s %s ", pMess, vsName[VarSize], vfName[VarForm]) ;
	switch(VarForm) {
	case vfUXX:			PRINT("%'llu ", x64Val.u64) ;	break ;
	case vfIXX:			PRINT("%'lli ", x64Val.i64) ;	break ;
	case vfFXX:			PRINT("%f ", x64Val.f64) ;		break ;
	default:			SL_ERR(debugAPPL_PLACE) ;
	}
}

/**
 * XXX must document the logic and reasoning behind this....
 * @param x64Val
 * @param VarForm
 * @param VarSize
 * @return
 */
x64_t	xValuesScaleX64(x64_t x64Val, varform_t VarForm, varsize_t VarSize) {
	x64_t	x64Res = { 0 } ;
	if (VarSize == vs64B) {
		x64Res = x64Val ;
	} else if (VarForm == vfFXX) {
		x64Res.x32[0].f32	= x64Val.f64 ;
	} else if (VarForm == vfUXX) {
		switch (VarSize) {
		case vs08B:		x64Res.x8[0].u8	  = x64Val.u64 % (1ULL << 8) ;			break ;
		case vs16B:		x64Res.x16[0].u16 = x64Val.u64 % (1ULL << 16) ;			break ;
		case vs32B:		x64Res.x32[0].u32 = x64Val.u64 % (1ULL << 32) ;			break ;
		default:		IF_myASSERT(debugPARAM, 0) ;
		}
	} else if (VarForm == vfIXX) {
		switch (VarSize) {
		case vs08B:
			x64Res.x8[0].i8	= INRANGE(-128, x64Val.i64, 127, int64_t) ? x64Val.i64 : x64Val.i64 > 0LL ? x64Val.i64 % 127 : x64Val.i64 % 128 ;
			break ;
		case vs16B:
			x64Res.x16[0].i16	= INRANGE(-32768, x64Val.i64, 32767, int64_t) ? x64Val.i64 : x64Val.i64 > 0 ? x64Val.i64 % 32767 : x64Val.i64 % 32768 ;
			break ;
		case vs32B:
			x64Res.x32[0].i32	= INRANGE(-2147483648, x64Val.i64, 2147483647, int64_t) ? x64Val.i64 : x64Val.i64 > 0 ? x64Val.i64 % 2147483647 : x64Val.i64 % 2147483648 ;
			break ;
		default:
			IF_myASSERT(debugPARAM, 0) ;
			break ;
		}
	} else {
		IF_myASSERT(debugPARAM, 0) ;
	}
	return x64Res ;
}

// ##################################### functional tests ##########################################

void	ValuesConvertUnitTest(void) {
#if		(debugTESTS && debugSCALE)
	int32_t iMin = -10000, iMax = +10000 ;
	i32ScaleValue(-9000	, 	iMin,	iMax,	INT32_MIN, INT32_MAX) ;
	i32ScaleValue(-5000	, 	iMin, 	iMax,	INT32_MIN, INT32_MAX) ;
	i32ScaleValue(-1000	, 	iMin,	iMax,	INT32_MIN, INT32_MAX) ;
	i32ScaleValue( 0	,	iMin,	iMax,	INT32_MIN, INT32_MAX) ;
	i32ScaleValue( 10	,	iMin,	iMax,	INT32_MIN, INT32_MAX) ;
	i32ScaleValue( 100	,	iMin,	iMax,	INT32_MIN, INT32_MAX) ;
	i32ScaleValue( 1000	,	iMin,	iMax,	INT32_MIN, INT32_MAX) ;
	i32ScaleValue( 2000	,	iMin,	iMax,	INT32_MIN, INT32_MAX) ;
	i32ScaleValue( 3333	,	iMin,	iMax,	INT32_MIN, INT32_MAX) ;
	i32ScaleValue( 7777	,	iMin,	iMax,	INT32_MIN, INT32_MAX) ;
	i32ScaleValue(10000	,	iMin,	iMax,	INT32_MIN, INT32_MAX) ;
	uint32_t uMin = 10000, uMax = 20000 ;
	u32ScaleValue(10000, 	uMin,	uMax,	UINT32_MIN, UINT32_MAX) ;
	u32ScaleValue(10001, 	uMin, 	uMax,	UINT32_MIN, UINT32_MAX) ;
	u32ScaleValue(11000, 	uMin,	uMax,	UINT32_MIN, UINT32_MAX) ;
	u32ScaleValue(13000,	uMin,	uMax,	UINT32_MIN, UINT32_MAX) ;
	u32ScaleValue(15000,	uMin,	uMax,	UINT32_MIN, UINT32_MAX) ;
	u32ScaleValue(17500,	uMin,	uMax,	UINT32_MIN, UINT32_MAX) ;
	u32ScaleValue(18000,	uMin,	uMax,	UINT32_MIN, UINT32_MAX) ;
	u32ScaleValue(18500,	uMin,	uMax,	UINT32_MIN, UINT32_MAX) ;
	u32ScaleValue(19000,	uMin,	uMax,	UINT32_MIN, UINT32_MAX) ;
	u32ScaleValue(19500,	uMin,	uMax,	UINT32_MIN, UINT32_MAX) ;
	u32ScaleValue(20000,	uMin,	uMax,	UINT32_MIN, UINT32_MAX) ;
#elif	(debugTESTS && debugTWO_COMP)

#endif
}
