using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ATSerialEmulator 
{
    public interface ILog
    {
        void Debug(string msg);
        void Debug(string msg, System.Exception ex);
        void Info(string msg);
        void Info(string msg, System.Exception ex);
        void Warning(string msg);
        void Warning(string msg, System.Exception ex);
        void Error(string msg);
        void Error(string msg, System.Exception ex);
        void Fatal(string msg);
        void Fatal(string msg, System.Exception ex);
    }


    public interface ILogWriter
    {

        void Write(string message);

        /// <summary>
        /// {L} level
        /// {t} Thread
        /// {T} Type
        /// {M} Message
        /// </summary>
        string Format { get; set; }
    }


    public interface ILogWriterManager
    {

        void Write(LogLevel level, int threadId, System.Type type, string message);

        /// <summary>
        /// {L} level
        /// {t} Thread
        /// {T} Type
        /// {M} Message
        /// </summary>
        string Format { get; set; }
    }

    public class ConsoleWriter : ILogWriter
    {
        public void Write(string message)
        {
            System.Console.WriteLine(message);
        }

        public string Format { get; set; }
    }

    public class FileWriter : ILogWriter, IDisposable
    {
        private System.IO.FileStream fileStream;
        private static object _lock = new object();


        public string Format { get; set; }


        public FileWriter(string filename)
        {
            fileStream = new System.IO.FileStream(filename, System.IO.FileMode.Create);

        }

        public void Write(string message)
        {
            lock (_lock)
            {
                byte[] bytes = System.Text.UTF8Encoding.UTF8.GetBytes(message + "\r\n");
                fileStream.Write(bytes, 0, bytes.Length);
                fileStream.Flush();
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
                    fileStream.Flush();
                    fileStream.Close();
                    fileStream.Dispose();
                }

                // TODO: libere los recursos no administrados (objetos no administrados) y reemplace el siguiente finalizador.
                // TODO: configure los campos grandes en nulos.

                disposedValue = true;
            }
        }

        // TODO: reemplace un finalizador solo si el anterior Dispose(bool disposing) tiene código para liberar los recursos no administrados.
        // ~FileWriter() {
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

    public class ControlWriter : ILogWriter
    {
        private System.Windows.Forms.TextBox textBox;

        public ControlWriter(System.Windows.Forms.TextBox txt,string format)
        {
            textBox = txt;
            Format = format;
        }
        public string Format { get; set; }

        private void PutText(string message)
        {

            textBox.Text += message + System.Environment.NewLine;
            textBox.SelectionStart = textBox.Text.Length;
            textBox.ScrollToCaret();
        }

        public void Write(string message)
        {
            if (textBox.InvokeRequired)
                textBox.Invoke(new System.Windows.Forms.MethodInvoker(delegate
                {
                    PutText(message);
                }));
            else
                PutText(message);
        }



    }


    public class WriterManager : ILogWriterManager
    {
        public List<ILogWriter> writers = new List<ILogWriter>();

        public string Format { get; set; }

        public WriterManager ()
        {
            Format = "{L} ({t}): - {T} - {M}";
        }

        public void AppendWriter(ILogWriter logwriter)
        {
            writers.Add(logwriter);
        }

        public void Write(LogLevel level, int threadId, System.Type type, string message)
        {
            foreach (var writer in writers)
            {
                try
                {
                    string format = writer.Format;
                    if (string.IsNullOrEmpty(format))
                        format = Format;


                  format=  format.Replace("{L}", level.ToString())
                        .Replace("{t}",System.Threading.Thread.CurrentThread.ManagedThreadId.ToString())
                        .Replace("{T}",type.ToString())
                        .Replace("{M}", message);

                    writer.Write(format);

                }
                catch (System.Exception ex)
                {
#if DEBUG
                    throw ex;
#endif
                }
            }

        }
    }

  

    public class FakeLogger : ILog
    {
        private Type typeReference = null;
        private ILogWriterManager logWriter = null;


        public FakeLogger(System.Type type, ILogWriterManager writer)
        {
            typeReference = type;
            logWriter = writer;
        }

        protected string ToString(System.Exception ex)
        {
            string result = null;
            if (ex != null)
            {
                result = "============= EXCEPTION ==============\r\n";
                result += ex.ToString() + "\r\n";
                result += "============= ENDEXCEPTION ==============\r\n";
            }
            return result;
        }

        #region ILog
        public void Debug(string msg)
        {
            if (LogManager.GetLevel()<=LogLevel.Debug)
            logWriter.Write(LogLevel.Debug, System.Threading.Thread.CurrentThread.ManagedThreadId, typeReference, msg);
        }

        public void Debug(string msg, Exception ex)
        {
            if (LogManager.GetLevel() <= LogLevel.Debug)
            {
                msg += ToString(ex);
                logWriter.Write( LogLevel.Info, System.Threading.Thread.CurrentThread.ManagedThreadId, typeReference, msg);
            }
        }

        public void Error(string msg)
        {
            if (LogManager.GetLevel() <= LogLevel.Error)
                logWriter.Write(LogLevel.Error, System.Threading.Thread.CurrentThread.ManagedThreadId, typeReference, msg);
        }

        public void Error(string msg, Exception ex)
        {
            if (LogManager.GetLevel() <= LogLevel.Error)
            {
                msg += ToString(ex);
                logWriter.Write( LogLevel.Info, System.Threading.Thread.CurrentThread.ManagedThreadId, typeReference, msg);

            }
        }

        public void Fatal(string msg)
        {
            if (LogManager.GetLevel() <= LogLevel.Fatal)
                logWriter.Write( LogLevel.Fatal, System.Threading.Thread.CurrentThread.ManagedThreadId, typeReference, msg);
        }

        public void Fatal(string msg, Exception ex)
        {
            if (LogManager.GetLevel() <= LogLevel.Fatal)
            {
                msg += ToString(ex);
                logWriter.Write( LogLevel.Fatal, System.Threading.Thread.CurrentThread.ManagedThreadId, typeReference, msg);
                
            }

        }

        public void Info(string msg)
        {
            if (LogManager.GetLevel() <= LogLevel.Info)
                logWriter.Write( LogLevel.Info, System.Threading.Thread.CurrentThread.ManagedThreadId, typeReference, msg);
        }

        public void Info(string msg, Exception ex)
        {
            if (LogManager.GetLevel() <= LogLevel.Info)
            {
                msg += ToString(ex);
                logWriter.Write( LogLevel.Info, System.Threading.Thread.CurrentThread.ManagedThreadId, typeReference, msg);

            }
        }

        public void Warning(string msg)
        {
            if (LogManager.GetLevel() <= LogLevel.Warning)
                logWriter.Write( LogLevel.Warning, System.Threading.Thread.CurrentThread.ManagedThreadId, typeReference, msg);
        }

        public void Warning(string msg, Exception ex)
        {
            if (LogManager.GetLevel() <= LogLevel.Warning)
            {
                msg += ToString(ex);
                logWriter.Write( LogLevel.Info, System.Threading.Thread.CurrentThread.ManagedThreadId, typeReference, msg);

            }
        }
        #endregion
    }



    public enum LogLevel
    {
        Debug,
        Info,
        Warning,
        Error,
        Fatal

    }

    public static class LogManager
    {

        private static WriterManager writerManager = new WriterManager();
        private static LogLevel levelLog = LogLevel.Debug;
        public static void AppendWriter(ILogWriter logWriter)
        {
            writerManager.AppendWriter(logWriter);
        }

        public static void SetLevel(LogLevel level)
        {
            levelLog = level;
        }
        public static LogLevel GetLevel()
        {
            return levelLog;
        }

        public static ILog GetLogger(System.Type type)
        {
            return new FakeLogger(type, writerManager);
        }
    }
}
