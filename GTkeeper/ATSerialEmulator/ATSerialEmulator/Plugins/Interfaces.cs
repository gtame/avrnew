using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;

namespace ATSerialEmulator.Plugins
{

    public interface IATEmulator
    {
        void SendData(string data);
        void SendData(byte[] bytes);
    }

    public interface IATPlugin
    {
        string ProcessCommand(ref bool handled, string command);

    }


}
