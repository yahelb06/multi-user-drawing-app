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

            // TODO: כאן תוסיף את בדיקות התקינות ואת שליחת בקשת ה-SIGNUP_REQUEST לשרת ה-C++
            MessageBox.Show($"יוצר משתמש: {username}");
        }

        private async System.Threading.Tasks.Task<bool> SendSignUpRequest(string user, string pass, string email)
        {
            try
            {
                var SignUpData = new { username = user, password = pass, mail = email };
                string jsonString = JsonSerializer.Serialize(SignUpData);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 4 + jsonBytes.Length];
                packet[0] = (byte)MessageCode.SIGNUP_REQUEST;

                string lengthStr = jsonBytes.Length.ToString("D4");
                byte[] messageSize = Encoding.UTF8.GetBytes(lengthStr);
                Array.Copy(messageSize, 0, packet, 1, 4);
                Array.Copy(jsonBytes, 0, packet, 5, jsonBytes.Length);

                await stream.WriteAsync(packet, 0, packet.Length);

                byte[] buffer = new byte[1024];
                int bytesRead = await stream.ReadAsync(buffer, 0, buffer.Length);

                if (buffer[0] == (byte)MessageCode.ERROR_CODE)
                {
                    ShowErrorMsg(buffer);
                    return false;
                }

                return true;
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