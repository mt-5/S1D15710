#include <avr/io.h>
#include "s1d15710.h"
#include <util/delay.h>

unsigned char LCD_ReadData(void){
	unsigned char dane;
	SET_RST;
	SET_WR;
	CLR_CS1;
	SET_CS1;
	LCD_DATA_SET(0xAA);
	LCD_DATA_READ();
	CLR_RD;
	CLR_CS1;
	SET_CS1;
	CLR_CS1;
	__asm__ volatile ("nop");
	__asm__ volatile ("nop");
	LCD_DATA_GET(dane);
	SET_CS1;
	SET_RD;
	LCD_DATA_WRITE();
	return dane;
}

void LCD_WriteCommand(unsigned char d){
	LCD_DATA_SET(d);
	CLR_A0;
	CLR_CS1;
	CLR_WR;
	SET_RD;
	SET_WR;
	SET_CS1;
	SET_A0;
}

void LCD_WriteData(unsigned char ch){
	LCD_DATA_WRITE();
	CLR_RD;
	CLR_A0;
	CLR_CS1;
	CLR_WR;
	SET_RD;
	SET_A0;
	LCD_DATA_SET(ch);
	SET_WR;
	SET_CS1;
	SET_A0;
}

void LCD_Init(){
	CLR_RST;
	SET_RST;
	LCD_DATA_WRITE();
	LCD_WriteCommand(DISP_OSCL);
	LCD_WriteCommand(DISP_BIAS);
	LCD_WriteCommand(DISP_TYPE);
	LCD_WriteCommand(DISP_COMMON);
	LCD_WriteCommand(DISP_START_LINE);
	LCD_WriteCommand(DISP_PWR_SET);
	LCD_WriteCommand(DISP_V5ADJ);
	LCD_WriteCommand(DISP_ELECTRCTRL);
	LCD_WriteCommand(0xA0);
	LCD_WriteCommand(DISP_ONOFF);
	LCD_WriteCommand(END);
}

void LCD_Clear(uint8_t mode){
	uint8_t a = 0;
	uint8_t i;
	LCD_DATA_WRITE();
	while (a<=MaxPage){
		LCD_WriteCommand(DISP_PAGE_ADDR+a);
		LCD_WriteCommand(DISP_COL_ADDR_H);
		LCD_WriteCommand(DISP_COL_ADDR_L);
		for (i=0; i<=MaxColumn; ++i)
			LCD_WriteData(0xFF*mode);
		a++;
	}

}

void LCD_SetPixel(uint8_t x,uint8_t y,uint8_t mode){
	if(x >= 0 && x < 219 && y >= 0 && y < 60) {
		uint8_t y_h, y_l,d;
		uint8_t tmp;
		uint8_t page;
		x+=1;
		y_h=(x>>4);
		y_l=x&0x0f;
		page=y/8;
		tmp=y-page*8;
		d=(1<<tmp);
		LCD_DATA_WRITE();
		LCD_WriteCommand(DISP_PAGE_ADDR | page);		
		LCD_WriteCommand(DISP_COL_ADDR_H | y_h);		
		LCD_WriteCommand(DISP_COL_ADDR_L | y_l);
		tmp = LCD_ReadData();
		if (mode) 
			d^= tmp;
		else 
			d|= tmp;
		LCD_WriteCommand(DISP_PAGE_ADDR | page);	
		LCD_WriteCommand(DISP_COL_ADDR_H | y_h);	
		LCD_WriteCommand(DISP_COL_ADDR_L | y_l);
		LCD_WriteData(d);
	}
}

void LCD_WriteChar(char str, uint8_t x, uint8_t y,uint8_t mode){
    uint8_t c;
    for (uint8_t i=0; i<5;i++){
		c = pgm_read_byte(&font[((str-32)*5)+i]);
		for (uint8_t j=0;j<8;j++){
			if(mode){ 
				if (c&(1<<j)) 
					LCD_SetPixel(x+i,y+j,mode);
			}
			else 
				if (!(c&(1<<j))) 
					LCD_SetPixel(x+i,y+j,mode);
		}
    }
}

void LCD_WriteText(char *str, uint8_t x, uint8_t y,uint8_t mode){
	uint8_t step=0;
	while(*str){
		LCD_WriteChar(*str++,x+step,y,mode);
		step+=6; 
	}
}

void LCD_DrawRect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2, uint8_t fill, uint8_t mode){
	for (uint8_t i=x1;i<=x2;i++)
		for (uint8_t j=y1;j<=y2;j++)
			if(fill || (i == x1 || i == x2 || j == y1 || j == y2))
				LCD_SetPixel(i,j,mode);
}

void LCD_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2, uint8_t mode){
  signed short deltax, deltay, g, h, c;

  deltax = x2-x1;
  if ( deltax > 0 ) g = +1; else g = -1;
  deltax = abs(deltax);
  deltay = y2-y1;
  if ( deltay > 0 ) h = +1; else h = -1;
  deltay = abs(deltay);
  if ( deltax > deltay ) {
    c = -deltax;
    while ( x1 != x2 ) {
      LCD_SetPixel ( x1, y1, mode );
      c += 2*deltay;
      if ( c > 0 ) { y1 += h; c -= 2*deltax; }
      x1 += g;
    }
  }
  else {
    c = -deltay;
    while ( y1 != y2 ) {
      LCD_SetPixel ( x1, y1, mode );
      c += 2*deltax;
      if ( c > 0 ) { x1 += g; c -= 2*deltay; }
      y1 += h;
    }
  }
}