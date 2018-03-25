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
    [ExportMetadata("Pattern", "AT+SAPBR")]
    class SAPBRCommand : ICommand
    {
        string ICommand.ProcessCommand(string command)
        {
            string response=string.Empty;


            string[] parser = command.ParseCommand("AT+SAPBR=");

            if (parser[0]=="2")
            {
                response = $"+SAPBR: {parser[0]},1,127.0.0.1\r\n";
            }
            else if (parser[0] == "4")
            {
                response = $"+SAPBR:\r\n";
                response = $"{parser[2]},{parser[2]}";
            }

            response += "OK\r\n";

            return response;
        }

 
    }
}
