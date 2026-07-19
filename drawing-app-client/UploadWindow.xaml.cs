using System;
using System.Collections.Generic;
using System.Text;
using System.Text.Json;
using System.Windows;
using static DrawingApp.LoginWindow;

namespace DrawingApp
{
    public partial class UploadWindow : Window
    {
        private List<string> _userPaints;
        private string _roomId;
        public UploadWindow(List<string> paintsList, string roomId)
        {
            InitializeComponent();

            _userPaints = paintsList;
            _roomId = roomId;

            PaintsListBox.ItemsSource = _userPaints;
        }

        private async void ConfirmSelection_Click(object sender, RoutedEventArgs e)
        {
            if (PaintsListBox.SelectedItem is string selectedPaint)
            {
                List<LineData> paintLine = await getPaint(selectedPaint);
                if(await uploadPaint(_roomId, selectedPaint, paintLine))
                {
                    ManagerRoomWindow roomWin = new ManagerRoomWindow(_roomId, paintLine, selectedPaint);
                    roomWin.Show();
                    this.Close();
                }
            }
            else
            {
                MessageBox.Show("אנא בחר ציור מהרשימה לפני האישור.");
            }
        }
        private async Task<List<LineData>> getPaint(string paintName)
        {
            List<LineData> newLines = new List<LineData>();
            try
            {
                var data = new { user = LoginWindow.username, paintName = paintName };
                string jsonString = JsonSerializer.Serialize(data);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 6 + jsonBytes.Length];
                packet[0] = (byte)MessageCode.GET_PAINT_BY_NAME;

                string lengthStr = jsonBytes.Length.ToString("D6");
                byte[] messageSize = Encoding.UTF8.GetBytes(lengthStr);
                Array.Copy(messageSize, 0, packet, 1, 6);
                Array.Copy(jsonBytes, 0, packet, 7, jsonBytes.Length);

                byte[] response = await networkManager.SendAndReceiveAsync(packet);

                using (JsonDocument doc = JsonDocument.Parse(response))
                {
                    if (doc.RootElement.TryGetProperty("PaintLines", out JsonElement linesArray))
                    {
                        foreach (JsonElement line in linesArray.EnumerateArray())
                        {
                            uint x1 = line.GetProperty("start").GetProperty("x").GetUInt32();
                            uint y1 = line.GetProperty("start").GetProperty("y").GetUInt32();

                            uint x2 = line.GetProperty("end").GetProperty("x").GetUInt32();
                            uint y2 = line.GetProperty("end").GetProperty("y").GetUInt32();

                            string color = line.GetProperty("color").GetString();

                            LineData newLine = new LineData(new Coordinates(x1, y1), new Coordinates(x2, y2), color);

                            newLines.Add(newLine);
                        }
                    }
                    return newLines;
                }
            }
            catch (Exception e)
            {
                return newLines;
            }
        }

        private async Task<bool> uploadPaint(string roomId, string paintName, List<LineData> lines)
        {
            try
            {
                var data = new { manager = LoginWindow.username, roomId = _roomId, paint = new { paintName = paintName, lines = lines } };
                string jsonString = JsonSerializer.Serialize(data);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 6 + jsonBytes.Length];
                packet[0] = (byte)MessageCode.UPLOAD_PAINT_TO_ROOM;

                string lengthStr = jsonBytes.Length.ToString("D6");
                byte[] messageSize = Encoding.UTF8.GetBytes(lengthStr);
                Array.Copy(messageSize, 0, packet, 1, 6);
                Array.Copy(jsonBytes, 0, packet, 7, jsonBytes.Length);

                byte[] response = await networkManager.SendAndReceiveAsync(packet);
                if (response.Length >= 7)
                {
                    string jsonStr = Encoding.UTF8.GetString(response).TrimEnd('\0');

                    using (JsonDocument doc = JsonDocument.Parse(jsonStr))
                    {
                        if (doc.RootElement.TryGetProperty("status", out JsonElement statusElem))
                        {
                            int status = statusElem.GetInt32();
                            if (status == 1)
                            {
                                return true;
                            }
                        }
                    }
                }
                return false;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                return false;
            }
        }


        private void ClosePopUp_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}