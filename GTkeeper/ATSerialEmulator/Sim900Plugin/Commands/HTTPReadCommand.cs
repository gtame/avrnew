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


        [Import(typeof(ISim900Emulator))]
        public ISim900Emulator Sim900;


        string ICommand.ProcessCommand(string command)
        {
            string response = string.Empty;
            string [] parameters= command.ParseCommand("AT+HTTPREAD=");

            int desde = 0;
            int count = 0;

            if (int.TryParse(parameters[0],out desde) &&
            int.TryParse(parameters[1], out count))
            {
                response = Sim900.Request.Content.Substring(desde, count);


                //byte[] bytes = new byte[count];
                //using (FileStream fs = System.IO.File.OpenRead(Sim900Plugin.FILENAME_PATH))
                //{
                //    fs.Seek(desde, SeekOrigin.Begin);
                //    fs.Read(bytes, 0, count);
                //    fs.Close();
                //}

                //response= System.Text.Encoding.ASCII.GetString(bytes);
            }

            response = $"+HTTPREAD:{response.Length}\r\n" + response + "\r\nOK\r\n";
            return response;
        }
    }
}
