using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GTKeeperAPI.Models
{

     public enum LogType
    {
        Critical,
        Error,
        Info,
        Warning
    }

    public enum LogSource
    {
        Web,
        Device
    }

    public class Logs : List<Log>
    {

    }

    public class Log
    {
        #region Properties
        public int LogID { get; set; }
        public LogType Type { get; set; }
        public string Mensaje { get; set; }
        public System.DateTime Fecha { get; set; }
        public System.DateTime Registrado { get; set; }
        public LogSource Source { get; set; }
        #endregion

        public Device  Device { get; set; }


    }
}
