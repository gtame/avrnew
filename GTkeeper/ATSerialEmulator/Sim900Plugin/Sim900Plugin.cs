using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using ATSerialEmulator.Plugins;
using Sim900Plugin.Commands;

namespace Sim900Plugin
{

    [Export(typeof(IATPlugin))]
    //[ExportMetadata("Symbol", '-')]
    class Sim900Plugin : IATPlugin
    {

        public const string FILENAME_PATH = "file.txt";

        [Import(typeof(IATEmulator))]
        public IATEmulator ATapp;


        [ImportMany]
        IEnumerable<Lazy<ICommand, ICommandPattern>> commands;


        public Sim900Plugin()
        {
        }

        string IATPlugin.ProcessCommand(ref bool handled, string command)
        {
 
            if(!string.IsNullOrEmpty(command))
            {
                foreach (Lazy<ICommand, ICommandPattern> i in commands)
                {
                    if (command.StartsWith(i.Metadata.Pattern))
                    {
                        handled = true;
                        return i.Value.ProcessCommand(command);
                    }
                }

            }


            return null;
        }



    }


}
