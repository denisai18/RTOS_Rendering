/*
 * The Game class defines the TicTacToe logic and keeps track of the game progress
 */
#ifndef GAME_H
#define GAME_H

class Game
{
  public:
  Game()
  {
    initBoard();   
    local = false;
  }
  bool set(int row, int col, int val)
  {
   
    if(col >= 3 || row >= 3)
    return false;
    board[row][col] = val;
     updated = true;
    return true;
  }
  bool setToCurrent(int row, int col)
  {
    
     if(col >= 3 || row >= 3)
    return false;
    board[row][col] = currentTurn;
    updated = true;
    return true;
  }
    void startLocal()
  {
    local = true;
    newGame(false);
  }
  void setCurrentTurn(char val)
  {
    currentTurn = val;
    turnChanged = true;
  }
void Reset()
{
initBoard();
updated=true;
hostToken = (char)1;
clientToken = (char)2;
currentTurn = (char)1;
win = -1;
winPos = -1;
turnChanged = true;
if (hosting)
myToken = hostToken;
else
myToken = clientToken;
}
void newGame(bool host)
  {
    updated = true;
    initBoard();  
    hosting = host;
   Reset();
  }
  String getCurrentTurn()
  {
    if(currentTurn == 1)
    return "X";
    else
    return "O";
  }
  char getTurn()
  {
    return currentTurn;
  }
  void update()
  {
    turnChanged = false;
    if(hosting || local)
    {
       Serial.println("Next turn...");
      winPos = checkWin();
      Serial.println(win);
      if(win < 0){
        nextTurn();
       
      }
      
  }
  }
  bool host()
  {
    return hosting;
  }
  bool turnChange()
  {
    bool old = turnChanged;
    turnChanged = false;
    return old;
  }
  int winner()
  {
    return win;
  }
  int winPosition()
  {
    return winPos;
  }
  bool gameOver()
  {
    return win >= 0;
  }
  bool myTurn()
  {
    return currentTurn == myToken;
  }
  bool didIWin()
  {
    return myToken == win;
  }
  void setWin(char val)
  {
    updated = true;
    win = val;
  }
  void setWinPos(char val)
  {
    winPos = val;
    Serial.println(winPos);
  }
  char get(int row, int col)
  {
    return board[row][col];
  }

  void rendered()
  {
    updated = false;
  }
  bool needRender()
  {
    return updated;
  }
private:
void initBoard()
{
  for(int i=0; i<3; ++i){
    for(int j=0; j<3; ++j)
    board[i][j] = 0;
  }
}
void nextTurn()
{
  if(currentTurn == 1){
        currentTurn = 2;
      }
      else 
      {
        currentTurn =1;
      }
    turnChanged = true;
    }
    
 bool checkLine(int i, int symbol)
{

    for (int col = 0; col < 3; ++col)
    {
        if (board[i][col] != symbol)
            return false;
    }
    return true;
}
bool checkCol(int i, int symbol)
{

    for (int line = 0; line < 3; ++line)
    {
        if (board[line][i] != symbol)
            return false;
    }
    return true;
}

int checkDiag(int symbol)
{
    bool princ = true;
    bool sec = true;
    for (int i = 0; i < 3; ++i)
    {

        for (int j = 0; j < 3; ++j)
        {
            if (i == j)
                if (board[i][j] != symbol)
                    princ = false;
            if (i == 2 - j)
                if (board[i][j] != symbol)
                    sec = false;
        }
    }

    if (princ)
        return 1;
    if (sec)
        return 2;
    return 0;
}
int checkTie()
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (board[i][j] == 0)
                return 0;
        }
    }
    return 99;
}
int checkWin()
{
    int tie = checkTie();
    if (tie > 0)
    {
        win = 0;
        return tie;

    }
    for (int i = 0; i < 3; ++i)
    {
        if (checkLine(i, hostToken))
        {
            win = hostToken;
            return i;
        }
        if (checkLine(i, clientToken))
        {
            win = clientToken;
            return i;
        }

        if (checkCol(i, hostToken))
        {
            win = hostToken;
            return 3 + i;
        }
        if (checkCol(i, clientToken))
        {
            win = clientToken;
            return 3 + i;
        }
        int diag = 0;
        diag = checkDiag(hostToken);
        if (diag != 0)
        {
            win = hostToken;
            return 6 + diag;
        }
        diag = checkDiag(clientToken);
        if (diag != 0)
        {
            win = clientToken;
            return 6 + diag;
        }

    }
    return -1;
}

char board[3][3];
bool hosting;

char hostToken;
char clientToken;
char currentTurn;
int win;
int winPos;
bool turnChanged;
char myToken;
bool local;
bool updated;

};

#endif
