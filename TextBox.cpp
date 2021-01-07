#include"TextBox.h"
#include <TFT_eSPI.h>
#include "Keyboard.h"

TextBox::TextBox()
{
	width=240;
	height=95;
	x=0;
	y=204;
	text="";
}
TextBox::TextBox(int _x, int _y, int w, int h)
{
	width=w;
	height=h;
	x=_x;
	y=_y;
	text="";
}
void TextBox::render(TFT_eSPI& tft)
{
	tft.fillRect(x,y,width,height,TFT_WHITE);
	tft.drawRect(x,y,width,height,TFT_BLACK);
	tft.setTextSize(1);
	tft.setTextColor(TFT_BLACK);
	tft.drawString(Keys.getCurrentWord(),x+2,y+2,2);
	
}
