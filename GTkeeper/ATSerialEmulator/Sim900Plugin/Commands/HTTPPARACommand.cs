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
    [ExportMetadata("Pattern", "AT+HTTPPARA")]
    class HTTPPARACommand : ICommand
    {
        string ICommand.ProcessCommand(string command)
        {
            string response = "OK\r\n";
            string[] parser = command.ParseCommand("AT+HTTPPARA=");
            if (parser[0] == "\"URL\"")
            {
                string filename = parser[1];
                System.IO.File.Copy(filename, Sim900Plugin.FILENAME_PATH);
            }

            return response;
        }
    }
}
