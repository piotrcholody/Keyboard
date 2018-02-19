#include "Board_LED.h"                  // ::Board Support:LED
#include "Board_Joystick.h"             // ::Board Support:Joystick
#include "LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "Open1768_LCD.h"
#include <stdio.h>
#include <stdlib.h>
#include "LCD_ILI9325.h"
#include "asciiLib.h"
#include <cmath>
#include "TP_Open1768.h"

#define M_PI 3.14159265358979323846

uint8_t record[14950];
int counter = 0;
int counter2 = 0;
uint8_t key = 0;

uint32_t table[] = { 512,528,544,560,576,592,607,623,
639,654,670,685,700,715,729,744,
758,772,786,799,812,825,838,850,
862,873,884,895,906,916,925,935,
943,952,960,967,974,981,987,993,
998,1003,1007,1011,1014,1017,1019,1021,
1022,1023,1023,1023,1022,1021,1019,1017,
1014,1011,1007,1003,998,993,987,981,
974,967,960,952,943,935,925,916,
906,895,884,873,862,850,838,825,
812,799,786,772,758,744,729,715,
700,685,670,654,639,623,607,592,
576,560,544,528,512,495,479,463,
447,431,416,400,384,369,353,338,
323,308,294,279,265,251,237,224,
211,198,185,173,161,150,139,128,
117,107,98,88,80,71,63,56,
49,42,36,30,25,20,16,12,
9,6,4,2,1,0,0,0,
1,2,4,6,9,12,16,20,
25,30,36,42,49,56,63,71,
80,88,98,107,117,128,139,150,
161,173,185,198,211,224,237,251,
265,279,294,308,323,338,353,369,
384,400,416,431,447,463,479,495 };

static int value = 0;
bool flag = false;


void play(int i) {
	if (record[i] == 0) {
		key = 0;
	}
	else if (record[i] == 1) {
		key = 1;
		LPC_TIM0->MR0 = freq(262);
	}
	else if (record[i] == 2) {
		key = 2;
		LPC_TIM0->MR0 = freq(277);
	}
	else if (record[i] == 3) {
		key = 3;
		LPC_TIM0->MR0 = freq(294);
	}
	else if (record[i] == 4) {
		key = 4;
		LPC_TIM0->MR0 = freq(311);
	}
	else if (record[i] == 5) {
		key = 5;
		LPC_TIM0->MR0 = freq(330);
	}
	else if (record[i] == 6) {
		key = 6;
		LPC_TIM0->MR0 = freq(349);
	}
	else if (record[i] == 7) {
		key = 7;
		LPC_TIM0->MR0 = freq(370);
	}
	else if (record[i] == 8) {
		key = 8;
		LPC_TIM0->MR0 = freq(392);
	}
	else if (record[i] == 9) {
		key = 9;
		LPC_TIM0->MR0 = freq(415);
	}
	else if (record[i] == 10) {
		key = 10;
		LPC_TIM0->MR0 = freq(440);
	}
	else if (record[i] == 11) {
		key = 11;
		LPC_TIM0->MR0 = freq(466);
	}
	else if (record[i] == 12) {
		key = 12;
		LPC_TIM0->MR0 = freq(494);
	}
	else if (record[i] == 13) {
		key = 13;
		LPC_TIM0->MR0 = freq(523);
	}
	LPC_TIM0->TC = 0;
}


void EINT1_IRQHandler(void) {
	flag = true;
	LPC_SC->EXTINT = 2;
}

void TIMER0_IRQHandler(void) {
	if(key != 0) {
	LPC_DAC->DACR = ((int)(table[value] * 0.8) << 6) | (1 << 16);
	value++;
	if (value == 200) value = 0;
	}
	LPC_TIM0->IR = 1;
}

void TIMER1_IRQHandler(void) {
	if (counter <= 14950 && !flag) {
		record[counter] = key;
		counter++;
	}

	if (flag) {
		play(counter2);
		counter2++;
	}
	if (counter2 == counter) {
		counter2 = 0;
		flag = false;
		counter = 0;
		for(int i = 0; i < 14950; ++i) {
			record[i] = 0;
		}
	}
	LPC_TIM1->IR = 1;
}

int freq(int value) {
	return 250000 / value;
}





