using DrawingApp;
using System.IO;
using System.Text;
using System.Text.Json;
using System.Windows;
using static DrawingApp.LoginWindow;

namespace drawing_app_client
{
    public partial class SignUpWindow : Window
    {
        public SignUpWindow()
        {
            InitializeComponent();
        }
        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private async void SignUp_Click(object sender, RoutedEventArgs e)
        {
            string username = UsernameTextBox.Text;
            string email = EmailTextBox.Text;
            string password = PasswordBox.Password;

            if(await SendSignUpRequest(username, password, email))
            {
                MessageBox.Show($"יוצר משתמש: {username}");
            }
        }

        private async System.Threading.Tasks.Task<bool> SendSignUpRequest(string user, string pass, string email)
        {
            try
            {
                var SignUpData = new { username = user, password = pass, mail = email };
                string jsonString = JsonSerializer.Serialize(SignUpData);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 6 + jsonBytes.Length];
                packet[0] = (byte)MessageCode.SIGNUP_REQUEST;

                string lengthStr = jsonBytes.Length.ToString("D6");
                byte[] messageSize = Encoding.UTF8.GetBytes(lengthStr);
                Array.Copy(messageSize, 0, packet, 1, 6);
                Array.Copy(jsonBytes, 0, packet, 7, jsonBytes.Length);

                byte[] response = await networkManager.SendAndReceiveAsync(packet);

                string jsonStr = Encoding.UTF8.GetString(response).TrimEnd('\0');

                using (JsonDocument doc = JsonDocument.Parse(jsonStr))
                {
                    if (doc.RootElement.TryGetProperty("Status", out JsonElement status))
                    {
                        if (status.GetInt32() == 1)
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("SignUp failed: " + ex.Message);
                return false;
            }
        }

        private void BackToLogin_Click(object sender, RoutedEventArgs e)
        {
            DrawingApp.LoginWindow loginWin = new DrawingApp.LoginWindow();
            loginWin.Show();
            this.Close();
        }
    }
}