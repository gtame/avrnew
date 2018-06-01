using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GTKeeperAPI.Models
{
    /*
    //Sumatorio total 127 para todos los dias
    typedef enum {
        SMSNone = 0,//
        SMSInicioSector = (1 << 0),//1
        SMSFinSector = (1 << 1),//2
        SMSProgramacionWeb = (1 << 2),//4
        SMSReset = (1 << 3),//8
        SMSReservado1 = (1 << 4),//16
        SMSReservado2 = (1 << 5),//32
        SMSReservado3 = (1 << 6)//64
    }
    SmsAviso;

   




    //--Numero para enviar avisos
    //--Configuracion APN
    //--UserAPN
    //--PwdAPN
    //--Password para sms
    //--Nivel de envios de sms
    typedef struct  {

    char MovilAviso[15];
    char PasswordSMS[5];
    uint8_t AvisosSMS;
    char Imei[16]; //Imei del modulo GSM - tiene 14 o 15 caracters mas el NULL =16
    char APN[25]; //APN utilizado para conexion internet x modulo GSM
    char userAPN[25]; // Usuario APN (en el caso que lo tenga)
    char pwdAPN[25];// Pwd APN (en el caso que lo tenga)
    bool GSMAvailable; //Modulo GSM disponible
    uint8_t numpuertos; //Numero de puertos (sectores)
    uint8_t numabono;//Numero de bombas  de abono.
    bool motor_diesel;//indicador si hay motor para riego


    time_t lastupdateconfig;//Cuando se cargo por ultima vez la configuracion desde web
    time_t lastupdateprog;//Cuando se cargaron por ultima vez los programas
    time_t lasthora;//Ultima hora fijada

    char flag_check; //Este flag es para comprobar que la configuracion se lee correctamnete, siempre que se lee debe ir a 'X';

}
tConfiguracion_t , tConfiguracion, * tmConfiguracionPtr_t;*/



    //Sumatorio total 127 para todos los dias
    [Flags]
    public enum SmsAviso
    {
        SMSNone = 0,//
        SMSInicioSector = (1 << 0),//1
        SMSFinSector = (1 << 1),//2
        SMSProgramacionWeb = (1 << 2),//4
        SMSReset = (1 << 3),//8
        SMSReservado1 = (1 << 4),//16
        SMSReservado2 = (1 << 5),//32
        SMSReservado3 = (1 << 6)//64
    }

    public class Device
    {

        private Programas _programas=null;
        private Salidas _salidas = null;
        private Logs _logs = null;

        public string Nombre { get; set; } //Nombre del dispositivo

        public string Imei { get; set; } //Imei del modulo GSM - tiene 14 o 15 caracters mas el NULL =16


        #region SMS
        public string MovilAviso { get; set; }

        public string PasswordSMS { get; set; }

        public SmsAviso AvisosSMS { get; set; }
        #endregion 

        public int NumPuertos { get; set; } //Numero de puertos (sectores)

        public int NumAbono { get; set; }//Numero de bombas  de abono.

        public bool MotorDiesel { get; set; }//indicador si hay motor para riego


        public bool IsConfigPendingSync { get; set; }

        

        public DateTime LastUpdateConfig {get;set; }//Cuando se cargo por ultima vez la configuracion desde web

        public bool IsProgramPendingSync { get; set; }

        public DateTime LastUpdateProgram { get; set; }//Cuando se cargo por ultima vez la configuracion desde web


        public DateTime? LastCommitDate { get; set; }//Variable para indicar el Sync

        public DateTime LastSync { get; set; }//Variable para indicar la ultima sincronizacion

        //char flag_check; //Este flag es para comprobar que la configuracion se lee correctamnete, siempre que se lee debe ir a 'X';

        
        /// <summary>
        /// Programas de riego asociados al dispositivo
        /// </summary>
        public IReadOnlyCollection<Programa> Programas
        {
            get
            {
                if (_programas == null)
                    _programas = new Programas();
                return _programas.AsReadOnly();
            }
        }

        //Fija una programacion
        public void SetPrograma(int index,int sector,Dia dias,TimeSpan hora, TimeSpan abono,TimeSpan riego)
        {
            _programas[index].Sector = sector;
            _programas[index].Dias = dias;
            _programas[index].Hora = hora;
            _programas[index].TiempoAbono = abono;
            _programas[index].TiempoRiego = riego;
        }

        #region Salidas

        /// <summary>
        /// Salidas activas en el dispositivo
        /// </summary>
        public IReadOnlyCollection<Salida> Salidas
        {
            get
            {
                if (_salidas == null)
                    _salidas = new Salidas();
                return _salidas.AsReadOnly();
            }
        }
        /*
         
                     public int Programa { get; set; }

        public int Sector { get; set; }

        public TipoSalida Tipo { get; set; }

        public DateTime Desde { get; set; }

        public DateTime Hasta { get; set; }
             */
        public void AddSalida(Salida salida)
        {
            _salidas.Add(salida);
        }


        public void AddSalida(TipoSalida tipo,int? programa,int? sector, System.DateTime desde, System.DateTime? hasta)
        {
            Salida salida = new Salida()
            {
                Tipo = tipo,
                Programa = programa,
                Sector = sector,
                Desde = desde,
                Hasta = hasta
            };
            AddSalida(salida);
        }


        public void ClearSalidas()
        {
            foreach(var salida in Salidas)
                AddLog(LogType.Warning, LogSource.Web, $"Forzado apagado de salida {salida}", System.DateTime.Now);
            _salidas.Clear();
        }

        public void RemoveSalida(Salida salida)
        {
            
            _salidas.Remove(salida);
//            AddLog(LogType.Info, LogSource.Device, $"Apagado de salida {salida}", System.DateTime.Now);
        }



        //public void AddSalida(TipoSalida tipo, System.DateTime desde)
        //{
        //    AddSalida(tipo, null, null, desde, null);
        //}


        //public void AddSalida(TipoSalida tipo,System.DateTime desde)
        //{ 
        //    AddSalida(tipo,null,null,desde,null);
        //}

        #endregion

        #region Logs
        /// <summary>
        /// Logs registrados
        /// </summary>
        public IReadOnlyCollection<Log>  Logs
        {
            get
            {
                if (_logs == null)
                    _logs = new Logs();
                return _logs.AsReadOnly();
            }
        }

        public void AddLog (Log log)
        {
            log.Registrado = System.DateTime.Now;
            _logs.Add(log);
        }

        public void AddLog(LogType type,LogSource source,string message, System.DateTime fecha)
        {
            Log log = new Log()
            {
                Type = type,
                Source = source,
                Mensaje = message,
                Fecha = fecha,
            };
            AddLog(log);
        }

        public void AddLog(string message, System.DateTime fecha)
        {
            AddLog(LogType.Info, LogSource.Web, message, fecha);
        }

        public void AddLog(string message)
        {
            AddLog(LogType.Info, LogSource.Web, message, System.DateTime.Now);
        }

        #endregion


        public void CommitChanges()
        {
            IsConfigPendingSync = false;
            IsProgramPendingSync = false;
            LastCommitDate = System.DateTime.Now;
        }


        enum LineType
        {
            Desconocido,
            Config,
            Programa,
            Salida,
            Log
        };

        public bool ParseFile (string [] lines)
        {

            bool result = true;
            LineType linetype = LineType.Desconocido;

            foreach (string line in lines)
            {
                switch (line)
                {
                    case "+C:":
                        {
                            linetype = LineType.Config;
                        }
                        break;
                    case "+P:":
                        {
                            linetype = LineType.Programa;
                        }
                        break;
                    case "+S:":
                        {
                            linetype = LineType.Salida;
                        }
                        break;
                    case "+L:":
                        {
                            linetype = LineType.Log;
                        }
                        break;
                    default:
                        {
                            switch (linetype)
                            {
                                case LineType.Config:
                                    {
                                        
                                    }
                                    break;
                                case LineType.Salida:
                                    {

                                    }
                                    break;
                                case LineType.Programa:
                                    {

                                    }
                                    break;
                                case LineType.Log:
                                    {

                                    }
                                    break;
                                case LineType.Desconocido:
                                    {
                                        result = false;
                                    }
                                    break;
                            }
                        }
                        break;
                }

            }

            return result;
        }

        public bool IsPendingConfig(string luc)
        {
            if (IsConfigPendingSync)
            {
                if (string.IsNullOrEmpty(luc))
                    return true;
                else
                    return (LastUpdateConfig.Ticks > long.Parse(luc));
            }
            else
                return false;
        }


        public bool IsPendingProgram(string lup)
        {
            if (IsProgramPendingSync)
            {
                if (string.IsNullOrEmpty(lup))
                    return true;
                else
                    return (LastUpdateProgram.Ticks > long.Parse(lup));
            }
            else
                return false;
        }

        /// <summary>
        /// Pasa a cadena para el traspaso de información al dispositivo
        /// </summary>
        /// <returns>
        ///12765331679911111215
        ///3 para AvisosSMS
        ///6 siguientes para movil
        ///NUM 4 password sms
        ///BOOL 1 o 0 para motor diesel
        ///NUM 1 para abono
        ///NUM 2 para puertos
        ///char config[] = "1276533167991111121";
        /// </returns>
        public override string ToString()
        {
            return string.Format("{0:000}{1}{2}{3}{4}{5:00}", (int)AvisosSMS, MovilAviso, PasswordSMS, (MotorDiesel?"1":"0"), NumAbono, NumPuertos);
        }



        public void ParseDevice(string device)
        {

            if (string.IsNullOrEmpty(device))
                throw new Exception("La cadena esta vacía");

            if (device.Length != 19)
                throw new Exception($"La longitud de la cadena es incorrecta (17) vs ({device.Length})");

            if (!device.All(char.IsDigit))
                throw new Exception("La cadena no puede contener caracteres,solo puede contener digitos");

            AvisosSMS = (SmsAviso)int.Parse(device.Substring(0, 3));
            MovilAviso = device.Substring(3, 6);
            PasswordSMS = device.Substring(9, 4);
            MotorDiesel = (device.Substring(13, 1) == "1");
            NumAbono = int.Parse(device.Substring(14, 1));
            NumPuertos = int.Parse(device.Substring(15));
            
            if ((int)AvisosSMS != int.Parse(device.Substring(0, 3)))
                throw new Exception("No coinciden los avisos SMS");
            
        }

    }
    
    public class Devices : List<Device>
    {
        public Device this [string imei]
        {
            get
            {
                return this.Find(t => t.Imei == imei);
            }
        }
    }
}
