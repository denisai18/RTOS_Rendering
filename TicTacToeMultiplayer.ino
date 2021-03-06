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



#include <TFT_eSPI.h>   
#include<string>
#include<memory>
#include <WiFi.h>
#include <WiFiMulti.h>


#include "Game.h"
Game g;
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
#include "Keyboard.h"
#include "TextBox.h"
#include "Button.h"

enum SystemState{
  TYPE_SELECT,
  LOCAL_GAME,
  JOIN_GAME
};

union integer {
  int value;
  char chrVal[4];
};

WiFiMulti WiFiMulti;
WiFiClient client;
bool connectedClient = false;
//Type Select UI

Button local(20,10,200,50,"Local game");
Button host(20, 70,200,50,"Host game");
Button join(20,130,200,50,"Join game");
Button reset(140,320-40,90,30,"Reset");

TextBox tb(10,10,200,20);
xSemaphoreHandle xMutex=NULL;

SystemState state = TYPE_SELECT;

void localTouch()
{
  Serial.println("Local game...");
  state = LOCAL_GAME;
  g.startLocal();
  tft.fillScreen(TFT_BLACK);
}
void resetTouch()
{
  g.startLocal();
}

void joinTouch()
{
  Serial.println("Join game...");
  state = JOIN_GAME;
  tft.fillScreen(TFT_BLACK);
}

