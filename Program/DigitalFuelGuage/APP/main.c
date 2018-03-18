/*	Main.c: Application program for Digital Fuel Gauge	*/
/*	Written By	: Prashanth BS (info@bspembed.com)		*/
/*	Demo at		: www.youtube.com/c/bspembed			*/
#include "main.h"

struct App Flag;
extern int8u lcdptr;
double FuelAdded;
int Amount;
extern char PhNum[];

int main(void) {
	init();
	while (TRUE) {
		if (Flag.Meas) {
			MeasFuel(0xc7);
			Flag.Meas = FALSE;
		}
		if (Flag.Fuel)
			FillFuel();
		if (Flag.Msg) 
			DecodeMsg();
		sleep_cpu();
	}
	return 0;
}
static void init(void) {
	Flag.Fuel = FALSE;
	Flag.Meas = FALSE;
	buzinit();
	ledinit();
	beep(2,100);
	lcdinit();
	HX711init();
	uartinit();
	EXTINTinit();
	tmr1init();
	GPSInit();
	GSMEn();
	GSMinit();
	EnUARTInt();
	if ((INTPIN & _BV(INT0_PIN)) == 0)
		ConfigSens();
	disptitl();
	sei();
	sleep_enable();
	sleep_cpu();
}
static void disptitl(void) {
	lcdclr();
	lcdws("DigitalFuelGauge");
	lcdr2();
	lcdws("  Fuel:     Lts");
}
static void tmr1init(void) {
	TCNT1H   = 225;					/* overflows at every 250msec */
	TCNT1L   = 123;
	TIMSK   |= _BV(TOIE1);			/* ENABLE OVERFLOW INTERRUPT */
	TCCR1A   = 0x00;
	TCCR1B  |= _BV(CS12);			 /* PRESCALAR BY 256 */
}
static void EXTINTinit(void) {
	INTDDR 	&= ~_BV(INT0_PIN);
	INTPORT |= _BV(INT0_PIN);
	GICR	|= _BV(INT0);		//ENABLE EXTERNAL INTERRUPT
	MCUCR	|= _BV(ISC01);		//FALLING EDGE INTERRUPT
}
/* overflows at every 250msec */
ISR(TIMER1_OVF_vect) { 
	static int8u i, j;
	TCNT1H = 0xD3;
	TCNT1L = 0x00;
	if (!Flag.Meas && ++j >= 4) {
		Flag.Meas = TRUE;
		j = 0;
	}
	if (++i >= 20) 
		 i = 0;
	switch(i) {
		case 0: case 2: ledon(); break;
		case 1: case 3: ledoff(); break;
	} 
}
ISR(INT0_vect) {
	Flag.Fuel	= 1;
	GICR		|= _BV(INT0);
}
static void FillFuel(void) {
	double NewFuelRd, OldFuelRd;
	char tmpstr[10];
	Flag.Fuel = FALSE;
	GICR &=  ~_BV(INT0);
	MCUCR &= ~_BV(ISC01);
	DisUARTInt();
	beep(1,100);
	OldFuelRd = MeasPres(0);
	lcdclr();
	lcdws(" Fill The Fuel");
	lcdr2();
	lcdws( "Press SW A'Compl");
	while (INTPIN & _BV(INT0_PIN));
	beep(1,100);
	NewFuelRd = MeasPres(0);
	lcdclr();
	FuelAdded = NewFuelRd - OldFuelRd;
	if (FuelAdded > 0.1) {
		lcdws("Fuel Added:      Lts");
		ftoa(FuelAdded, tmpstr,2);
		lcdptr = 0x8b;
		lcdws(tmpstr);
		lcdr2();
		lcdws( "Amount:      Rs");
		Amount = FuelAdded * PPL;
		itoa(Amount, tmpstr);
		lcdptr = 0xc7;
		lcdws(tmpstr);
		beep(1,100);
		dlyms(2000);
		lcdclrr(1);
		sendmsg();
	} else {
		lcdws ( "No Fuel Added! " );
		beep(1,250);
		dlyms(2000);
	}
	disptitl();
	GICR |=  _BV(INT0);
	MCUCR |= _BV(ISC01);
	EnUARTInt();
}
static void sendmsg (void) {
		int8u gsmmsg[75];
		char tmpstr[10];
		gsmmsg[0] = '\0';
		ftoa(FuelAdded, tmpstr,2);
		strcat(gsmmsg, tmpstr);
		strcat(gsmmsg,"Lts & ");
		itoa(Amount,tmpstr);
		strcat(gsmmsg,tmpstr);
		strcat (gsmmsg, "Rs of fuel filled at");
		SendLinkLoc(PhNum1,gsmmsg);
		SendLinkLoc(PhNum2,gsmmsg);
}
static void DecodeMsg(void) {
	DisUARTInt();
	Flag.Msg = FALSE;
	lcdclrr(1);
	lcdws("Message Received");
	beep(1,250);
	switch (CheckMsg()){
		case 1:	SendLinkLoc(PhNum,"Your Vehicle is Located at "); break;
		default: beep(1,500); lcdclrr(1); break;
	}
	disptitl();
	EnUARTInt();
}
static int8u CheckMsg(void) {
	if ((!strcmp(PhNum, PhNum1)) || (!strcmp(PhNum,PhNum2)))
		return 1;
	else
		return 0;
}