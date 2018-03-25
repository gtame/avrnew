using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;


namespace Sim900Plugin.Commands
{

    [Export(typeof(ICommand))]
    [ExportMetadata("Pattern", "AT+HTTPTERM")]
    class HTTPTERMCommand : ICommand
    {
        string ICommand.ProcessCommand(string command)
        {
            string response = "OK";
            return response;
        }
    }
}
