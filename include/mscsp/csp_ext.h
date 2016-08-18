/*
[]=========================================================================[]

	Copyright(C) 2000-2004, Feitian Technologies Co., Ltd.
	ePass Team
	All rights reserved.

FILE:
	csp_ext.h

DESC:
	

REVISION:
	2004-01-14 [Apex Liu]
		Created.
	2004-07-13 [Skybird Le]
		Add SSF33 support.
[]=========================================================================[]
*/

#ifndef __EPSNG_CSP_CONST_FEITIAN_H__
#define __EPSNG_CSP_CONST_FEITIAN_H__

// algorithm extended by Ftsafe
#define ALG_SID_SSF33			33
#define CALG_SSF33			(ALG_CLASS_DATA_ENCRYPT|ALG_TYPE_BLOCK|ALG_SID_SSF33)
#define CALG_IDEA           (ALG_CLASS_DATA_ENCRYPT|ALG_TYPE_BLOCK|ALG_SID_IDEA)
#define CALG_AES            (ALG_CLASS_DATA_ENCRYPT|ALG_TYPE_BLOCK|ALG_SID_AES)

#define ALG_SID_SCB2			40
// Mechanism for SCB2
#define CALG_SCB2			(ALG_CLASS_DATA_ENCRYPT|ALG_TYPE_BLOCK|ALG_SID_SCB2)
#endif // __EPS2K_CSP_CONST_FEITIAN_H__
