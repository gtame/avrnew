﻿using ATSerialEmulator.Plugins;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.Composition;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ATSerialEmulator
{
    public partial class frmMain : Form
    {


        [Import(typeof(IATEmulator))]
        public IATEmulator emulator;

        private ATSerial serial
        {
            get
            {
                return (ATSerial)emulator;
            }
        }

        private FileSystemWatcher watcher = new FileSystemWatcher();

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

  

        public MyApplicationContext Context { get; set; }

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

            watcher.EnableRaisingEvents = false;

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
                fileDialog.Filter = "Text Files | *.txt";
                fileDialog.DefaultExt = "txt";
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
  
                //Open Serial
                serial.Open(cmbSerialPorts.Text, baudrate);

                if (serial.CommandDictionary.Count==0)
                    Logger.Warning($"Dictionary is empty!!.");
            }
            catch(System.Exception)
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


        private void AddFileWatcher(string file)
        {
            string path= Path.GetDirectoryName(file); ;
            string filter= Path.GetFileName(file);
            if (watcher.Path != path && watcher.Filter != file)
            {
                watcher.Path = path;
                watcher.Filter = filter;
                watcher.Changed += (s, e) =>
                {
                //Reload
                Logger.Info($"Dictionary file changed , reloading file..");
                    try
                    {
                        LoadDictionary(file);
                    }
                    catch (System.Exception ex)
                    {
                        Logger.Error($"Error mapping file dictionary {file}", ex);
                    }
                };
                watcher.EnableRaisingEvents = true;
            }
 
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
                AddFileWatcher(file);
           }
        }

        private void mnuSelectAll_Click(object sender, EventArgs e)
        {
            txtLog.SelectAll();
        }

        private void mnuCopiar_Click(object sender, EventArgs e)
        {
            Clipboard.SetText(txtLog.SelectedText);
        }

        private void mnuSave_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.SaveFileDialog fileDialog = new System.Windows.Forms.SaveFileDialog();
            fileDialog.Filter = "Text Files | *.txt";
            fileDialog.DefaultExt = "txt";
            if (fileDialog.ShowDialog() == DialogResult.OK)
            {
              try
                {
                    System.IO.File.WriteAllText(fileDialog.FileName,txtLog.Text);
                }
                catch(System.Exception ex)
                {
                    MessageBox.Show(this,"Unexpected error, more details in monitor","Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    Logger.Error("An error happens when the file was being saved :(", ex);
                }
            }
        }

        private void btnEditDictionary_Click(object sender, EventArgs e)
        {
            try
            {
                if (System.IO.File.Exists(txtDictionaryPath.Text))
                {
                    ProcessStartInfo psi = new ProcessStartInfo(txtDictionaryPath.Text);
                    psi.UseShellExecute = true;
                    Process.Start(psi);
                }
                else
                    MessageBox.Show(this, "There isn't a dictionary file selected valid", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            catch(System.Exception ex )
            {
                MessageBox.Show(this, "Unexpected error, more details in monitor", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Logger.Error("An error happens when the file was being saved :(", ex);
            }
        }

        private void mnuClear_Click(object sender, EventArgs e)
        {
            txtLog.Clear();
        }

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (Visible)
            {
                e.Cancel = true;
                Context.HideApp();
            }

        }
    }
}
