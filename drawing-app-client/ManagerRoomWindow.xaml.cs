using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Media;
using System.Windows.Shapes;

namespace DrawingApp
{
    public partial class ManagerRoomWindow : Window
    {
        public ManagerRoomWindow(string roomId)
        {
            InitializeComponent();

            // דוגמה קטנה להרצה כדי שתראה שזה עובד מיד כשהחלון נפתח:
            List<LineData> demoLines = new List<LineData>
            {
                new LineData(new Coordinates(50, 50), new Coordinates(200, 200), "#966859"),
                new LineData(new Coordinates(200, 200), new Coordinates(400, 50), "Red")
            };

            DrawLinesOnCanvas(demoLines);
            RoomIdTextBlock.Text = roomId;
        }
        public void DrawLinesOnCanvas(List<LineData> lines)
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
    }
}