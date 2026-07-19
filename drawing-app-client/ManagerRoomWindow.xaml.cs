using System;
using System.Collections.Generic;
using System.Text;
using System.Text.Json;
using System.Windows;
using System.Windows.Media;
using System.Windows.Shapes;
using static DrawingApp.LoginWindow;
using System.Threading.Tasks;
using Microsoft.VisualBasic;

namespace DrawingApp
{
    public partial class ManagerRoomWindow : Window
    {
        private List<LineData> _lines;
        private string _paintName;
        private string _roomId;
        private string currColor = "RED";
        private bool _isSave = true;
        private bool _isDrawing = false;
        private bool _isNewPaint = true;
        private bool _isSendingLoopRunning = false;
        private List<LineData> _pendingLines = new List<LineData>();
        Point _lastPoint;
        public ManagerRoomWindow(string roomId)
        {
            InitializeComponent();

            this._roomId = roomId;
            RoomIdTextBlock.Text = roomId;
        }

        public ManagerRoomWindow(string roomId, List<LineData> lines, string paintName)
        {
            InitializeComponent();

            this._roomId = roomId;
            _lines = lines;
            _paintName = paintName;
            _isNewPaint = false;

            _lines = lines ?? new List<LineData>();
            AddLinesToCanvas(_lines);
            RoomIdTextBlock.Text = roomId;

            InitializeRoomPaintAsync();
        }

        private async Task InitializeRoomPaintAsync()
        {
            await sendAddLines(_lines);
            _lines.Clear();
        }
        public void AddLinesToCanvas(List<LineData> lines)
        {
            DrawingCanvas.Children.Clear();

            if (lines == null) return;

            foreach (var lineData in lines)
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
        public async Task<bool> SendAddUser(bool accept, string _userToAdd)
        {
            try
            {
                var data = new { manager = LoginWindow.username, userToAdd = _userToAdd, roomId = this._roomId, accept = accept };
                string jsonString = JsonSerializer.Serialize(data);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 6 + jsonBytes.Length];
                packet[0] = (byte)MessageCode.ADD_USER_TO_ROOM;

                string lengthStr = jsonBytes.Length.ToString("D6");
                byte[] messageSize = Encoding.UTF8.GetBytes(lengthStr);
                Array.Copy(messageSize, 0, packet, 1, 6);
                Array.Copy(jsonBytes, 0, packet, 7, jsonBytes.Length);

                byte[] response = await networkManager.SendAndReceiveAsync(packet);

                return true;
            }
            catch (Exception ex)
            {
                MessageBox.Show("error happend try again");
                return false;
            }
        }

