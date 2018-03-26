using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using System.IO;

namespace ATSerialEmulator.Plugins
{

    public interface IATEmulator
    {
        void SendData(string data);
        void SendData(byte[] bytes);

        Stream BaseStream { get; }

        bool ProcessMessages{ get; set; }
    }

    public interface IATPlugin
    {
        string ProcessCommand(ref bool handled, string command);

    }


}
