#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <SPI.h>
#include <TFT_eSPI.h>


class Keyboard
{
public:
Keyboard();
void render(TFT_eSPI& tft);
void update(TFT_eSPI& tft);
void setSelKey(int i, int j)
{
	lastRow=selRow;
	lastCol=selCol;
	selRow=i;
	selCol=j;
	if(selCol < 0)
		selCol=0;
	if(selRow < 0)
		selRow=0;
	
	if(selCol > 12)
		selCol=12;
	if(selRow > 4)
		selRow = 4;
}
int getSelRow()
{
	return selRow;
}
int getSelCol()
{
	return selCol;
}
String getCurrentWord()
{
	return currentWord;
}
void addToCurrentWord()
{
	if(selRow== 4&& selCol==4)
	{
		caps=!caps;
	}
	else{
		char c=keys[selRow][selCol];
	if(caps)
		if('a'<=c && c <='z')
		currentWord+=(char)(c-32);
		else
			currentWord+=(char)(c);
	else
		currentWord+=keys[selRow][selCol];
	}
}
bool requestRender()
{
  return updated;
}
void backspace()
{
	if(currentWord.length() != 0)
	currentWord.remove(currentWord.length()-1);
}
void processInput(int x_, int y_)
{
  if(y_<y)
    return;
  y_-=y;
  int row;
  int col;

  getRowColFromPosition(x_,y_, row, col);
  if(col < 0 || col > 10 || row < 0 || row > 7)
    return;
  Serial.print(row); Serial.print(" "); Serial.println(col);
  if(row == 6 && col == 9)
  {
    Serial.println("Removing...");
    backspace();
    return;
  }
  
  if(row== 6 && col==0)
  {
    caps=!caps;
    updated = true;
    return;
  }
  else{
    char c=keys[row][col];
  if(caps)
    if('a'<=c && c <='z')
    currentWord+=(char)(c-32);
    else
      currentWord+=(char)(c);
  else
    currentWord+=keys[row][col];
  }

  Serial.println(currentWord);
  
}
bool isInInterval(int x, int minimum, int maximum)
{
  if(x > minimum && x < maximum)
    return true;
  return false;
}
void getRowColFromPosition(int x, int y, int& row, int& col)
{
  int w = 240/3;
  for(int i=0;i<7;++i){
  for(int j=0;j<10;++j){
    int mx = 6+j*(keyWidth+2);
    int my = 4+i*(keyWidth+2);
    int fx = mx + keyWidth;
    int fy = my + keyWidth;
    if(isInInterval(x,mx,fx) && isInInterval(y,my,fy))
    {
      row = i;
      col = j;
      return;
    }
  }
  }
}

private:
TFT_eSPI tft;
String currentWord;
int x, y, width, height;
int selRow;
int selCol;
int lastCol;
int lastRow;
bool inited;
bool caps;
int keyWidth;
bool updated;
char keys[7][10]=
{{'1','2','3','4','5','6','7','8','9','0'},
{'q','w','e','r','t','y','u','i','o','p'}, 
{'a','s','d','f','g','h','l',';',':','‘'},
{'z','x','c','v','b','n','m',',','.','-'},
{'!','@','#','$','%','^','&','*','(',')'},
{'/','?','<','>','_','=','[',']','\\','+'},
{'s',' ',' ',' ',' ',' ',' ',' ',' ','<'}}; 

};
extern Keyboard Keys;
#endif
