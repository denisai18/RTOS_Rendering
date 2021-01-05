/*
 * This application uses an ESP32 as it's microcontroller with a touchscreen display connected to it
 * To make it easy to drive the display the TFT_eSPI graphics library is used.
 * Also the ESP32 SDK for Arduino IDE is based around FreeRTOS so no integration with a real time operating system will be necessary
 * TODO:
 * 1. Setup separated tasks for the game update (represented by the update method in the Game class and some other changes that will come trough the network) and render (represented by the drawBoard method defined below) methods
 * 
 * 2. Implement a state machine for the game logic with the following states:
 *    A. WAITING_FOR_CHOICE - user choses if it is a host or a client game
 *    B. WAITING_FOR_CONNECTION - host waits for a client
 *                              - client scans for nearby games and coonects to them
 *    C. GAME_IN_PROGRESS - both players are present and playing the game
 *    
 * 3. Implement simple socket messaging system for the devices interaction 
 *    Example: array of bytes with a length of 3 for sending a symbol over the network: [1, 2,2]
 *    The first byte represents the message type (a turn) and the next 2 represent the row and column that the player chose
 *   
 */



#include <SPI.h>
#include <TFT_eSPI.h>   
#include<string>
#include<memory>
#include "Game.h"
Game g;
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

void setup() {
  Serial.begin(115200);

  tft.init();

  // Set the rotation before we calibrate
  tft.setRotation(2);

  // Calibrate the touch screen and retrieve the scaling factors
  //touch_calibrate();
  uint16_t calData[5] = { 370, 3412, 404, 3452, 2 };
  tft.setTouch(calData);

//uint16_t calData[5] = { 425, 3439, 355, 3459, 7 };
  //tft.setTouch(calData);


  tft.fillScreen(TFT_BLACK);
  drawBoard();
}

bool pressing = false;
void loop(void) {
  uint16_t x = 0, y = 0;
  boolean pressed = tft.getTouch(&x, &y);

  // Draw a white spot at the detected coordinates
  if (pressed) {
    if(!pressing){
      if(g.myTurn()){
      int row;
      int col;
      getRowColFromPosition(x,y, row, col);
      if(g.setToCurrent(row, col))
      {
        //send value change over the network here
      }
      }
    pressing = true;

    }
    

  }
  else
  {
    pressing = false;
  }
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
  for(int i=0;i<3;++i){
  for(int j=0;j<3;++j){
    int mx = j*w;
    int my = i *w;
    int fx = mx + w;
    int fy = my + w;
    if(isInInterval(x,mx,fx) && isInInterval(y,my,fy))
    {
      row = i;
      col = j;
      return;
    }
  }
  }
}


void drawBoard()
{
  int w = 240/3;
  tft.fillRect(0,0,240,240,TFT_BLACK);
  for(int i=0;i<3;++i){
  for(int j=0;j<3;++j){
    int x = j*w;
    int y = i *w;
    int mx = j*w;
    int my = i *w;
    int fx = mx + w;
    int fy = my + w;
    int offset = 10;
    tft.drawRect(x,y,w,w,TFT_WHITE);
    if(g.get(i,j) != 0){
    if(g.get(i,j) == 1)
      {
        int x1 = mx + offset;
        int y1 = my + offset;
        int x2 = fx - offset;
        int y2 = fy - offset;
        tft.drawLine(x1,y1,x2,y2, TFT_WHITE);
        tft.drawLine(x1,y2,x2,y1, TFT_WHITE);
      }
      else
      {
        int xc = mx + w/2;
        int yc = my + w/2;
        tft.drawCircle(xc,yc, w/2 - offset, TFT_WHITE);
      }
    }
  }
 }
}

// Code to run a screen calibration, not needed when calibration values set in setup() provided by the library
void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // Calibrate
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch corners as indicated");

  tft.setTextFont(1);
  tft.println();

  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

  Serial.println(); Serial.println();
  Serial.println("// Use this calibration code in setup():");
  Serial.print("  uint16_t calData[5] = ");
  Serial.print("{ ");

  for (uint8_t i = 0; i < 5; i++)
  {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  Serial.print("  tft.setTouch(calData);");
  Serial.println(); Serial.println();

  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");

  delay(4000);
}
