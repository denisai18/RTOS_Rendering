using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;
using System.Runtime.Remoting.Messaging;

namespace MultiplayerTicTacToe
{
    public partial class GameControl : UserControl
    {
        Game game;
        Socket socket;
        bool havePlayer = false;
        bool hosting = false;
        List<SocketCommunicationClient> clients = new List<SocketCommunicationClient>();
        SocketCommunicationClient client = null;
        SocketCommunicationListener server = new SocketCommunicationListener();
        Thread serverThread;
        int[,] board = new int[3, 3];
        Dictionary<int, int> score = new Dictionary<int, int>();
        public GameControl()
        {
            InitializeComponent();
            game = new Game();
        }

        private void GameControl_Load(object sender, EventArgs e)
        {
            Reset();
            score.Add(1, 0);
            score.Add(2, 0);
        }

        void Reset()
        {
            game.Reset();
        }
        private void SendClientTurnMessage()
        {
            if (client != null)
            {
                List<byte> message = new List<byte>();
                int messageSize = 2;
                byte sByte = (byte)game.currentTurn;
                message.AddRange(BitConverter.GetBytes(messageSize));
                message.Add(2);
                message.Add(sByte);
                client.SendData(message.ToArray());
            }
        }
        private void SendClientResetMessage()
        {
            if (client != null)
            {
                List<byte> message = new List<byte>();
                int messageSize = 1;
                message.AddRange(BitConverter.GetBytes(messageSize));
                message.Add(4);
                client.SendData(message.ToArray());
            }
        }

        private void SendClientMoveMessage(int x, int y)
        {
            if (client != null)
            {
                List<byte> message = new List<byte>();
                int messageSize = 3;
                byte xByte = (byte)x;
                byte yByte = (byte)y;
                message.AddRange(BitConverter.GetBytes(messageSize));
                message.Add(1);
                message.Add(xByte);
                message.Add(yByte);
                client.SendData(message.ToArray());
            }
        }

        private void SendClientScoreMessage()
        {
            if (client != null)
            {
                List<byte> message = new List<byte>();
                int messageSize = 1 + 2*sizeof(int);
                message.AddRange(BitConverter.GetBytes(messageSize));
                message.Add(5);
                int xScore = score[1];
                int yScore = score[2];
                message.AddRange(BitConverter.GetBytes(xScore));
                message.AddRange(BitConverter.GetBytes(yScore));
                client.SendData(message.ToArray());
            }
        }

        private void SendClientWinMessage()
        {
            if (client != null)
            {
                List<byte> message = new List<byte>();
                int messageSize = 3;
                byte winByte = (byte)game.winner();
                byte codeByte = (byte)game.winPosition();
                message.AddRange(BitConverter.GetBytes(messageSize));
                message.Add(3);
                message.Add(winByte);
                message.Add(codeByte);
                client.SendData(message.ToArray());
            }
        }
        private void Server_ClientAvailable(object sender, CommunicationListenerEventArgs e)
        {
            if (!havePlayer)
            {
                client = e.Client;
                client.DataAvailable += Client_DataAvailable;
                havePlayer = true;
                SendClientTurnMessage();
                RenderFromThread();
            }


        }

        private void Client_DataAvailable(object sender, MessageEventArgs e)
        {
            Console.WriteLine("Data");
            byte type = e.Payload[0];
            switch (type)
            {
                case 0:
                    {
                    }
                    break;
                case 1:
                    {
                        byte xPos = e.Payload[1];
                        byte yPos = e.Payload[2];
                        game.setToCurrent(xPos, yPos);
                        if(game.host())
                        {
                            game.update();
                            SendClientTurnMessage();
                        }
                    }
                    break;
                case 2:
                    {
                        game.setCurrentTurn((char)e.Payload[1]);
                    }
                    break;
                case 3:
                    {
                        game.setWin((char)e.Payload[1]);
                        game.setWinPos((char)e.Payload[2]);
                        game.update();
                        RenderFromThread();

                    }
                    break;
                case 4:
                    {
                        Reset();
                    }
                    break;
                case 5:
                    {
                        int xScore = BitConverter.ToInt32(e.Payload,1);
                        int oScore = BitConverter.ToInt32(e.Payload, 1+sizeof(int));
                        score[1] = xScore;
                        score[2] = oScore;
                    }
                    break;
            }
            if (hosting)
            {
                SendWin();
            }
            RenderFromThread();

        }
        void SendWin()
        {
            int win = game.winner();
            if (win >=0)
            {
                SendClientWinMessage();
                if (win > 0)
                    score[win]++;
                SendClientScoreMessage();
            }
        }
        private void HostAGame()
        {
            if (game.host())
                return;
            
            server.ClientAvailable += Server_ClientAvailable;
            hosting = true;
            Reset();
            game.newGame(true);
            serverThread = new Thread(new ThreadStart(() =>
           {
               server.Start();
           }));
            serverThread.Start();
            joinBtn.Enabled = false;

        }

