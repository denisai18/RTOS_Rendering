using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace MultiplayerTicTacToe
{
    public class SocketCommunicationClient
    {
        public int Type { get; set; }
        public long Id { get; set; }
        public Socket Socket { get; set; }
        public string Name { get; set; }

        public event EventHandler<MessageEventArgs> DataAvailable;
        protected virtual void OnDataAvailable(MessageEventArgs args)
        {
            DataAvailable?.Invoke(this, args);
        }
        public SocketCommunicationClient(Socket soket)
        {
            Socket = soket;
            Message state = new Message();
            state.MessageDecoded += State_MessageDecoded;
            state.WorkSocket = Socket;
            Socket.BeginReceive(state.Buffer, state.Offset, Message.BufferSize - state.Offset, 0,
                new AsyncCallback(ReadCallback), state);
        }

        private void State_MessageDecoded(object sender, MessageEventArgs e)
        {
            OnDataAvailable(e);
        }

        public void ReadCallback(IAsyncResult ar)
        {
            String content = String.Empty;

            // Retrieve the state object and the handler socket  
            // from the asynchronous state object.  
            Message state = (Message)ar.AsyncState;
            Socket handler = state.WorkSocket;

            // Read data from the client socket.
            try
            {
                int bytesRead = handler.EndReceive(ar);

                if (bytesRead > 0)
                {

                    state.BytesRead += (ulong)bytesRead;

                    // There  might be more data, so store the data received so far.  

                    // Check for end-of-file tag. If it is not there, read
                    // more data.  
                    state.ReadMessage();
                    // Not all data received. Get more.  
                    handler.BeginReceive(state.Buffer, state.Offset, Message.BufferSize - state.Offset, 0,
                    new AsyncCallback(ReadCallback), state);
                }
            }
            catch (SocketException e)
            {
                Console.WriteLine(e.Message);
            }
        }


        public void SendData(byte[] data)
        {
            Socket.BeginSend(data, 0, data.Length, SocketFlags.None, SendCallback, Socket);
        }
        private void SendCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the socket from the state object.  
                Socket client = (Socket)ar.AsyncState;

                // Complete sending the data to the remote device.  
                int bytesSent = client.EndSend(ar);

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
    }
}