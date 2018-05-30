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
    [ExportMetadata("Pattern", "AT+HTTPDATA")]
    class HTTPDATACommand : ICommand
    {
        [Import(typeof(ISim900Emulator))]
        public ISim900Emulator Sim900;


        [Import(typeof(IATEmulator))]
        public IATEmulator ATapp;

        string ICommand.ProcessCommand(string command)
        {
            string response = string.Empty;
            if (Sim900.Request == null)
                throw new System.Exception("The  request is not opened");

            if ((Sim900.Request is Sim900HttpRequest))
            {
                string [] parser=command.ParseCommand("AT+HTTPDATA=");
                ((Sim900HttpRequest)Sim900.Request).Length = int.Parse(parser[0]);
                ((Sim900HttpRequest)Sim900.Request).IsSendingData = true;


                ATapp.ProcessMessages = false;

                byte[] bytetext = System.Text.ASCIIEncoding.ASCII.GetBytes("DOWNLOAD\r\n");
                ATapp.BaseStream.Write(bytetext, 0, bytetext.Length);



                int total = ((Sim900HttpRequest)Sim900.Request).Length ;
                    int contador = 0;
                    byte[] bytes = new byte[total];
                    while (contador < total)
                    {
                        try
                        {
                            byte byteReaded = (byte)ATapp.BaseStream.ReadByte();
                            if (byteReaded != -1)
                            {
                                bytes[contador] = byteReaded;
                                contador++;
                            }
                        }
                        catch (System.TimeoutException ex)
                        {
                            
                        }
                    }

                     

        
                
                ((Sim900HttpRequest)Sim900.Request).IsSendingData = false;
                ATapp.ProcessMessages = true;

                ((Sim900HttpRequest)Sim900.Request).Data = System.Text.ASCIIEncoding.ASCII.GetString(bytes);





            }
            else
               response = "DOWNLOAD\r\nOK\r\n";


            return response;
        }
    }
}
