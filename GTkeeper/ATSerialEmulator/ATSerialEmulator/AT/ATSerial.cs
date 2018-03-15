using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ATSerialEmulator
{
    class ATSerial:IDisposable
    {
        private SerialPort _serial = null;
        private CancellationTokenSource cts;
        private Dictionary<string, string> responseDict = new Dictionary<string, string>();
        private List<Command> commands = new List<Command>();
        
        private const int TIMEOUT = 500;

        enum TypeCommand
        {
            Command,
            CommandBytes,
            File
        }

        class CommandByte:Command
        {
            public CommandByte(byte [] bytes):base( TypeCommand.CommandBytes,"")
            {
                Bytes = bytes;
            }

            public byte []  Bytes { get; set; }
        }

        class Command
        {
            public Command(TypeCommand type,string command)
            {
                Type = type;
                CommandString = command;
            }

            public   TypeCommand Type { get;set; }
            public string CommandString { get; set; }
        }

        private ILog _logger = null;
        protected ILog Logger
        {
            get
            {
                if (_logger==null)
                    _logger= LogManager.GetLogger(this.GetType());
                return _logger;
            }
        }


        [DefaultValue(false)]
        public bool IsOpened
        {
            get; protected set;
        }


        /// <summary>
        /// Command - Responses in dictioanry
        /// </summary>
        public void ClearDictionary()
        {
            responseDict.Clear();
        }

        /// <summary>
        /// Command - Responses in dictioanry
        /// </summary>
        public IReadOnlyDictionary<string,string> CommandDictionary
        {
            get
            {
                return responseDict;
            }

        }


        /// <summary>
        /// Open serial port with default properies
        /// </summary>
        /// <param name="serialport"></param>
        public ATSerial(string serialport)
        {
            _serial = new SerialPort(serialport);
            
        }


        /// <summary>
        /// Open serial port with the specified baudrate & default properies 
        /// </summary>
        public ATSerial(string serialport, int baudrate)
        {

            _serial = new SerialPort(serialport)
            {
                ReadTimeout = TIMEOUT,
                WriteTimeout = SerialPort.InfiniteTimeout,
                BaudRate = baudrate,
            };
            
        }

        /// <summary>
        /// Open serial port with the specified  properies 
        /// </summary>
        public ATSerial(string serialport,int baudrate,int databits, System.IO.Ports.Parity parity, System.IO.Ports.StopBits stopbits, System.IO.Ports.Handshake handshake)
        {
            _serial = new SerialPort(serialport)
            {
                ReadTimeout = TIMEOUT,
                WriteTimeout = SerialPort.InfiniteTimeout,
                BaudRate = baudrate,
                DataBits=databits,
                Parity=parity,
                StopBits=stopbits,
                Handshake=handshake
            };
        }


        public void AddResponse(string request,string response)
        {
            if (!responseDict.ContainsKey(request))
                responseDict.Add(request, response);
        }
        
        private string ParseCommand(string command)
        {
            if (!string.IsNullOrEmpty(command))
            {
                return command.Replace("<CR>", "\r").Replace("<LF>", "\n");
            }
            else
                return command;
        }

        /// <summary>
        /// Async Task for serial port IO operations
        /// </summary>
        /// <param name="ct"></param>
        /// <returns></returns>
        private async Task SerialProcessAsync(CancellationToken ct)
        {
            
            while (!cts.IsCancellationRequested)
            {
                try
                {
                    //Commands
                    foreach (Command command in commands)
                    {
                        if (command.Type == TypeCommand.Command)
                        {
                            Logger.Info("=== SEND  COMMAND ===");
                            Logger.Info($"<<{command.CommandString}");
                            _serial.Write(ParseCommand(command.CommandString));
                            Logger.Info("=== END SEND  COMMAND ===");

                        }
                        else if  (command.Type == TypeCommand.File)
                        {
                            Logger.Info($"=== SEND  FILE {command.CommandString} ===");
                            Logger.Info(System.IO.File.ReadAllText(command.CommandString));
                            _serial.Write(System.IO.File.ReadAllText(command.CommandString));
                            Logger.Info($"=== END SEND  FILE {command.CommandString} ===");
                        }
                        else if (command.Type == TypeCommand.CommandBytes)
                        {
                            byte[] bytes = ((CommandByte)command).Bytes;
                            Logger.Info($"=== SEND  BYTES {bytes.Length} ===");
                            Logger.Info(System.Text.UTF8Encoding.Unicode.GetString(bytes));
                            _serial.Write(bytes, 0, bytes.Length);
                            Logger.Info($"=== END SEND  BYTES ===");
                        }
                    }
                    //Clear Commands
                    commands.Clear();

                    //Read serial port
                    string readed = _serial.ReadLine();
                    Logger.Info($">>{readed}");

                    //Dictionary responses
                    if (responseDict.ContainsKey(readed))
                    {
                        string response = responseDict[readed];
                        Logger.Info($"<<{response}");
                        _serial.WriteLine(response);
                    }
                    else
                    {
                        //Like option
                        foreach(var response in responseDict)
                        {
                            if (response.Key.Like(readed))
                            { 
                                Logger.Info($"<<{response.Value}");
                                _serial.WriteLine(ParseCommand(response.Value));
                                break;
                            }
                        }
                    }

                   
                }
                catch (System.TimeoutException timeoutex)
                {
                    //No readed
                    Logger.Debug($"Timeout reading serial port {_serial.PortName}..");
                }
                catch (System.Exception ex)
                {
                    Logger.Error($"IO-Error {_serial.PortName}", ex);
                }

            }
             //end task
        }


        /// <summary>
        /// Open serial port
        /// </summary>
        public  void Open()
        {
            if (_serial.IsOpen)
                throw new System.Exception($"The port {_serial.PortName} is in use." );


            _serial.NewLine = "\r\n";
            _serial.Open();
            _serial.DiscardInBuffer();
            _serial.DiscardOutBuffer();
            IsOpened = true;

            cts = new CancellationTokenSource();
            Task.Factory.StartNew(() => SerialProcessAsync(cts.Token));
 
        }




        /// <summary>
        /// Add HEX command
        /// </summary>
        /// <param name="bytes"></param>
        public void AddCommand(byte [] bytes)
        {
            commands.Add(new CommandByte(bytes));
        }


        /// <summary>
        /// Add Command
        /// </summary>
        /// <param name="command"></param>
        public void AddCommand(string command)
        {
            commands.Add(new Command ( TypeCommand.Command, command));
        }


        /// <summary>
        /// Send raw file
        /// </summary>
        /// <param name="filePath"></param>
        public void SendFile (string filePath)
        {
            commands.Add(new Command(TypeCommand.File, filePath));
        }

        /// <summary>
        /// Close serial port
        /// </summary>
        public void Close()
        {
            if (cts != null)
            {
                cts.Cancel();
            }

            if ( _serial.IsOpen)
            {
                _serial.Close();

            }
        }

        #region IDisposable Support
        private bool disposedValue = false; // Para detectar llamadas redundantes

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: elimine el estado administrado (objetos administrados).
                    Close();


                    if (_serial!=null)
                        _serial.Dispose();

                }

                // TODO: libere los recursos no administrados (objetos no administrados) y reemplace el siguiente finalizador.
                // TODO: configure los campos grandes en nulos.

                disposedValue = true;
            }
        }

        // TODO: reemplace un finalizador solo si el anterior Dispose(bool disposing) tiene código para liberar los recursos no administrados.
        // ~ATSerial() {
        //   // No cambie este código. Coloque el código de limpieza en el anterior Dispose(colocación de bool).
        //   Dispose(false);
        // }

        // Este código se agrega para implementar correctamente el patrón descartable.
        public void Dispose()
        {
            // No cambie este código. Coloque el código de limpieza en el anterior Dispose(colocación de bool).
            Dispose(true);
            // TODO: quite la marca de comentario de la siguiente línea si el finalizador se ha reemplazado antes.
            // GC.SuppressFinalize(this);
        }
        #endregion



    }
}
