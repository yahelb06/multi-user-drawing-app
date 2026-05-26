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
        public CreateRoom()
        {
            InitializeComponent();

            if (LoginWindow.stream != null && !string.IsNullOrEmpty(LoginWindow.username))
            {
                UserNameLabel.Text = LoginWindow.username;
            }
        }
        private async void NewProjectClick(object sender, RoutedEventArgs e)
        {
            try
            {
                int roomId = await SendCreateRoom();
                ManagerRoomWindow roomWin = new ManagerRoomWindow(roomId.ToString());
                roomWin.Show();
                this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"שגיאה בפתיחת החדר: {ex.Message}", "שגיאה", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
        private void UploadAssetsClick(object sender, RoutedEventArgs e)
        {
            try
            {
                List<string> userPaints = GetUserPaintsName();
                UploadWindow uploadWin = new UploadWindow(userPaints);
                uploadWin.Show();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"שגיאה בטעינת הציורים: {ex.Message}", "שגיאה", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        public static async Task<int> SendCreateRoom()
        {
            try
            {
                var createRoom = new { username = LoginWindow.username };
                string jsonString = JsonSerializer.Serialize(createRoom);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 4 + jsonBytes.Length];
                packet[0] = (byte)MessageCode.CREATE_ROOM;

                string lengthStr = jsonBytes.Length.ToString("D4");
                byte[] messageSize = Encoding.UTF8.GetBytes(lengthStr);
                Array.Copy(messageSize, 0, packet, 1, 4);
                Array.Copy(jsonBytes, 0, packet, 5, jsonBytes.Length);

                await LoginWindow.stream.WriteAsync(packet, 0, packet.Length);
                await LoginWindow.stream.FlushAsync();

                byte[] buffer = new byte[1024];
                int bytesRead = stream.Read(buffer, 0, buffer.Length);

                if (buffer[0] == (byte)MessageCode.ERROR_CODE)
                {
                    LoginWindow.ShowErrorMsg(buffer);
                    return -1;
                }
                else
                {
                    if (bytesRead >= 5)
                    {
                        string jsonStr = Encoding.UTF8.GetString(buffer, 5, bytesRead - 5);

                        using (JsonDocument doc = JsonDocument.Parse(jsonStr))
                        {
                            return doc.RootElement.GetProperty("roomId").GetInt32();
                        }
                    }
                }
                return -1;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                return -1;
            }
        }

        public static List<string> GetUserPaintsName()
        {
            List<string> paintsName = new List<string>();
            try
            {
                var loginData = new { username = LoginWindow.username };
                string jsonString = JsonSerializer.Serialize(loginData);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 4 + jsonBytes.Length];
                packet[0] = (byte)MessageCode.GET_USER_PAINTS;

                string lengthStr = jsonBytes.Length.ToString("D4");
                byte[] messageSize = Encoding.UTF8.GetBytes(lengthStr);
                Array.Copy(messageSize, 0, packet, 1, 4);
                Array.Copy(jsonBytes, 0, packet, 5, jsonBytes.Length);

                LoginWindow.stream.WriteAsync(packet, 0, packet.Length);

                byte[] buffer = new byte[1024];
                int bytesRead = stream.Read(buffer, 0, buffer.Length);

                if (buffer[0] == (byte)MessageCode.ERROR_CODE)
                {
                    LoginWindow.ShowErrorMsg(buffer);
                    paintsName.Add("-1");
                }
                else
                {
                    if (bytesRead >= 5)
                    {
                        string jsonStr = Encoding.UTF8.GetString(buffer, 5, bytesRead - 5);

                        using (JsonDocument doc = JsonDocument.Parse(jsonStr))
                        {
                            if (doc.RootElement.TryGetProperty("paintsName", out JsonElement paintsArray))
                            {
                                foreach (JsonElement roomElement in paintsArray.EnumerateArray())
                                {
                                    paintsName.Add(roomElement.GetString());
                                }
                            }
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