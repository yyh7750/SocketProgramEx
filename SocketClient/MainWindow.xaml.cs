using SocketClient.ViewModel;
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Windows;

namespace SocketClient
{
    public partial class MainWindow : Window
    {
        private MessageViewModel clientViewModel;

        public MainWindow()
        {
            InitializeComponent();
            clientViewModel = new MessageViewModel(listBox);
        }

        private void SendMessage(object sender, RoutedEventArgs e)
        {
            string message = textBox.Text + '\0';
            object result = clientViewModel.SendMessage(message.ToCharArray());

            if (result is string)
            {
                MessageBox.Show(result.ToString());
            }
            else
            {
                textBox.Text = "";
            }
        }
    }
}
