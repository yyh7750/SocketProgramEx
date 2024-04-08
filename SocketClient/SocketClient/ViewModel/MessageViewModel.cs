using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using SocketClient.Model;
using System.Diagnostics;
using System.Windows.Controls;

namespace SocketClient.ViewModel
{
    public class MessageViewModel
    {
        private UdpClient client;
        private IPEndPoint serverEndPoint;
        private const string ServerIp = "127.0.0.1";
        private const int ServerPort = 9999;
        private const int ClientReceivePort = 9998;
        private bool isRunning = true;
        private ListBox listBox;

        public MessageViewModel(ListBox listBox)
        {
            this.listBox = listBox;
            InitializeSocket();
            StartReceiving();
        }

        public void InitializeSocket()
        {
            try
            {
                client = new UdpClient(ClientReceivePort);
                // 서버 IP와 포트를 설정.
                serverEndPoint = new IPEndPoint(IPAddress.Parse(ServerIp), ServerPort);
                Debug.WriteLine("초기화 실행");
            }
            catch (Exception ex)
            {
                Debug.WriteLine("UDP 클라이언트 초기화 오류: " + ex.Message);
            }
        }

        public async Task StartReceiving()
        {
            Debug.WriteLine("수신");
            await Task.Run(() =>
            {
                try
                {
                    while (isRunning)
                    {
                        IPEndPoint remoteEndPoint = new IPEndPoint(IPAddress.Any, 0);
                        byte[] data = client.Receive(ref remoteEndPoint);

                        // 수신된 바이트 배열을 구조체로 변환
                        Message message = BytesToMessage(data);

                        string receivedData = new string(message.GetData());
                        Debug.WriteLine("수신된 데이터: " + receivedData);

                        listBox.Dispatcher.Invoke(() =>
                        {
                            listBox.Items.Add(receivedData.Substring(0, receivedData.IndexOf('\0')));
                        });
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine("수신 오류: " + ex.Message);
                }
            });
        }

        public Message BytesToMessage(byte[] byteArray)
        {
            IntPtr ptr = Marshal.AllocHGlobal(byteArray.Length);
            Marshal.Copy(byteArray, 0, ptr, byteArray.Length);

            Message message = (Message)Marshal.PtrToStructure(ptr, typeof(Message));

            Marshal.FreeHGlobal(ptr);
            return message;
        }

        public object SendMessage(char[] data)
        {
            Message message = new Message(ServerPort, data);
            byte[] byteData = MessageToBytes(message);

            try
            {
                // 데이터 전송
                client.Send(byteData, byteData.Length, serverEndPoint);
                return true;
            }
            catch (Exception e)
            {
                return "메시지 전송 오류: " + e.Message;
            }
        }

        public byte[] MessageToBytes(Message message)
        {
            byte[] portBytes = BitConverter.GetBytes(message.GetPort());
            byte[] dataBytes = Encoding.UTF8.GetBytes(message.GetData());

            byte[] result = new byte[portBytes.Length + dataBytes.Length];
            portBytes.CopyTo(result, 0);
            dataBytes.CopyTo(result, portBytes.Length);

            return result;
        }
    }
}
