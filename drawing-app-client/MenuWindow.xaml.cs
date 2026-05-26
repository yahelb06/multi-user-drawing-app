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

namespace DrawingApp
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class MenuWindow : Window
    {
        private string roomId = string.Empty;
        public MenuWindow()
        {
            InitializeComponent();

            UserNameLabel.Text = "שלום, " + LoginWindow.username;
        }

        private async void CreateRoomClick(object sender, RoutedEventArgs e)
        {
            if(await SendCreateRoomRequest(LoginWindow.username))
            {
                RoomWindow roomWin = new RoomWindow();
                roomWin.Show();
                this.Close();
            }
        }

        private async System.Threading.Tasks.Task<bool> SendCreateRoomRequest(string user)
        {
            try
            {
                var loginData = new { username = LoginWindow.username };
                string jsonString = JsonSerializer.Serialize(loginData);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 4 + jsonBytes.Length];
                packet[0] = (byte)LoginWindow.MessageCode.CREATE_ROOM;

                string lengthStr = jsonBytes.Length.ToString("D4");
                byte[] messageSize = Encoding.UTF8.GetBytes(lengthStr);
                Array.Copy(messageSize, 0, packet, 1, 4);
                Array.Copy(jsonBytes, 0, packet, 5, jsonBytes.Length);

                await LoginWindow.stream.WriteAsync(packet, 0, packet.Length);

                byte[] buffer = new byte[1024];
                int bytesRead = await LoginWindow.stream.ReadAsync(buffer, 0, buffer.Length);

                if (buffer[0] == (byte)LoginWindow.MessageCode.ERROR_CODE)
                {
                    LoginWindow.ShowErrorMsg(buffer);
                    return false;
                }
                if (bytesRead >= 5)
                {
                    string jsonStr = Encoding.UTF8.GetString(buffer, 5, bytesRead - 5);

                    using (JsonDocument doc = JsonDocument.Parse(jsonStr))
                    {
                        roomId = doc.RootElement.GetProperty("RoomId").GetString();
                    }
                }
                return true;
            }
            catch (Exception ex)
            {
                MessageBox.Show("Create room failed: " + ex.Message);
                return false;
            }
        }
        private async void JoinRoomClick(object sender, RoutedEventArgs e)
        {
            JoinRoomWindow joinRoomWin = new JoinRoomWindow();
            joinRoomWin.Show();
            this.Close();
        }

        private void ExitButtonClick(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
