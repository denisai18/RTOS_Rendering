#include"Button.h"
#include <TFT_eSPI.h>

Button::Button()
{
	width=240;
	height=95;
	x=0;
	y=204;
	text="";
}
Button::Button(int _x, int _y, int w, int h)
{
	width=w;
	height=h;
	x=_x;
	y=_y;
	text="";
}
Button::Button(int _x, int _y, int w, int h, String _text)
{
  width=w;
  height=h;
  x=_x;
  y=_y;
  text=_text;
  updated = true;
}
void Button::render(TFT_eSPI& tft)
{
  if(!updated)
  return;

  updated = false;
	tft.fillRect(x,y,width,height,TFT_WHITE);
	tft.drawRect(x,y,width,height,TFT_BLACK);
	tft.setTextSize(1);
	tft.setTextColor(TFT_BLACK);
	tft.drawCentreString(text,x+width/2,y+height/4,2);
	
}
void Button::setCallback(ButtonCallback cb)
{
  touchCb = cb;
}
