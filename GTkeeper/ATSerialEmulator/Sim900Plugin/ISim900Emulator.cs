using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sim900Plugin
{
    interface ISim900Emulator
    {
        Sim900Request Request { get; set; }
    }
}
