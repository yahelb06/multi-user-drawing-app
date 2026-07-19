using System;
using System.Collections.Generic;
using System.Text;
using System.Text.Json;
using System.Windows;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Threading.Tasks;

namespace DrawingApp
{
    public partial class RoomWindow : Window
    {
        private List<LineData> _lines;
        private string _roomId;
        public RoomWindow(string roomId)
        {
            InitializeComponent();
            _roomId = roomId;
            if (_lines == null) _lines = new List<LineData>();

            getPaintFromRoom();

            RoomIdTextBlock.Text = roomId;
        }

        private async Task getPaintFromRoom()
        {
            try
            {
                var data = new { roomId = _roomId };
                string jsonString = JsonSerializer.Serialize(data);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 6 + jsonBytes.Length];
                packet[0] = (byte)LoginWindow.MessageCode.GET_PAINT_FROM_ROOM;

                string lengthStr = jsonBytes.Length.ToString("D6");
                byte[] messageSize = Encoding.UTF8.GetBytes(lengthStr);
                Array.Copy(messageSize, 0, packet, 1, 6);
                Array.Copy(jsonBytes, 0, packet, 7, jsonBytes.Length);

                byte[] response = await networkManager.SendAndReceiveAsync(packet);
                
                List<LineData> newLines = new List<LineData>();
                string jsonStr = Encoding.UTF8.GetString(response).TrimEnd('\0');
                using (JsonDocument doc = JsonDocument.Parse(jsonStr))
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
                    AddLinesToCanvas(newLines);
                }
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
            }
        }
        public void AddLinesToCanvas(List<LineData> newLines)
        {
            if (newLines == null || newLines.Count == 0) return;

            _lines.AddRange(newLines);

            foreach (var lineData in newLines)
            {
                Line wpfLine = new Line();

                wpfLine.X1 = lineData.Start.X;
                wpfLine.Y1 = lineData.Start.Y;
                wpfLine.X2 = lineData.End.X;
                wpfLine.Y2 = lineData.End.Y;

                wpfLine.StrokeThickness = 2;

                try
                {
                    var converter = new BrushConverter();
                    wpfLine.Stroke = (Brush)converter.ConvertFromString(lineData.ColorHex);
                }
                catch
                {
                    wpfLine.Stroke = Brushes.Black;
                }

                DrawingCanvas.Children.Add(wpfLine);
            }
        }
    }
}