        public async Task sendAddLines(List<LineData> lines)
        {
            try
            {
                _isSave = false;
                var data = new { manager = LoginWindow.username, roomId = _roomId, line = lines };
                string jsonString = JsonSerializer.Serialize(data);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 6 + jsonBytes.Length];
                packet[0] = (byte)MessageCode.ADD_LINE_TO_PAINT;

                string lengthStr = jsonBytes.Length.ToString("D6");
                byte[] messageSize = Encoding.UTF8.GetBytes(lengthStr);
                Array.Copy(messageSize, 0, packet, 1, 6);
                Array.Copy(jsonBytes, 0, packet, 7, jsonBytes.Length);

                await networkManager.SendOnlyAsync(packet);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
        private void ColorBox_MouseLeftButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (sender is Rectangle clickedRectangle)
            {
                RedColorBox.Stroke = Brushes.Transparent;
                GreenColorBox.Stroke = Brushes.Transparent;
                BlueColorBox.Stroke = Brushes.Transparent;
                YellowColorBox.Stroke = Brushes.Transparent;
                OrangeColorBox.Stroke = Brushes.Transparent;
                PurpleColorBox.Stroke = Brushes.Transparent;
                BlackColorBox.Stroke = Brushes.Transparent;

                clickedRectangle.Stroke = Brushes.Black;
                string name = clickedRectangle.Name;

                if (name.EndsWith("ColorBox"))
                {
                    currColor = name.Replace("ColorBox", "").ToUpper();
                }
            }
        }
        private void DrawingCanvas_MouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            _isDrawing = true;
            _lastPoint = e.GetPosition(DrawingCanvas);
        }
        private void DrawingCanvas_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            if (_isDrawing)
            {
                Point currentPoint = e.GetPosition(DrawingCanvas);

                double deltaX = currentPoint.X - _lastPoint.X;
                double deltaY = currentPoint.Y - _lastPoint.Y;
                double distance = Math.Sqrt(deltaX * deltaX + deltaY * deltaY);

                if (distance < 2.5)
                {
                    return;
                }

                Line wpfLine = new Line
                {
                    X1 = _lastPoint.X,
                    Y1 = _lastPoint.Y,
                    X2 = currentPoint.X,
                    Y2 = currentPoint.Y,
                    StrokeThickness = 2
                };

                try
                {
                    var converter = new BrushConverter();
                    wpfLine.Stroke = (Brush)converter.ConvertFromString(currColor);
                }
                catch
                {
                    wpfLine.Stroke = Brushes.Black;
                }
                DrawingCanvas.Children.Add(wpfLine);

                Coordinates startCoord = new Coordinates((uint)_lastPoint.X, (uint)_lastPoint.Y);
                Coordinates endCoord = new Coordinates((uint)currentPoint.X, (uint)currentPoint.Y);
                LineData newLine = new LineData(startCoord, endCoord, currColor);

                lock (_pendingLines)
                {
                    _pendingLines.Add(newLine);
                }

                _lastPoint = currentPoint;

                StartSendingLoopIfNeeded();
            }
        }


        private void DrawingCanvas_MouseUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (_isDrawing)
            {
                _isDrawing = false;

                StartSendingLoopIfNeeded();
            }
        }

        private async void StartSendingLoopIfNeeded()
        {
            if (_isSendingLoopRunning) return;
            _isSendingLoopRunning = true;

            try
            {
                while (true)
                {
                    List<LineData> linesToSend = null;

                    lock (_pendingLines)
                    {
                        if (_pendingLines.Count > 0)
                        {
                            linesToSend = new List<LineData>(_pendingLines);
                            _pendingLines.Clear();
                        }
                    }

                    if (linesToSend != null)
                    {
                        await sendAddLines(linesToSend);
                    }

                    await Task.Delay(80);

                    lock (_pendingLines)
                    {
                        if (_pendingLines.Count == 0 && !_isDrawing)
                        {
                            _isSendingLoopRunning = false;
                            break;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Sending loop error: {ex.Message}");
                _isSendingLoopRunning = false;
            }
        }

        private async void SaveButton_Click(object sender, RoutedEventArgs e)
        {
            if (_isNewPaint)
            {
                string paintName = Interaction.InputBox("Enter Paint Name:", "Save Paint", "");

                if (string.IsNullOrWhiteSpace(paintName))
                {
                    return;
                }

                _paintName = paintName;
                _isNewPaint = false;
            }
            await sendSaveMsg();
        }

        private async Task sendSaveMsg()
        {
            try
            {
                _isSave = true;
                var data = new { roomId = _roomId, manager = LoginWindow.username, paintName = _paintName };
                string jsonString = JsonSerializer.Serialize(data);
                byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

                byte[] packet = new byte[1 + 6 + jsonBytes.Length];
                packet[0] = (byte)MessageCode.SAVE_PAINT;

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
                        if (status.GetInt32() != 1)
                        {
                            MessageBox.Show("some error happend try again");
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
        private void ExitButtonClick(object sender, RoutedEventArgs e)
        {
            if (!_isSave)
            {
                MessageBoxResult result = MessageBox.Show(
                $"Do you want to save the changes?",
                "Save changes",
                MessageBoxButton.YesNo
                );
                if (result == MessageBoxResult.Yes)
                {
                    SaveButton_Click(sender, e);
                }
            }
            CreateRoom createRoomWin = new CreateRoom(_roomId);
            createRoomWin.Show();
            this.Close();
        }
    }
}