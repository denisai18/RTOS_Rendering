#include "Keyboard.h"
#include <TFT_eSPI.h>

Keyboard::Keyboard()
{
	width=240;
	height=220;
	x=0;
	y=320 - (240/10)*7;
	selCol=0;
	selRow=0;
	lastRow=0;
	lastCol=0;
	currentWord="";
	caps=false;
 updated=true;
}

void Keyboard::render(TFT_eSPI& tft)
{
  updated = false;
	uint16_t keyBg=0x0A0C;
	uint16_t boardBg=0x0397;
	int gap=240/10 - 3;
  keyWidth = gap;
	tft.fillRect(x,y,width-1,height-1,boardBg);
	tft.drawRect(x,y,width,height,TFT_BLACK);
	for(int i=0;i<7;++i)
	{
		for(int j=0;j<10;++j)
		{
			int localX = x+6+j*(gap+2);
			int localY = y+4+i*(gap+2);
			tft.fillRect(localX,localY,gap,gap,keyBg);
			tft.setTextColor(TFT_WHITE);
			char k=keys[i][j];
			if('a'<=k && k <='z')
			{
				if(caps)
				k-=32;
			}
			tft.drawCentreString(String(k),localX+gap/2,localY,2);
		}
	}
}
void Keyboard::update(TFT_eSPI& tft)
{
	uint16_t keyBg=0x0A0C;
	uint16_t selKeyBg=0x6DDD;
	uint16_t boardBg=0x0397;
	int gap=tft.fontHeight(2);
	int localX = x+2+lastCol*(gap+2);
	int localY = y+2 +lastRow*(gap+2);
	int newX = x+2+selCol*(gap+2);
	int newY = y+2+selRow*(gap+2);
	tft.fillRect(localX,localY,gap,gap,keyBg);
	tft.fillRect(newX,newY,gap,gap,selKeyBg);
	tft.setTextColor(TFT_WHITE);
	char k=keys[lastRow][lastCol];
	if('a'<=k && k <='z'){
		if(caps)
				k-=32;
	}
	tft.drawCentreString(String(k),localX+gap/2,localY,2);
	k=keys[selRow][selCol];
	if('a'<=k && k <='z')
		if(caps)
				k-=32;
	tft.drawCentreString(String(k),newX+gap/2,newY,2);
}
Keyboard Keys;