void setup() {
  Serial.begin(115200);

  WiFiMulti.addAP("CBN_SSID_2.4G", "ddm33Qxxdmfe");

    Serial.println();
    Serial.println();
    Serial.print("Waiting for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

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
  local.setCallback(localTouch);
  reset.setCallback(resetTouch);
  join.setCallback(joinTouch);

//RTOS task creation
 xMutex=xSemaphoreCreateMutex();  //xMutex will be storing NULL when Mutex not created
  if(xMutex!=NULL) {
  xTaskCreate(
renderUI, /* Task function. */
"renderUI", /* name of task. */
10000, /* Stack size of task */
NULL, /* parameter of the task */
1, /* priority of the task */
NULL); /* Task handle to keep track of created task */

  xTaskCreate(
handleInput, /* Task function. */
"handleInput", /* name of task. */
10000, /* Stack size of task */
NULL, /* parameter of the task */
1, /* priority of the task */
NULL); /* Task handle to keep track of created task */

  xTaskCreate(
handleClientSocket, /* Task function. */
"handleClientSocket", /* name of task. */
10000, /* Stack size of task */
NULL, /* parameter of the task */
1, /* priority of the task */
NULL); /* Task handle to keep track of created task */
}
}

bool pressing = false;
void loop(void) {
}

void SendClientTurnMessage()
{
  if(!connectedClient)
  {
    return;
  }
  std::vector<char> message;
  integer messageSize;
  messageSize.value = 2;
  message.push_back(messageSize.chrVal[0]);
  message.push_back(messageSize.chrVal[1]);
  message.push_back(messageSize.chrVal[2]);
  message.push_back(messageSize.chrVal[3]);
  message.push_back(2);
  message.push_back(g.getTurn());
  client.write(message.data(), message.size());
  
}

void SendClientMoveMessage(char x, char y)
{
  if(!connectedClient)
  {
    return;
  }
  std::vector<char> message;
  integer messageSize;
  messageSize.value = 3;
  message.push_back(messageSize.chrVal[0]);
  message.push_back(messageSize.chrVal[1]);
  message.push_back(messageSize.chrVal[2]);
  message.push_back(messageSize.chrVal[3]);
  message.push_back(1);
  message.push_back(x);
  message.push_back(y);
  client.write(message.data(), message.size());
}

void SendClientWinMessage()
{
  char winByte = (char)g.winner();
  char codeByte = (char)g.winPosition();
  std::vector<char> message;
  integer messageSize;
  messageSize.value = 3;
  message.push_back(messageSize.chrVal[0]);
  message.push_back(messageSize.chrVal[1]);
  message.push_back(messageSize.chrVal[2]);
  message.push_back(messageSize.chrVal[3]);
  message.push_back(3);
  message.push_back(winByte);
  message.push_back(codeByte);
 client.write(message.data(), message.size());
}

void handleMessage(std::vector<char> message)
{
  char type = message[0];

switch (type)
{
case 0:
{
 }
break;
case 1:
{
byte xPos = message[1];
byte yPos = message[2];
g.setToCurrent(xPos, yPos);
if(g.host())
{
g.update();
SendClientTurnMessage();
}
}
break;
case 2:
{
g.setCurrentTurn((char)message[1]);
}
break;
case 3:
{
g.setWin((char)message[1]);
g.setWinPos((char)message[2]);
g.update();
}                    
break;
case 4:
{
  g.Reset();
}
break;
case 5:
{
  //Score functionality not yet present
}
break;
}
}


std::vector<char> message;
std::vector<char> messageSizev;
integer messageSize;
int currentRead = 0;
int currentByte = 0;
void handleClientSocket( void * parameter)
{

/* loop forever */
for(;;)
{
  if(state == JOIN_GAME)
  {
  xSemaphoreTake(xMutex, portMAX_DELAY);
    const uint16_t port = 11000;
    const char * host = "192.168.0.31";
    if(!connectedClient){
      if (!client.connect(host, port)) {
        Serial.println("Connection failed.");
        Serial.println("Waiting 5 seconds before retrying...");
        vTaskDelay(5000);
        
    }
    else
    {
      connectedClient=true;
      g.newGame(false);
    }
    }
    else
    {
      while(client.available() > 0)
      {
        char chr = client.read();
        if(messageSize.value == 0)
        {
          messageSizev.push_back(chr);
          currentByte++;
          if(currentByte == 4)
          {
            messageSize.chrVal[0] = messageSizev[0];
            messageSize.chrVal[1] = messageSizev[1];
            messageSize.chrVal[2] = messageSizev[2];
            messageSize.chrVal[3] = messageSizev[3];
            currentByte = 0;
            messageSizev.clear();
          }
        }
        else{
          message.push_back(chr);
          if(message.size() == messageSize.value)
          {
            handleMessage(message);
            message.clear();
            messageSize.value =  0;
          }
        }
      }
    }
  xSemaphoreGive(xMutex);
}
  vTaskDelay(10);
}
/* delete a task when finish,
this will never happen because this is infinity loop */
vTaskDelete( NULL );
}


void handleInput( void * parameter )
{
/* loop forever */
for(;;)
{
  xSemaphoreTake(xMutex, portMAX_DELAY);
  uint16_t x = 0, y = 0;
  boolean pressed = tft.getTouch(&x, &y);

  // Draw a white spot at the detected coordinates
  if (pressed) {
    if(!pressing){

switch(state)
{
  case TYPE_SELECT:
  {
    local.processInput(x,y);
    host.processInput(x,y);
    join.processInput(x,y);
  }
  break;
  case JOIN_GAME:
  {
    int row;
      int col;
      if(!g.gameOver())
      {
      if(x <=240 && y <=240){
      getRowColFromPosition(x,y, row, col);
      if(g.setToCurrent(row, col))
      {
        SendClientMoveMessage(row,col);
      }
      }
      }
  }
  break;
  case LOCAL_GAME:
  {
      int row;
      int col;
      if(!g.gameOver())
      {
      if(x <=240 && y <=240){
      getRowColFromPosition(x,y, row, col);
      if(g.setToCurrent(row, col))
      {
        g.update();
      }
      }
      }
      reset.processInput(x,y);
  }break;
}
    pressing = true;

    }
    

  }
  else
  {
    pressing = false;
  }
  xSemaphoreGive(xMutex);
  vTaskDelay(5);
}
/* delete a task when finish,
this will never happen because this is infinity loop */
vTaskDelete( NULL );
}

void renderUI( void * parameter )
{
/* loop forever */
for(;;)
{
xSemaphoreTake(xMutex, portMAX_DELAY);
/*
tb.setText(Keys.getCurrentWord());
tb.render(tft);
if(Keys.requestRender()){
  Keys.render(tft);
}
*/

switch(state)
{
  case TYPE_SELECT:
  {
    local.render(tft);
    host.render(tft);
    join.render(tft);
  }
  break;
  case JOIN_GAME:
  {
        drawBoard();
        if(g.turnChange())
        {
      tft.fillRect(0,245, 230,30,TFT_BLACK);
      tft.setTextColor(TFT_WHITE);

      tft.drawString("Current turn:" + g.getCurrentTurn(),10,245);
        }
  }
  break;
  case LOCAL_GAME:
  {
    drawBoard();
        if(g.turnChange())
        {
      tft.fillRect(0,245, 230,30,TFT_BLACK);
      tft.setTextColor(TFT_WHITE);

      tft.drawString("Current turn:" + g.getCurrentTurn(),10,245);
        }
        reset.render(tft);
  }
  break;
}

vTaskDelay(16);
xSemaphoreGive(xMutex);

}
/* delete a task when finish,
this will never happen because this is infinity loop */
vTaskDelete( NULL );
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
  if(!g.needRender())
  return;
int winCode = g.winPosition();
int winCol = -1;
int winRow = -1;
int winDiag = -1;
if (winCode >= 0)
{
if (winCode <= 2)
{
winRow = winCode;
}
else
{
if (winCode <= 5)
{
winCol = winCode - 3;
}
else
{
winDiag = winCode - 6;
}
}
}
  
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
 if(winRow >= 0)
 {
  int y = w*winRow + w/2;
  tft.drawLine(0,y,240,y,TFT_RED);
 }
 if(winCol >=0)
 {
  int x = w*winCol + w/2;
  tft.drawLine(x,0,x,240,TFT_RED);
 }
 if(winDiag == 1)
 {
  tft.drawLine(0,0,240,240,TFT_RED);
 }
 if(winDiag == 2)
 {
  tft.drawLine(240,0,0,240,TFT_RED);
 }
 g.rendered();
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
