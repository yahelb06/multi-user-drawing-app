using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DrawingApp
{
    public class LineData
    {
        public Coordinates Start { get; set; }
        public Coordinates End { get; set; }
        public string ColorHex { get; set; }

        public LineData(Coordinates start, Coordinates end, string colorHex)
        {
            Start = start;
            End = end;
            ColorHex = colorHex;
        }
    }
}
