using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using ATSerialEmulator.Plugins;

namespace Sim900Plugin.Commands
{
    [Export(typeof(ICommand))]
    [ExportMetadata("Pattern", "AT+HTTPDATA")]
    class HTTPDATACommand : ICommand
    {

 
        string ICommand.ProcessCommand(string command)
        {
            string response = "DOWNLOAD\r\nOK\r\n";

            
            return response;
        }
    }
}