void EINT3_IRQHandler(void) {

	int tp_x = 0;
	int tp_y = 0;
	int sumX = 0;
	int sumY = 0;
	int x = 0;
	int y = 0;
	for (int i = 0; i < 100; ) {
		tp_x = touchpanelReadX();
		tp_y = touchpanelReadY();
		if (tp_x == 4095 && tp_y == 0) {
			key = 0;
			break;
		}
		if ((tp_x > 180 && tp_x < 4000) && (tp_y > 180 && tp_y < 4000))
		{
			sumX += tp_x;
			sumY += tp_y;
			i++;
		}
	}
	x = sumX / 100;
	y = sumY / 100;






	////////////////////////////
	if (y >= 400 && y <= 2100) {
		if (x >= 200 && x <= 640) {
			LPC_TIM0->MR0 = freq(262);
			key = 1;
		}
		else if (x >= 640 && x <= 1080) {
			LPC_TIM0->MR0 = freq(294);
			key = 3;
		}
		else if (x >= 1080 && x <= 1520) {
			LPC_TIM0->MR0 = freq(330);
			key = 5;
		}
		else if (x >= 1520 && x <= 1960) {
			LPC_TIM0->MR0 = freq(349);
			key = 6;
		}
		else if (x >= 1960 && x <= 2400) {
			LPC_TIM0->MR0 = freq(392);
			key = 8;
		}
		else if (x >= 2400 && x <= 2840) {
			LPC_TIM0->MR0 = freq(440);
			key = 10;
		}
		else if (x >= 2840 && x <= 3280) {
			LPC_TIM0->MR0 = freq(494);
			key = 12;
		}
		else if (x >= 3280 && x <= 3800) {
			LPC_TIM0->MR0 = freq(523);
			key = 13;
		}
	}
	else if (y >= 2100 && y <= 3800) {
		if (x >= 200 && x <= 530) {
			LPC_TIM0->MR0 = freq(262);
			key = 1;
		}
		else if (x >= 750 && x <= 970) {
			LPC_TIM0->MR0 = freq(294);
			key = 3;
		}
		else if (x >= 1190 && x <= 1520) {
			LPC_TIM0->MR0 = freq(330);
			key = 5;
		}
		else if (x >= 1520 && x <= 1850) {
			LPC_TIM0->MR0 = freq(349);
			key = 6;
		}
		else if (x >= 2070 && x <= 2290) {
			LPC_TIM0->MR0 = freq(392);
			key = 8;
		}
		else if (x >= 2510 && x <= 2730) {
			LPC_TIM0->MR0 = freq(440);
			key = 10;
		}
		else if (x >= 2950 && x <= 3280) {
			LPC_TIM0->MR0 = freq(494);
			key = 12;
		}
		else if (x >= 3280 && x <= 3800) {
			LPC_TIM0->MR0 = freq(523);
			key = 13;
		}
		///////czarne//////
		if (x >= 530 && x <= 750) {
			LPC_TIM0->MR0 = freq(277);
			key = 2;
		}
		else if (x >= 970 && x <= 1190) {
			LPC_TIM0->MR0 = freq(311);
			key = 4;
		}
		else if (x >= 1850 && x <= 2070) {
			LPC_TIM0->MR0 = freq(370);
			key = 7;
		}
		else if (x >= 2290 && x <= 2510) {
			LPC_TIM0->MR0 = freq(415);
			key = 9;
		}
		else if (x >= 2730 && x <= 2950) {
			LPC_TIM0->MR0 = freq(466);
			key = 11;
		}
	}
	////////////////////////////


	LPC_GPIOINT->IO0IntClr = (1 << 19);
	LPC_TIM0->TC = 0;
	
}





int main() {
	touchpanelInit();
	lcdConfiguration();
	NVIC_EnableIRQ(EINT1_IRQn);
	PIN_Configure(2,11,PIN_FUNC_1, PIN_PINMODE_PULLUP, PIN_PINMODE_NORMAL);


	PIN_Configure(0, 26, PIN_FUNC_2, PIN_PINMODE_TRISTATE, PIN_PINMODE_NORMAL);

	LPC_TIM0->PR = 0;
	LPC_TIM0->MCR = 3;
	LPC_TIM0->TCR = 1;
	NVIC_EnableIRQ(TIMER0_IRQn);







	LPC_GPIOINT->IO0IntEnR = (1 << 19);
	NVIC_EnableIRQ(EINT3_IRQn);


	init_ILI9325();
	lcdSetCursor(0, 0);
	lcdWriteReg(ADRX_RAM, 0);
	lcdWriteReg(ADRY_RAM, 0);
	lcdWriteReg(DATA_RAM, LCDGreen);

	int x = 0;
	int y = 0;
	int tmp = 240 * 320;




	for (int i = 0; i < tmp; i++) {
		if (!(i % 240) && i != 0) {
			y++;
			x = 0;
		}
		if (x >= 120 && x <= 238 && y >= 30 && y <= 50) {
			lcdWriteData(LCDBlack);
		}
		else if (x >= 120 && x <= 238 && y >= 70 && y <= 90) {
			lcdWriteData(LCDBlack);
		}
		else if (x >= 120 && x <= 238 && y >= 150 && y <= 170) {
			lcdWriteData(LCDBlack);
		}
		else if (x >= 120 && x <= 238 && y >= 190 && y <= 210) {
			lcdWriteData(LCDBlack);
		}
		else if (x >= 120 && x <= 238 && y >= 230 && y <= 250) {
			lcdWriteData(LCDBlack);
		}
		else if (x >= 0 && x <= 238 && y >= 1 && y <= 39) {
			lcdWriteData(LCDWhite);
		}
		else if (x >= 0 && x <= 238 && y >= 41 && y <= 79) {
			lcdWriteData(LCDWhite);
		}
		else if (x >= 0 && x <= 238 && y >= 81 && y <= 119) {
			lcdWriteData(LCDWhite);
		}
		else if (x >= 0 && x <= 238 && y >= 121 && y <= 159) {
			lcdWriteData(LCDWhite);
		}
		else if (x >= 0 && x <= 238 && y >= 161 && y <= 199) {
			lcdWriteData(LCDWhite);
		}
		else if (x >= 0 && x <= 238 && y >= 201 && y <= 239) {
			lcdWriteData(LCDWhite);
		}

		else if (x >= 0 && x <= 238 && y >= 241 && y <= 279) {
			lcdWriteData(LCDWhite);
		}
		else if (x >= 0 && x <= 238 && y >= 281 && y <= 319) {
			lcdWriteData(LCDWhite);
		}
		else {
			lcdWriteData(LCDBlack);
		}
		x++;

	}


	LPC_SC->PCONP |= (1 << 1) | (1 << 2);
	LPC_TIM1->PR = 0;
	LPC_TIM1->MCR = 3;
	LPC_TIM1->TCR = 1;
	LPC_TIM1->MR0 = 500000;
	NVIC_EnableIRQ(TIMER1_IRQn);

	while (1) {

	}



}
