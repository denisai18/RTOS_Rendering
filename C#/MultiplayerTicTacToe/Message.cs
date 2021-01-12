using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace MultiplayerTicTacToe
{
    class Message
    {
        public event EventHandler<MessageEventArgs> MessageDecoded;
        protected virtual void OnMessageDecoded(MessageEventArgs args)
        {
            MessageDecoded?.Invoke(this, args);
        }

        public int MessageSize { get; set; }
        public const int BufferSize = 1024;
        public byte[] Buffer { get; set; } = new byte[BufferSize];
        public List<byte> Payload { get; set; } = new List<byte>();
        public int Offset { get; set; } = 0;
        public ulong BytesRead { get; set; } = 0;
        public Socket WorkSocket { get; set; }
        public Message()
        {
            WorkSocket = null;
            BytesRead = 0;
            MessageSize = 0;
        }

        public void ReadMessage()
        {
            int index = 0;
            ulong tempBytesRead = BytesRead;
            if (MessageSize == 0 && BytesRead > sizeof(int))
            {
                byte[] msgSize = new byte[sizeof(int)];
                Array.Copy(Buffer, msgSize, sizeof(int));
                MessageSize = BitConverter.ToInt32(msgSize, 0);
                index += sizeof(int);
                tempBytesRead -= sizeof(int);
            }
            if (MessageSize != 0)
            {
                ulong rem = (ulong)(MessageSize - Payload.Count);
                if (rem <= tempBytesRead)
                {
                    byte[] remBytes = new byte[rem];
                    Array.Copy(Buffer, index, remBytes, 0, (int)rem);
                    Payload.AddRange(remBytes);
                    index += (int)rem;
                    tempBytesRead -= rem;

                }
                else
                {
                    byte[] remBytes = new byte[tempBytesRead];
                    Array.Copy(Buffer, index, remBytes, 0, (int)tempBytesRead);
                    Payload.AddRange(remBytes);
                    index += (int)rem;
                    tempBytesRead = 0;
                }
            }

            if (Payload.Count == MessageSize)
            {
                MessageEventArgs args = new MessageEventArgs();
                args.Payload = new byte[Payload.Count];
                Array.Copy(Payload.ToArray(), args.Payload, Payload.Count);
                OnMessageDecoded(args);
                Reset();
            }
            if (tempBytesRead != 0)
            {
                Array.Copy(Buffer, index, Buffer, 0, (int)tempBytesRead);
            }
            Offset = (int)tempBytesRead;
            BytesRead = tempBytesRead;

            if (BytesRead >= sizeof(int))
            {
                ReadMessage();
            }
        }
        private void Reset()
        {
            Payload.Clear();
            MessageSize = 0;

        }
    }
    public class MessageEventArgs : EventArgs
    {
        public uint Type { get; set; }
        public byte[] Payload { get; set; }
    }
}
