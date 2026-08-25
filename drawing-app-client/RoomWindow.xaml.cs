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
        public RoomWindow(string roomId, List<LineData> lines)
        {
            InitializeComponent();
            _roomId = roomId;
            _lines = lines;

            RoomIdTextBlock.Text = roomId;
            AddLinesToCanvas(_lines);
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