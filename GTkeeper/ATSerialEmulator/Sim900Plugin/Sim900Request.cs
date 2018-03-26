using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sim900Plugin
{

    //Representa una clase para hacer la request
    class Sim900Request
    {
        //El nombre del fichero o URL
        public string Resource { get; set; }

        //El contenido del archivo o del resultado de la solicitud
        public string Content { get; set; }


    }


    class Sim900HttpRequest:Sim900Request
    {
        public string ContentType { get; set; }

        public int Length { get; set; }

        //DAtos enviados en la peticion post
        public string Data { get; set; }


        [DefaultValue(false)]
        //DAtos enviados en la peticion post
        public bool  IsSendingData { get; set; }

    }

}
