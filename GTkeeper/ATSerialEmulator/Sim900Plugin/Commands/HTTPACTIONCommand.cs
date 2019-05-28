using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using ATSerialEmulator.Plugins;
using System.Net;
using System.IO;

namespace Sim900Plugin.Commands
{
    [Export(typeof(ICommand))]
    [ExportMetadata("Pattern", "AT+HTTPACTION")]
    class HTTPACTIONCommand:ICommand
    {

        [Import(typeof(IATEmulator))]
        public IATEmulator ATapp;

        [Import(typeof(ISim900Emulator))]
        public ISim900Emulator Sim900;


        void GetRequest()
        {

            using (var wb = new WebClient())
            {
                Sim900.Request.Content = wb.DownloadString(Sim900.Request.Resource);
            }

            ATapp.SendData($"+HTTPACTION:0,200,{Sim900.Request.Content.Length}\r\n");
        }

        void PostRequest()
        {

             

            var request = (HttpWebRequest)WebRequest.Create(Sim900.Request.Resource);
 

            request.Method = "POST";

            request.ContentType = ((Sim900HttpRequest)Sim900.Request).ContentType;


            
            var data = Encoding.ASCII.GetBytes(((Sim900HttpRequest)Sim900.Request).Data);
            request.ContentLength = data.Length;
            using (var stream = request.GetRequestStream())
            {
                stream.Write(data, 0, data.Length);
            }

            var response = (HttpWebResponse)request.GetResponse();

            ((Sim900HttpRequest)Sim900.Request).Content = new StreamReader(response.GetResponseStream()).ReadToEnd();


            if (response.StatusCode== HttpStatusCode.OK)
                ATapp.SendData($"+HTTPACTION:0,200,{Sim900.Request.Content.Length}\r\n");
        }

         void SendFile()
        {
            System.Threading.Thread.Sleep(new Random().Next(0,5) *500);

            ATapp.SendData($"+HTTPACTION:0,200,{Sim900.Request.Content.Length}\r\n");
        }

        string ICommand.ProcessCommand(string command)
        {

            if (Sim900.Request == null)
                throw new System.Exception("The  request is not opened");

            if (Sim900.Request is Sim900HttpRequest)
            {
                string [] parse=command.ParseCommand("AT+HTTPACTION=");


               ((Sim900HttpRequest)Sim900.Request).IsSendingData = false;

                if (parse[0]=="0")
                    Task.Run(() => GetRequest());
                else if(parse[0] == "1")
                    Task.Run(() => PostRequest());
                else
                    throw new System.Exception("HTTPACTION not supported method");
            }
            else
                //Lo enviamos async
                Task.Run(() => SendFile());

            return "OK\r\n";
        }
    }
}
