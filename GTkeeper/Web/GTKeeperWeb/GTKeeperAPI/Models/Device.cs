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


        public DateTime LastUpdateConfig {get;set; }//Cuando se cargo por ultima vez la configuracion desde web

        public DateTime LastUpdateProgram { get; set; }//Cuando se cargo por ultima vez la configuracion desde web

        public DateTime LastHora { get; set; }//Variable para indicar la hora del dispositivo

        //char flag_check; //Este flag es para comprobar que la configuracion se lee correctamnete, siempre que se lee debe ir a 'X';

        
        /// <summary>
        /// Programas de riego asociados al dispositivo
        /// </summary>
        public Programas Programas
        {
            get
            {
                if (_programas == null)
                    _programas = new Programas();
                return _programas;
            }
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
        ///char config[] = "12765331679911111215";
        /// </returns>
        public override string ToString()
        {
            return string.Format("{0:000}{1}{2}{3}{4}{5:00}", (int)AvisosSMS, MovilAviso, PasswordSMS, (MotorDiesel?"1":"0"), NumAbono, NumPuertos);
        }
    }
    
    public class Devices : List<Device> { }
}
