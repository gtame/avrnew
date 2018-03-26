using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace ATSerialEmulator
{
    /// <summary>
    /// Parser arguments ESPGUI
    /// -S SERVER
    /// -U USER
    /// -P PASSWORD
    /// -T TRANSACTION
    /// -H HELP
    /// -url URL para conexion
    /// -type Tipo de conexion
    /// </summary>
    public static class ArgumentParser
    {
        public const string PORT_OPTION = "-p";
        public const string BAUDRATE_OPTION = "-b";
        public const string DICTIONARY_OPTION = "-d";
         public const string NO_CR_OPTION = "--nocr";
        public const string NO_LF_OPTION = "--nolf";
        public const string OPEN_OPTION = "--o";
        public const string HELP_OPTION = "--help";
        public const string HELP_OPTION2 = "-help";
        public const string HELP_OPTION3 = "/?";


        ////private static string s_system = null;
        ////private static string s_user = null;
        ////private static string s_password = null;
        ////private static string s_transaction= null;

        public static string Port = null;

        public static string BaudRate = null;

        public static string Dictionary = null;

        public static bool CR = true;

        public static bool LF = true;

        public static bool Open = false;


        private static void ShowHelp()
        {
            Console.WriteLine("Use -p PORTNAME - COM1");
            Console.WriteLine("Use -b BAUDRATE - 9600");
            Console.WriteLine("Use -d DICTIONARYFILE - \"C:\\dictionary.txt\"");
            Console.WriteLine("Use --nocr - No add suffix <CR> ");
            Console.WriteLine("Use --nocr - No add suffix <LF> ");
            Console.WriteLine("Use --o - Open connection automatically ");
        }
        private static void ShowError(string arg)
        {
            Console.WriteLine("Use --help command.");
        }

        public static void Parser(string[] args)
        {
            bool boption = true;
            string s_option = null;

            foreach (string arg in args)
            {
                if (boption)
                {
                    switch (arg)
                    {
                        case ArgumentParser.PORT_OPTION:
                            {
                                s_option = arg;
                                boption = false;
                            }
                            break;
                        case ArgumentParser.BAUDRATE_OPTION:
                            {
                                s_option = arg;
                                boption = false;
                            }
                            break;
                        case ArgumentParser.DICTIONARY_OPTION:
                            {

                                s_option = arg;
                              
                                boption = false;
                            }
                            break;
                        case ArgumentParser.NO_CR_OPTION:
                            {
                                CR = false;
                                boption = true;
                            }
                            break;
                        case ArgumentParser.NO_LF_OPTION:
                            {
                                LF = false;;
                                boption = true;
                            }
                            break;
                        case ArgumentParser.OPEN_OPTION:
                            {
                                Open = true;
                                boption = false;
                            }
                            break;
                        case ArgumentParser.HELP_OPTION:
                        case ArgumentParser.HELP_OPTION2:
                        case ArgumentParser.HELP_OPTION3:
                            ShowHelp();
                            break;
                        default:
                            ShowError(arg);
                            break;
                    }
                }
                else
                {
                    //Value 
                    switch (s_option)
                    {
                        case ArgumentParser.PORT_OPTION:
                            {
                                Port = arg;
                            }
                            break;
                        case ArgumentParser.BAUDRATE_OPTION:
                            {
                                BaudRate = arg;
                            }
                            break;
                        case ArgumentParser.DICTIONARY_OPTION:
                            {

                                if (!System.IO.Path.IsPathRooted(arg))
                                {
                                    Dictionary = System.IO.Path.Combine(System.IO.Path.GetDirectoryName(Assembly.GetEntryAssembly().Location), arg);
                                }
                                else
                                 Dictionary = arg;
                            }
                            break;
                      
                        case ArgumentParser.HELP_OPTION:
                            break;
                        default:
                            break;
                    }
                    s_option = null;
                    boption = true;
                }

            }
        }

    }
}
