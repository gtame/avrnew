using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using System.IO;

namespace Sim900Plugin.Commands
{

    [Export(typeof(ICommand))]
    [ExportMetadata("Pattern", "AT+HTTPREAD")]
    class HTTPREADCommand : ICommand
    {
        string ICommand.ProcessCommand(string command)
        {
            string response = string.Empty;
            string [] parameters= command.ParseCommand("AT+HTTPREAD=");

            int desde = 0;
            int count = 0;

            if (int.TryParse(parameters[0],out desde) &&
            int.TryParse(parameters[1], out count))
            {
                byte[] bytes = new byte[count];
                using (FileStream fs = System.IO.File.OpenRead(Sim900Plugin.FILENAME_PATH))
                {
                    fs.Read(bytes, desde, count);
                    fs.Close();
                }

                response= System.Text.Encoding.ASCII.GetString(bytes);
            }
            response += "\r\nOK\r\n";
            return response;
        }
    }
}
