#ifndef BUTTON_H
#define BUTTON_H
#include <TFT_eSPI.h>

typedef void(*ButtonCallback)();

class Button
{
public:
Button();
Button(int _x, int _y, int w, int h);
Button(int _x, int _y, int w, int h, String _text);
void render(TFT_eSPI& tft);
void setText(String value)
{
	text=value;
}
bool isInInterval(int x, int minimum, int maximum)
{
  if(x > minimum && x < maximum)
    return true;
  return false;
}
void processInput(int x_, int y_)
{
  if(!isInInterval(x_,x,x+width) || !isInInterval(y_,y,y+height))
  {
    return;
  }
  Serial.println("Touched");
  if(touchCb)
    touchCb();
}

void setCallback(ButtonCallback cb);
private:
ButtonCallback touchCb;
String text;
bool updated;

int x, y, width, height;


};


#endif
