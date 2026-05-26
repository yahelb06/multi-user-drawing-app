using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DrawingApp
{
    public class Coordinates
    {
        public uint X { get; set; }
        public uint Y { get; set; }

        public Coordinates(uint x, uint y)
        {
            X = x;
            Y = y;
        }
    }
}
