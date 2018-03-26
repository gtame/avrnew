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
    [ExportMetadata("Pattern", "AT+HTTPACTION")]
    class HTTPACTIONCommand:ICommand
    {

        [Import(typeof(IATEmulator))]
        public IATEmulator ATapp;

         void SendFile()
        {
            System.Threading.Thread.Sleep(new Random().Next(0,5) *500);
            
            long filesize=  new System.IO.FileInfo(Sim900Plugin.FILENAME_PATH).Length;
            ATapp.SendData($"+HTTPACTION:0,200,{filesize}\r\n");
        }

        string ICommand.ProcessCommand(string command)
        {
 

            //Lo enviamos async
            Task.Run(() => SendFile());

            return "OK\r\n";
        }
    }
}
