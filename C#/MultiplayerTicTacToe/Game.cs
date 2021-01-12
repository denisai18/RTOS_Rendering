using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MultiplayerTicTacToe
{
    class Game
    {
        public Game()
        {
            board = new char[3, 3];
            initBoard();
            local = false;
            
        }
        public bool set(int row, int col, int val)
        {

            if (col >= 3 || row >= 3)
                return false;
            board[row,col] = (char)val;
            updated = true;
            return true;
        }
        public bool setToCurrent(int row, int col)
        {

            if (col >= 3 || row >= 3)
                return false;
            board[row, col] = currentTurn;
            updated = true;
            return true;
        }
        public void startLocal()
        {
            local = true;
            newGame(false);
        }
        public void newGame(bool host)
        {
            updated = true;
            initBoard();
            hosting = host;
            Reset();
        }
        public void Reset()
        {
            initBoard();
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
        public string getCurrentTurn()
        {
            if (currentTurn == 1)
                return "X";
            else
                return "O";
        }
        public void update()
        {
            turnChanged = false;
            if (hosting || local)
            {
                winPos = checkWin();
                if (win < 0)
                {
                    nextTurn();

                }

            }
        }
        public bool host()
        {
            return hosting;
        }
        public bool turnChange()
        {
            bool old = turnChanged;
            turnChanged = false;
            return old;
        }
        public int winner()
        {
            return win;
        }
        public int winPosition()
        {
            return winPos;
        }
        public bool gameOver()
        {
            return win >= 0;
        }
        public bool myTurn()
        {
            return currentTurn == myToken;
        }
        public bool didIWin()
        {
            return myToken == win;
        }
        public void setWin(char val)
        {
            win = val;
        }
        public void setWinPos(char val)
        {
            winPos = val;
        }
        public char get(int row, int col)
        {
            return board[row, col];
        }

        void rendered()
        {
            updated = false;
        }
        bool needRender()
        {
            return updated;
        }
        void initBoard()
        {
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                    board[i, j] = (char)0;
            }
        }
        void nextTurn()
        {
            if (currentTurn == 1)
            {
                currentTurn = (char)2;
            }
            else
            {
                currentTurn = (char)1;
            }
            turnChanged = true;
        }

        bool checkLine(int i, int symbol)
        {

            for (int col = 0; col < 3; ++col)
            {
                if (board[i, col] != symbol)
                    return false;
            }
            return true;
        }
        bool checkCol(int i, int symbol)
        {

            for (int line = 0; line < 3; ++line)
            {
                if (board[line, i] != symbol)
                    return false;
            }
            return true;
        }
        public void setCurrentTurn(char val)
        {
            currentTurn = val;
            turnChanged = true;
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
                        if (board[i, j] != symbol)
                            princ = false;
                    if (i == 2 - j)
                        if (board[i, j] != symbol)
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
                    if (board[i, j] == 0)
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

        char[,] board;
        bool hosting;

        public char hostToken;
        public char clientToken;
        public char currentTurn;
        int win;
        int winPos;
        bool turnChanged;
        char myToken;
        bool local;
        bool updated;

    }
}
