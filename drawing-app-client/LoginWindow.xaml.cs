using drawing_app_client;
using Microsoft.VisualBasic;
using System.Net.Sockets;
using System.Text;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace DrawingApp
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class LoginWindow : Window
    {
        public static NetworkStream stream;
        public static string username = "";
        public LoginWindow()
        {
            InitializeComponent();
            ConnectToServer();
        }

        private void ConnectToServer()
        {
            try
            {
                TcpClient client = new TcpClient("127.0.0.1", 8826);
                stream = client.GetStream();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Could not connect to server: " + ex.Message);
            }
        }

        public enum MessageCode : byte
        {
            ERROR_CODE = 90,
            LOGIN_REQUEST = 100,
            SIGNUP_REQUEST = 110,
            USER_LOG_OUT_REQUEST = 120,
            REMOVE_USER = 130,
            ROOM_LOG_OUT_REQUEST = 140,
            CREATE_ROOM = 150,
            JOIN_ROOM = 160,
            ADD_USER_TO_ROOM = 170,
            REMOVE_USER_FROM_ROOM = 180,
            REMOVE_PAINT_FROM_ROOM = 190,
            ADD_PAINT_TO_ROOM = 200,
            GET_USERS_IN_ROOM = 210,
            ACCEPT_USER = 220,
            GET_USER_PAINTS = 230
        }

        private async void Login_Click(object sender, RoutedEventArgs e)
        {
            string user = UsernameTextBox.Text;
            username = user;
            string pass = PasswordBox.Password;

            if (await SendLoginRequest(user, pass))
            {
                MenuWindow menuWin = new MenuWindow();
                menuWin.Show();
                this.Close();
            }
        }
        private void Signup_Click(object sender, RoutedEventArgs e)
        {
            SignUpWindow signupWin = new SignUpWindow();
            signupWin.Show();
            this.Close();
        }

        private async System.Threading.Tasks.Task<bool> SendLoginRequest(string user, string pass)
        {
            try
            {
                var loginData = new { username = user, password = pass };
                string jsonString = JsonSerializer.Serialize(loginData);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 4 + jsonBytes.Length];
                packet[0] = (byte)MessageCode.LOGIN_REQUEST;

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
                MessageBox.Show("Login failed: " + ex.Message);
                return false;
            }
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        public static void ShowErrorMsg(byte[] buffer)
        {
            string errlengthStr = Encoding.UTF8.GetString(buffer, 1, 4);
            int errorLength = int.Parse(errlengthStr);

            string errorJson = Encoding.UTF8.GetString(buffer, 5, errorLength);
            using (JsonDocument doc = JsonDocument.Parse(errorJson))
            {
                string errorMessage = doc.RootElement.GetProperty("message").GetString();
                MessageBox.Show(errorMessage, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }
}