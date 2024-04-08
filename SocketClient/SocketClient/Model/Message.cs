using System;
using System.Runtime.InteropServices;

namespace SocketClient.Model
{
    [Serializable]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct Message
    {
        [MarshalAs(UnmanagedType.I4)]
        private int port;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 50)]
        private char[] data;

        public Message(int port, char[] data)
        {
            this.port = port;
            this.data = data;
        }

        public int GetPort()
        {
            return port;
        }

        public void SetPort(int value)
        {
            port = value;
        }

        public char[] GetData()
        {
            return data;
        }

        public void SetData(char[] value)
        {
            data = value;
        }

    }
}
