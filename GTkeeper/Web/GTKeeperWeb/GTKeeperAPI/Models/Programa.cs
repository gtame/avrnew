using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GTKeeperAPI.Models
{


    /*




//SSDDDHHMMRRRRAAAA
//SS   -> Sector
//DDD  -> Dias que se ejecutara (enum)
//HHMM -> Hora inicio ó Ejecución en el otro bloque
//RRRR -> Tiempo de riego
//AAAA -> Tiempo de abono

//Ej, 03101010120001200000
//03->Sector
//0101010-> Dias que se ejecutara (Martes,Jueves,Sabado)
//1200 -> Se ejecutara a las 12:00
//0120 -> Tiempo de riego
//0000 -> Tiempo de abono
typedef struct  {

  uint8_t Sector; //Sector
  uint8_t Dias; //Dias en los que se debe ejecutar
  uint8_t HoraInicio;//Cuando debe ejecutarse , tiene dos variantes
  uint8_t MinutoInicio;//88:XX --> Indicara que se lanze cuando termine el programa XX
  	  	  	  	  	   //HH:MM --> Indicara la hora a la que se lanzara si el dia esta marcado
  uint16_t TiempoRiego; //Tiempo de riego , en minutos
  uint16_t TiempoAbono; //Tiempo de abono,  en minutos
  
  bool Motor;

} 	tPrograma_t, tPrograma, *tmProgramaPtr_t;


///Definimos el peso para que la ordenacion tenga efecto
typedef enum {
	actNone=10,//No hay salida activa
	actPrograma=1,
	actSector=2,
	actAbono=3,
	actMotor=4,
	actLimpieza=5
} TipoSalida;


typedef struct {

  uint8_t Ident; //Sector(salida) ó Programa que lanzo
  TipoSalida Tipo; //Tipo, si es salida o
  uint8_t Sector; //Este campo es para asegurar cual era el sector cuando el programa se lanzo para evitar modificaciones entre que salta y se para
  time_t Desde; //Indica cuando se activo la salida ;)
  time_t Hasta;//Indica hasta cuando va a estar 'encendido el programa', si esta parado valor 0;

}  tSalida, *tmSalidasPtr_t;

    //Struct pra almacenar las estadisticas de riego
    typedef struct
    {

    uint8_t sector;
    time_t tiempo_riego;
    time_t tiempo_abono;

}
tEstadistica;


        

*/

    //Sumatorio total 127 para todos los dias
    [Flags]
    public enum Dia
    {
        None=0,
        Lunes=1,
        Martes=2,
        Miercoles=4,
        Jueves=8,
        Viernes=16,
        Sabado=32,
        Domingo=64
    }



    public class Programa
    {

        public static Dia GetDia(DayOfWeek day)
        {
            switch (day)
            {
                case DayOfWeek.Monday:
                    return Dia.Lunes;
                case DayOfWeek.Tuesday:
                    return Dia.Martes;
                case DayOfWeek.Wednesday:
                    return Dia.Miercoles;
                case DayOfWeek.Thursday:
                    return Dia.Jueves;
                case DayOfWeek.Friday:
                    return Dia.Viernes;
                case DayOfWeek.Saturday:
                    return Dia.Sabado;
                case DayOfWeek.Sunday:
                    return Dia.Domingo;
            }

            return Dia.None;

        }

        

        public int Sector { get; set; }

        public Dia Dias { get; set; }

        public TimeSpan Hora { get; set; }

        public TimeSpan TiempoRiego { get; set; }

        public TimeSpan TiempoAbono { get; set; }


        //03101010120001200000
        //03->Sector
        //127-> Dias * Dias que se ejecutara (Martes,Jueves,Sabado)
        //1200 -> Ejecucion * Hora que se ejecutara a las 12:00
        //0120 -> Tiempo de riego
        //0000 -> Tiempo de abono
        public override string ToString()
        {
            return string.Format("{0:00}{1:000}{2:00}{3:00}{4:0000}{5:0000}", Sector, (int)Dias, Hora.Hours, Hora.Minutes, TiempoRiego.TotalMinutes, TiempoAbono.TotalMinutes);
        }

        //03101010120001200000
        //03->Sector
        //127-> Dias * Dias que se ejecutara (Martes,Jueves,Sabado)
        //1200 -> Ejecucion * Hora que se ejecutara a las 12:00
        //0120 -> Tiempo de riego
        //0000 -> Tiempo de abono
        public static Programa ParsePrograma (string programa)
        {
            if (string.IsNullOrEmpty(programa))
                throw new Exception("La cadena esta vacía");

            if (programa.Length != 17)
                throw new Exception($"La longitud de la cadena es incorrecta (17) vs ({programa.Length})");

            if (!programa.All(char.IsDigit))
                throw new Exception("La cadena no puede contener caracteres,solo puede contener digitos");

            
            Programa prog = new Programa()
            {
                Sector = int.Parse(programa.Substring(0, 2)),
                Dias = (Dia) int.Parse(programa.Substring(2, 3)),
                Hora = TimeSpan.FromMinutes(int.Parse(programa.Substring(5, 2))*60 +  int.Parse(programa.Substring(7, 2))),
                TiempoRiego =TimeSpan.FromMinutes(  int.Parse(programa.Substring(9, 4))),
                TiempoAbono = TimeSpan.FromMinutes( int.Parse(programa.Substring(13)))
            };

            if ((int)prog.Dias != int.Parse(programa.Substring(2, 3)))
                throw new Exception("No coinciden los dias");

            return prog;
        }


    }


    public class Programas:List<Programa>
    {

    }
}
