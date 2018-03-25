//using System;
//using System.Collections.Generic;
//using System.Linq;
//using System.Text;
//using System.Threading.Tasks;
//using System.ComponentModel.Composition;
//using System.ComponentModel.Composition.Hosting;

//namespace ATSerialEmulator.Plugins
//{
     
//    [Export(typeof(IATPlugin))]
//    //[ExportMetadata("Symbol", '-')]
//    class PluginExample : IATPlugin
//    {
//        [Import(typeof(IATEmulator))]
//        public IATEmulator ATapp;

//        public PluginExample()
//        {
//        }

//        string IATPlugin.ProcessCommand(ref bool handled, string command)
//        {
//            return null;
//        }



//    }


//}
