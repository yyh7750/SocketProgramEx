using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using SocketClient.Model;
using System.Diagnostics;
using System.Windows.Controls;
using System.Windows;

namespace SocketClient.ViewModel
{
    public class MessageViewModel
    {
        private UdpClient client;
        private IPEndPoint serverEndPoint;
        private const string ServerIp = "127.0.0.1";
        private const int serverPort = 9999;
        private const int clientPort = 9998;
        private ListBox listBox;
        private Message sendMessage;
        private ListItem listItem;

        public MessageViewModel(ListBox listBox)
        {
            listItem = new ListItem();
            this.listBox = listBox;
            Random ran = new Random();
            sendMessage = new Message(ran.Next(2_147_483_647)); // ID 설정
            InitializeSocket();
            StartReceiving();
        }

        public void InitializeSocket()
        {
            try
            {
                client = new UdpClient(clientPort);
                // 서버 IP와 포트를 설정.
                serverEndPoint = new IPEndPoint(IPAddress.Parse(ServerIp), serverPort);
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
                    while (true)
                    {
                        IPEndPoint remoteEndPoint = new IPEndPoint(IPAddress.Any, 0);
                        byte[] data = client.Receive(ref remoteEndPoint);

                        // 수신된 바이트 배열을 구조체로 변환
                        Message message = BytesToMessage(data);

                        string recvData = new string(message.GetData());
                        StringBuilder sb = new StringBuilder();
                        sb.Append("<- ID: ").Append(message.GetId()).Append(", ");
                        sb.Append("PORT: ").Append(sendMessage.GetPort()).Append(", ");
                        sb.Append("Data: ").Append(recvData.Substring(0, recvData.IndexOf('\0')));
                        listItem.Content = sb.ToString();
                        listItem.Alignment = HorizontalAlignment.Left;

                        listBox.Dispatcher.Invoke(() =>
                        {
                            listBox.Items.Add(listItem);
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
            sendMessage.SetPort(clientPort);
            sendMessage.SetData(data);
            byte[] byteData = MessageToBytes(sendMessage);

            try
            {
                // 데이터 전송
                client.Send(byteData, byteData.Length, serverEndPoint);
                StringBuilder sb = new StringBuilder();
                sb.Append("-> ID: ").Append(sendMessage.GetId()).Append(", ");
                sb.Append("PORT: ").Append(sendMessage.GetPort()).Append(", ");
                sb.Append("Data: ").Append(sendMessage.GetData());
                listItem.Content = sb.ToString();
                listItem.Alignment = HorizontalAlignment.Right;

                listBox.Dispatcher.Invoke(() =>
                {
                    listBox.Items.Add(listItem);
                });
                return true;
            }
            catch (Exception e)
            {
                return "메시지 전송 오류: " + e.Message;
            }
        }

        public byte[] MessageToBytes(Message message)
        {
            byte[] idBytes = BitConverter.GetBytes(message.GetId());
            byte[] portBytes = BitConverter.GetBytes(message.GetPort());
            byte[] dataBytes = Encoding.UTF8.GetBytes(message.GetData());

            byte[] result = new byte[idBytes.Length + portBytes.Length + dataBytes.Length];
            idBytes.CopyTo(result, 0);
            portBytes.CopyTo(result, idBytes.Length);
            dataBytes.CopyTo(result, idBytes.Length + portBytes.Length);

            return result;
        }
    }
}
