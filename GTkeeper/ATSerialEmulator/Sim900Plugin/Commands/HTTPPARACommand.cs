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

        [Import(typeof(ISim900Emulator))]
        public ISim900Emulator Sim900;


        string ICommand.ProcessCommand(string command)
        {
            string response = "OK\r\n";
            string[] parser = command.ParseCommand("AT+HTTPPARA=");

           
            if (parser[0] == "\"URL\"")
            {
                string filepath = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetEntryAssembly().Location);

                string filename = parser[1];
                if (filename.StartsWith("\"http://"))
                {
                    //TO-DO
                    Sim900HttpRequest request = new Sim900HttpRequest()
                    {
                        Resource = filename.Replace("\"", string.Empty)
                    };

                    Sim900.Request = request;
                }
                else if (filename.StartsWith("\"file://"))
                {

                    filename = filename.Substring(8, filename.Length - 9);
                    Sim900Request request = new Sim900Request()
                    {
                        Resource = filename,
                        Content = System.IO.File.ReadAllText(filename)
                    };

                    Sim900.Request = request;
                }

                
            }
            else if (parser[0]=="\"CONTENT\"")
            {
                if (Sim900.Request == null)
                    throw new System.Exception("The url request is not opened");
                if ((Sim900.Request is Sim900HttpRequest))
                {
                 
                    ((Sim900HttpRequest)Sim900.Request).ContentType = parser[1].Replace("\"",string.Empty);
                }

            }

            return response;
        }
    }
}
