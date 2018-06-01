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
        
        public int? Programa { get; set; }

        public int? Sector { get; set; }

        public TipoSalida Tipo { get; set; }

        public DateTime Desde { get; set; }

        public DateTime? Hasta { get; set; }


        public static Salida ParseSalida(string salida)
        {
            return null;
        }

    }
}
