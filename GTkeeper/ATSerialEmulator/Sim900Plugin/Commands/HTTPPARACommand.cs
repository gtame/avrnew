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
                string filepath = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetEntryAssembly().Location);

                string filename = parser[1];
                if (filename.StartsWith(@"http://"))
                {
                    //TO-DO

                }
                else if (filename.StartsWith("\"file://"))
                {
                    filename = filename.Substring(8, filename.Length-9);
                    filepath = System.IO.Path.Combine(filepath, Sim900Plugin.FILENAME_PATH);
                    System.IO.File.Copy(filename, filepath,true);
                }

                
            }

            return response;
        }
    }
}
