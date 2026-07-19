using drawing_app_client;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using static DrawingApp.LoginWindow;

namespace DrawingApp
{
    /// <summary>
    /// Interaction logic for JoinRoomWindow.xaml
    /// </summary>
    public partial class JoinRoomWindow : Window
    {
        public JoinRoomWindow()
        {
            InitializeComponent();

            UserNameLabel.Text = LoginWindow.username;
        }

        private async void JoinRoomClick(object sender, RoutedEventArgs e)
        {
            string roomId = RoomCodeTextBox.Text;

            if(await SendJoinRoomRequest(roomId))
            {
                WaitingStatus.Visibility = Visibility.Visible;
            }
        }

        private async System.Threading.Tasks.Task<bool> SendJoinRoomRequest(string roomId)
        {
            try
            {
                var loginData = new { username = LoginWindow.username, roomId = roomId };
                string jsonString = JsonSerializer.Serialize(loginData);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 6 + jsonBytes.Length];
                packet[0] = (byte)MessageCode.JOIN_ROOM;

                string lengthStr = jsonBytes.Length.ToString("D6");
                byte[] messageSize = Encoding.UTF8.GetBytes(lengthStr);
                Array.Copy(messageSize, 0, packet, 1, 6);
                Array.Copy(jsonBytes, 0, packet, 7, jsonBytes.Length);

                byte[] response = await networkManager.SendAndReceiveAsync(packet);
                return true;
            }
            catch (Exception ex)
            {
                MessageBox.Show("join failed: " + ex.Message);
                return false;
            }
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
