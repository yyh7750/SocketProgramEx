using System;
using System.Runtime.InteropServices;

namespace SocketClient.Model
{
    [Serializable]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct Message
    {
        [MarshalAs(UnmanagedType.I4)]
        private int id;
        [MarshalAs(UnmanagedType.I4)]
        private int port;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 50)]
        private char[] data;

        public Message(int id)
        {
            this.id = id;
            this.port = 0;
            this.data = new char[50];
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

        public int GetId()
        {
            return id;
        }

        public void SetId(int value)
        {
            id = value;
        }
    }
}