        private void hostBtn_Click(object sender, EventArgs e)
        {
            HostAGame();
            hostingLbl.Visible = true;
            joinBtn.Enabled = false;
            hostBtn.Enabled = false;
        }

        private void RenderFromThread()
        {
            this.Invoke(new Action(() =>
            {
                RenderBoard();
            }));
        }
        private void RenderBoard()
        {
            scoreLbl.Text = $"X:{score[1]}{Environment.NewLine}O:{score[2]}";
            string turn = "";
            turn = game.getCurrentTurn();
            int you = game.host() ? game.hostToken : game.clientToken;
            string youTxt = "";
            if (you > 0)
            {
                youTxt = you == 1 ? "X" : "O";
            }
            if (turn != string.Empty)
                turnLbl.Text = $"Current turn: {turn}";
            if (youTxt != string.Empty)
                yourLbl.Text = $"You are: {youTxt}";
            if (game.winner() == -1)
            {
                winerLbl.Text = "";
            }
            else
            {
                if (game.host())
                {
                    resetBtn.Visible = true;
                }

                if (game.didIWin())
                {
                    winerLbl.ForeColor = Color.LimeGreen;
                    winerLbl.Text = "WINNER";
                }
                else
                {
                    winerLbl.ForeColor = Color.Red;
                    winerLbl.Text = "LOSER";
                }
                if (game.winner() == 0)
                {
                    winerLbl.ForeColor = Color.Blue;
                    winerLbl.Text = "TIE";
                }
                winerLbl.Location = new Point(this.Width / 2 - winerLbl.Width / 2, winerLbl.Location.Y);
            }
            if (client == null)
                return;
            int width = 100;
            int height = 100;
            int offset = 2;
            boardPnl.Controls.Clear();
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    Button btn = new Button();
                    btn.FlatStyle = FlatStyle.Flat;
                    btn.ForeColor = Color.Black;
                    btn.Font = new Font(btn.Font.FontFamily, 24);
                    btn.Size = new Size(width, height);
                    int currentX = offset + j * (width + offset);
                    int currentY = offset + i * (height + offset);
                    btn.Location = new Point(currentX, currentY);
                    int winCol = -1;
                    int winRow = -1;
                    int winDiag = -1;
                    int winCode = game.winPosition();
                    if (winCode >= 0)
                    {
                        Console.WriteLine(winCode);
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
                    if (winRow >= 0)
                    {
                        if (i == winRow)
                            btn.BackColor = Color.Red;
                    }
                    if (winCol >= 0)
                    {
                        if (j == winCol)
                            btn.BackColor = Color.Red;
                    }
                    if (winDiag >= 0)
                    {
                        if (winDiag == 1)
                        {
                            if (i == j)
                                btn.BackColor = Color.Red;
                        }
                        if (winDiag == 2)
                        {
                            if (i == 2-j)
                                btn.BackColor = Color.Red;
                        }
                    }
                    if ((!game.host() && game.currentTurn != game.clientToken) || (game.host() && game.currentTurn != game.hostToken) || game.winner() >= 0)
                    {
                        btn.Enabled = false;
                    }

                    if (game.get(i, j) != 0)
                    {
                        btn.Text = game.get(i, j) == 1 ? "X" : "O";
                        btn.Enabled = false;
                    }
                    else
                    {
                        int localI = i;
                        int localJ = j;
                        btn.Click += (sender, e) =>
                        {

                            SendClientMoveMessage(localI, localJ);
                            game.setToCurrent(localI, localJ);
                            if (game.host())
                            {
                                game.update();
                                SendWin();
                                if (game.winner() < 0)
                                {
                                    SendClientTurnMessage();
                                }

                            }
                            RenderFromThread();
                        };
                    }
                    boardPnl.Controls.Add(btn);
                    btn.Show();
                    boardPnl.Update();

                }
            }
        }


        private void joinBtn_Click(object sender, EventArgs e)
        {
            hostBtn.Enabled = false;
            joinBtn.Enabled = false;
            socket = new Socket(SocketType.Stream, ProtocolType.Tcp);
            socket.BeginConnect(IPAddress.Parse("127.0.0.1"), 11000, SocketConnected, null);

        }

        void SocketConnected(IAsyncResult state)
        {
            if (socket.Connected)
            {
                client = new SocketCommunicationClient(socket);
                client.DataAvailable += Client_DataAvailable;
                game.newGame(false);
                RenderFromThread();
                Console.WriteLine("M-am conectat");
            }
        }

        private void resetBtn_Click(object sender, EventArgs e)
        {
            Reset();
            SendClientResetMessage();
            SendClientTurnMessage();
            RenderFromThread();
            resetBtn.Visible = false;

        }
    }
}
