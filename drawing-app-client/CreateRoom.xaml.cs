using drawing_app_client;
using System;
using System.Collections.Generic;
using System.Text;
using System.Text.Json;
using System.Windows;
using static DrawingApp.LoginWindow;

namespace DrawingApp
{
    public partial class CreateRoom : Window
    {
        private string _roomId;
        public CreateRoom(string roomId)
        {
            InitializeComponent();
            _roomId = roomId;

            if (LoginWindow.stream != null && !string.IsNullOrEmpty(LoginWindow.username))
            {
                UserNameLabel.Text = LoginWindow.username;
            }
        }
        private async void NewProjectClick(object sender, RoutedEventArgs e)
        {
            try
            {
                ManagerRoomWindow roomWin = new ManagerRoomWindow(_roomId.ToString());
                roomWin.Show();
                this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"שגיאה בפתיחת החדר: {ex.Message}", "שגיאה", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
        private async void UploadAssetsClick(object sender, RoutedEventArgs e)
        {
            try
            {
                List<string> userPaints = await GetUserPaintsName();
                if (userPaints != null && userPaints.Count > 0)
                {
                    UploadWindow uploadWin = new UploadWindow(userPaints, _roomId);
                    uploadWin.Show();
                    this.Close();
                }
                else
                {
                    MessageBox.Show("לא נמצאו ציורים שמורים.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"שגיאה בטעינת הציורים: {ex.Message}", "שגיאה", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        public static async Task<List<string>> GetUserPaintsName()
        {
            List<string> paintsName = new List<string>();
            try
            {
                var loginData = new { username = LoginWindow.username };
                string jsonString = JsonSerializer.Serialize(loginData);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 6 + jsonBytes.Length];
                packet[0] = (byte)MessageCode.GET_USER_PAINTS;

                string lengthStr = jsonBytes.Length.ToString("D6");
                byte[] messageSize = Encoding.UTF8.GetBytes(lengthStr);
                Array.Copy(messageSize, 0, packet, 1, 6);
                Array.Copy(jsonBytes, 0, packet, 7, jsonBytes.Length);

                byte[] response = await networkManager.SendAndReceiveAsync(packet);

                using (JsonDocument doc = JsonDocument.Parse(response))
                {
                    if (doc.RootElement.TryGetProperty("paintsName", out JsonElement paintsArray))
                    {
                        foreach (JsonElement roomElement in paintsArray.EnumerateArray())
                        {
                            paintsName.Add(roomElement.GetString());
                        }
                    }
                }
                return paintsName;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                return paintsName;
            }
        }
        private void ExitButtonClick(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}