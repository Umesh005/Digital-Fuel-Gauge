#ifndef MAIN_H
#define MAIN_H

#include "includes.h"

#define PPL				70	/* Fuel Price Per Liter */


//FUNCTION PROTOTYPES
static	void	init		(void);
static	void	disptitl 	(void);
static	void	tmr1init	(void);
static	void	EXTINTinit	(void);
static	void	sendmsg		(void);
static	void	FillFuel	(void);
static	int8u	CheckMsg	(void);
static	void	DecodeMsg	(void);

#endif
