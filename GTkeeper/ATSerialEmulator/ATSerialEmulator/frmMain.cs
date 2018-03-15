using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ATSerialEmulator
{
    public partial class frmMain : Form
    {

        private ATSerial serial = null;


        private ILog _logger = null;
        protected ILog Logger
        {
            get
            {
                if (_logger == null)
                    _logger = LogManager.GetLogger(this.GetType());
                return _logger;
            }
        }


        public frmMain()
        {
            InitializeComponent();
        }

        protected override void OnLoad(EventArgs e)
        {
            LogManager.SetLevel(LogLevel.Info);
            LogManager.AppendWriter(new ControlWriter(txtLog,"{M}"));

            Logger.Info("Get SerialPorts...");
            cmbSerialPorts.Items.Clear();
            // Serials
            cmbSerialPorts.Items.AddRange(SerialPort.GetPortNames());

            //Arguments ....

            if (!string.IsNullOrEmpty(ArgumentParser.Port))
            {
               if ( cmbSerialPorts.Items.Contains(ArgumentParser.Port))
                    cmbSerialPorts.Text = ArgumentParser.Port;
               else
                    Logger.Error($"Serialport {ArgumentParser.Port}  param isn't correct ");

            }


            if (!string.IsNullOrEmpty(ArgumentParser.BaudRate) )
            {
                int baudrate = 0;
                if (int.TryParse(ArgumentParser.BaudRate, out baudrate))
                {
                    txtBaudRate.Text = baudrate.ToString();
                }
                else
                    Logger.Error($"BaudRate param isn't correct {ArgumentParser.BaudRate}");
            }

            chkCR.Checked = ArgumentParser.CR;
            chkLF.Checked = ArgumentParser.LF;

            if (ArgumentParser.Open)
            {
                Open();
            }


            if (!string.IsNullOrEmpty(ArgumentParser.Dictionary))
            {

                if (System.IO.File.Exists(ArgumentParser.Dictionary))
                {
                    LoadDictionary(ArgumentParser.Dictionary);
                }
                else
                    Logger.Error($"The file '{ArgumentParser.Dictionary}' doesn't exists ");
            }


            base.OnLoad(e);
        }

    
        protected override void OnClosed(EventArgs e)
        {

            if (serial != null)
            {
                serial.Close();
                serial.Dispose();
            }
            base.OnClosed(e);
        }

        private void cmbSerialPorts_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(cmbSerialPorts.Text) && 
                cmbSerialPorts.Items.Contains(cmbSerialPorts.Text) 
                )
            {
                Logger.Info($"Get settings serial Port {cmbSerialPorts.Text}");
                SerialPort serial = new SerialPort(cmbSerialPorts.Text);
                txtBaudRate.Text = serial.BaudRate.ToString();
                Logger.Info($"\tBaudRate default {serial.BaudRate}");
                Logger.Info($"\tParity {serial.Parity}");
                Logger.Info($"\tBits {serial.DataBits}");
                Logger.Info($"\tStopBits {serial.StopBits}");
                Logger.Info($"\tHandShake {serial.Handshake}");
                serial.Dispose();
            }
        }

        private void btnListen_Click(object sender, EventArgs e)
        {
            Open();

        }

   

        private void btnFile_Click(object sender, EventArgs e)
        {
            if (IsAvailable())
            {
                System.Windows.Forms.OpenFileDialog fileDialog = new System.Windows.Forms.OpenFileDialog();
                fileDialog.Multiselect = false;
                fileDialog.CheckFileExists = true;
                if (fileDialog.ShowDialog() == DialogResult.OK)
                {
                    serial.SendFile(fileDialog.FileName);
                }
            }
            else
                Logger.Error("First you must use listen command");
        }

        private void btnSendAscii_Click(object sender, EventArgs e)
        {
            if (IsAvailable())
            {
                serial.AddCommand(txtCommand.Text + GetSuffix());
                txtCommand.Text = string.Empty;
                txtCommand.Focus();
            }
            else
                Logger.Error("First you must use listen command");
        }

        private void btnSendHex_Click(object sender, EventArgs e)
        {
            if (IsAvailable())
            {

                byte[] bytes = System.Text.UnicodeEncoding.Default.GetBytes(txtCommand.Text + GetSuffix() );
                serial.AddCommand(bytes);
                txtCommand.Text = string.Empty;
                txtCommand.Focus();
            }
            else
                Logger.Error("First you must use listen command");
        }

        private void btnDictionary_Click(object sender, EventArgs e)
        {
        
                System.Windows.Forms.OpenFileDialog fileDialog = new System.Windows.Forms.OpenFileDialog();
                fileDialog.Multiselect = false;
                fileDialog.CheckFileExists = true;
                if (fileDialog.ShowDialog() == DialogResult.OK)
                {
                    LoadDictionary(fileDialog.FileName);
                }
 
        }


        private void Open()
        {

 
            if (!cmbSerialPorts.Items.Contains(cmbSerialPorts.Text))
            {
                Logger.Error($"Serialport {cmbSerialPorts.Text}  param isn't correct ");
            }

            int baudrate = 0;
            if (int.TryParse(txtBaudRate.Text, out baudrate))
                txtBaudRate.Text = baudrate.ToString();
            else
            { 
                Logger.Error($"BaudRate param isn't correct {txtBaudRate.Text}");
                return;
            }


            try
            {
                //Open Serial port
                serial = new ATSerial(cmbSerialPorts.Text, baudrate);

                //Open Serial
                serial.Open();

                if (serial.CommandDictionary.Count==0)
                    Logger.Warning($"Dictionary is empty!!.");
            }
            catch(System.Exception ex)
            {
                Logger.Error($"Can't open port {cmbSerialPorts.Text}");
                return;
            }
        
            //Disable controls
            cmbSerialPorts.Enabled = false;
            txtBaudRate.Enabled = false;
            btnListen.Enabled = false;
        }

        private bool IsAvailable()
        {
            return (serial != null && serial.IsOpened);
        }

        private string GetSuffix()
        {
            return  (chkCR.Checked ? "\r" : "") + (chkLF.Checked ? "\n" : "");
        }

        private void LoadDictionary(string file)
        {
            if (IsAvailable())
            {
                Logger.Info($"Loading dictionary {file}");
                serial.ClearDictionary();
                //Load dictionary
                foreach (string line in System.IO.File.ReadAllLines(file))
                {
                    if (!string.IsNullOrEmpty(line))
                    {
                        int index = line.IndexOf(';');
                        if (index != -1)
                        {
                            string comando = line.Substring(0, index);
                            string response = line.Substring(index + 1);
                            Logger.Info($"Command {comando}");
                            Logger.Info($"Response {response}");

                            serial.AddResponse(comando, response);
                        }
                        else
                        {
                            Logger.Error($"Command {line} without reponse :(");
                        }
                    }
                }
                Logger.Info($"Dictionary contains {serial.CommandDictionary.Count} entries");
                txtDictionaryPath.Text = file;
            }
        }
    }
}
