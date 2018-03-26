using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using ATSerialEmulator.Plugins;
using System.Reflection;

namespace ATSerialEmulator
{

    class Program
    {

        private static CompositionContainer _container;




        private Program()
        {

            //An aggregate catalog that combines multiple catalogs
            var catalog = new AggregateCatalog();
            //Adds all the parts found in the same assembly as the Program class
            catalog.Catalogs.Add(new AssemblyCatalog(typeof(Program).Assembly));

            string pluginsdir = System.IO.Path.GetDirectoryName(Assembly.GetEntryAssembly().Location); ;
            if (System.IO.Directory.Exists(pluginsdir))
                catalog.Catalogs.Add(new DirectoryCatalog(pluginsdir));


            //Create the CompositionContainer with the parts in the catalog
             _container = new CompositionContainer(catalog);

    
        }



        /// <summary>
        /// Punto de entrada principal para la aplicación.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {

            
            //Composition is performed in the constructor
            Program p = new Program();

            //Parse arguments
            ArgumentParser.Parser(args);


            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            frmMain main = new frmMain();
            //Fill the imports of this object
            try
            {
                _container.ComposeParts(main);
            }
            catch (CompositionException compositionException)
            {
                Console.WriteLine(compositionException.ToString());
            }


            //Launch form
            Application.Run(main);
        }
    }
}
