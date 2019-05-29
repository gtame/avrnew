using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GTKeeperAPI.Models
{


    public enum TipoSalida
    {
        Programa = 1,
        Sector = 2,
        Abono = 3,
        Motor = 4,
        Limpieza = 5
    }



    public class Salidas : List<Salida>
    {

    }

    public class Salida
    {
        public int SalidaID { get; set; }

        public int? Programa { get; set; }

        public int? Sector { get; set; }

        public TipoSalida Tipo { get; set; }

        public DateTime Desde { get; set; }

        public DateTime? Hasta { get; set; }


    /*

      	tSalida* stat=&salidas[salidaIndex];
 	sprintf_P(text,
	PSTR("%02i%02i%10lu%10lu%02i"),
	stat->Tipo,
	stat->Ident,
	stat->Desde,
	stat->Hasta,
	stat->Sector);
       */
    public static Salida ParseSalida(string salida)
        {
                if (string.IsNullOrEmpty(salida))
                  throw new Exception("La cadena esta vacía");

                if (salida.Length != 26)
                  throw new Exception($"La longitud de la cadena es incorrecta (26) vs ({salida.Length})");

                if (!salida.All(c => char.IsDigit(c) || char.IsWhiteSpace(c)))
                  throw new Exception("La cadena no puede contener caracteres,solo puede contener digitos");

      
                Salida salidaActiva = new Salida()
                {
                  Tipo =(TipoSalida) int.Parse(salida.Substring(0, 2)),
                  Programa = (string.IsNullOrEmpty(salida.Substring(2, 2)) ? (int?)null : int.Parse(salida.Substring(2, 2))),
                  Desde = new DateTime(int.Parse(salida.Substring(4, 10))),
                  Hasta = new DateTime(int.Parse(salida.Substring(14, 10))),
                  Sector = (string.IsNullOrEmpty(salida.Substring(24, 2)) ? (int?)null : int.Parse(salida.Substring(24, 2)))  
                };
      
                  

                return salidaActiva;

        }


        public Device Device { get; set; }

    }
}
