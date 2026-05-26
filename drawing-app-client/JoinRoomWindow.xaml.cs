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
                if(await WaitForMsgFromServer())
                {
                    RoomWindow roomWin = new RoomWindow(roomId);
                    roomWin.Show();
                    this.Close();
                }
            }
        }

        private async System.Threading.Tasks.Task<bool> SendJoinRoomRequest(string roomId)
        {
            try
            {
                var loginData = new { user = LoginWindow.username, roomId = roomId };
                string jsonString = JsonSerializer.Serialize(loginData);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 4 + jsonBytes.Length];
                packet[0] = (byte)MessageCode.LOGIN_REQUEST;

                string lengthStr = jsonBytes.Length.ToString("D4");
                byte[] messageSize = Encoding.UTF8.GetBytes(lengthStr);
                Array.Copy(messageSize, 0, packet, 1, 4);
                Array.Copy(jsonBytes, 0, packet, 5, jsonBytes.Length);

                await LoginWindow.stream.WriteAsync(packet, 0, packet.Length);

                byte[] buffer = new byte[1024];
                int bytesRead = await stream.ReadAsync(buffer, 0, buffer.Length);

                if (buffer[0] == (byte)MessageCode.ERROR_CODE)
                {
                    LoginWindow.ShowErrorMsg(buffer);
                    return false;
                }
                return true;
            }
            catch (Exception ex)
            {
                MessageBox.Show("Login failed: " + ex.Message);
                return false;
            }
        }

        static private async System.Threading.Tasks.Task<bool> WaitForMsgFromServer()
        {
            byte[] buffer = new byte[1024];
            int bytesRead = await stream.ReadAsync(buffer, 0, buffer.Length);

            if (buffer[0] == (byte)LoginWindow.MessageCode.ERROR_CODE)
            {
                LoginWindow.ShowErrorMsg(buffer);
                return false;
            }
            return true;
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
