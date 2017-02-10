#include "mps20n40.h"

extern int8u lcdptr;
int8u EEMEM FUEL_ADDR[25];

void ConfigSens(void) {
	char ZeroFuel[50];
	int8u i;
	int32u Pres = 0;
	
	lcdclr();
	lcdws("Fuel Sens: ");	
	for (i = 0; i < 8; i++)
		Pres += HX711Read();
		
	Pres >>= 3;
	
	sprintf(ZeroFuel,"%ld", Pres);
	eeprom_update_block ((const void *)ZeroFuel, (void *)FUEL_ADDR, 25);
	
	lcdws("OK");
	dlyms(2000);
}
double MeasPres(int8u disp) {
	char ZeroFuel[25];
	char s[10];
	int8u i;
	int32u Pres = 0;
	double PresFlo;
	
	eeprom_read_block ((void *)ZeroFuel , (const void *)FUEL_ADDR, 25);
/*	lcdclr();
	lcdws(ZeroFuel);	
	lcdr2();
	PresFlo = atof(ZeroFuel);	
	sprintf(s,"%0.1f", PresFlo);
	lcdws(s);	
	for(;;); */
	
	for (i = 0; i < 8; i++)
		Pres += HX711Read();
	Pres >>= 3;
	
	PresFlo = (Pres - (atof(ZeroFuel) - PRES_DELTA)) * ADC_CONST; 
	
	if (disp){
		sprintf(s,"%0.1f", PresFlo);
	//	ftoa(PresFlo, s, 1);
		//ltoa(Pres,s);
		lcdptr = disp;
		lcdws("    ");
		lcdptr = disp;
		lcdws(s);
	}
	return PresFlo;
}