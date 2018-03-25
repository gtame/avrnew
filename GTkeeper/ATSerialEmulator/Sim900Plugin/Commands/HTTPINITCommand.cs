using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;


namespace Sim900Plugin.Commands
{

    [Export(typeof(ICommand))]
    [ExportMetadata("Pattern", "AT+HTTPINIT")]
    class HTTPINITCommand : ICommand
    {
        string ICommand.ProcessCommand(string command)
        {
            string response = "OK\r\n";
            return response;
        }
    }
}
