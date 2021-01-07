#ifndef TEXTBOX_H
#define TEXTBOX_H
#include <TFT_eSPI.h>
#include<Arduino.h>


class TextBox
{
public:
TextBox();
TextBox(int _x, int _y, int w, int h);
void render(TFT_eSPI& tft);
void setText(String value)
{
	text=value;
}


private:
String text;


int x, y, width, height;


};
#endif
