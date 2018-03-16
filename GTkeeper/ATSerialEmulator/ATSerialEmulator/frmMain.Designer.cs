namespace ATSerialEmulator
{
    partial class frmMain
    {
        /// <summary>
        /// Variable del diseñador necesaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén usando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben desechar; false en caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido de este método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmMain));
            this.pnlTop = new System.Windows.Forms.Panel();
            this.btnEditDictionary = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.txtDictionaryPath = new System.Windows.Forms.TextBox();
            this.btnDictionary = new System.Windows.Forms.Button();
            this.btnListen = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.txtBaudRate = new System.Windows.Forms.TextBox();
            this.cmbSerialPorts = new System.Windows.Forms.ComboBox();
            this.pnlBottom = new System.Windows.Forms.Panel();
            this.txtCommand = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnFile = new System.Windows.Forms.Button();
            this.grpEOL = new System.Windows.Forms.GroupBox();
            this.chkLF = new System.Windows.Forms.CheckBox();
            this.chkCR = new System.Windows.Forms.CheckBox();
            this.btnSendAscii = new System.Windows.Forms.Button();
            this.btnSendHex = new System.Windows.Forms.Button();
            this.txtLog = new System.Windows.Forms.RichTextBox();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.mnuSelectAll = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuCopiar = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuSave = new System.Windows.Forms.ToolStripMenuItem();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.mnuClear = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.pnlTop.SuspendLayout();
            this.pnlBottom.SuspendLayout();
            this.panel1.SuspendLayout();
            this.grpEOL.SuspendLayout();
            this.contextMenuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // pnlTop
            // 
            this.pnlTop.Controls.Add(this.btnEditDictionary);
            this.pnlTop.Controls.Add(this.label3);
            this.pnlTop.Controls.Add(this.txtDictionaryPath);
            this.pnlTop.Controls.Add(this.btnDictionary);
            this.pnlTop.Controls.Add(this.btnListen);
            this.pnlTop.Controls.Add(this.label2);
            this.pnlTop.Controls.Add(this.label1);
            this.pnlTop.Controls.Add(this.txtBaudRate);
            this.pnlTop.Controls.Add(this.cmbSerialPorts);
            this.pnlTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnlTop.Location = new System.Drawing.Point(0, 0);
            this.pnlTop.Margin = new System.Windows.Forms.Padding(4);
            this.pnlTop.Name = "pnlTop";
            this.pnlTop.Size = new System.Drawing.Size(1059, 90);
            this.pnlTop.TabIndex = 2;
            // 
            // btnEditDictionary
            // 
            this.btnEditDictionary.Image = global::ATSerialEmulator.Properties.Resources.script_edit;
            this.btnEditDictionary.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnEditDictionary.Location = new System.Drawing.Point(759, 45);
            this.btnEditDictionary.Margin = new System.Windows.Forms.Padding(4);
            this.btnEditDictionary.Name = "btnEditDictionary";
            this.btnEditDictionary.Size = new System.Drawing.Size(204, 31);
            this.btnEditDictionary.TabIndex = 9;
            this.btnEditDictionary.Text = "Edit dictionary...";
            this.btnEditDictionary.UseVisualStyleBackColor = true;
            this.btnEditDictionary.Click += new System.EventHandler(this.btnEditDictionary_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 59);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(71, 16);
            this.label3.TabIndex = 8;
            this.label3.Text = "Dictionary:";
            // 
            // txtDictionaryPath
            // 
            this.txtDictionaryPath.BackColor = System.Drawing.Color.White;
            this.txtDictionaryPath.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtDictionaryPath.Location = new System.Drawing.Point(84, 49);
            this.txtDictionaryPath.Multiline = true;
            this.txtDictionaryPath.Name = "txtDictionaryPath";
            this.txtDictionaryPath.ReadOnly = true;
            this.txtDictionaryPath.Size = new System.Drawing.Size(484, 26);
            this.txtDictionaryPath.TabIndex = 7;
            // 
            // btnDictionary
            // 
            this.btnDictionary.Image = global::ATSerialEmulator.Properties.Resources.book_open;
            this.btnDictionary.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnDictionary.Location = new System.Drawing.Point(585, 45);
            this.btnDictionary.Margin = new System.Windows.Forms.Padding(4);
            this.btnDictionary.Name = "btnDictionary";
            this.btnDictionary.Size = new System.Drawing.Size(166, 31);
            this.btnDictionary.TabIndex = 6;
            this.btnDictionary.Text = "Open dictionary...";
            this.btnDictionary.UseVisualStyleBackColor = true;
            this.btnDictionary.Click += new System.EventHandler(this.btnDictionary_Click);
            // 
            // btnListen
            // 
            this.btnListen.Location = new System.Drawing.Point(393, 8);
            this.btnListen.Margin = new System.Windows.Forms.Padding(4);
            this.btnListen.Name = "btnListen";
            this.btnListen.Size = new System.Drawing.Size(120, 31);
            this.btnListen.TabIndex = 5;
            this.btnListen.Text = "Listen";
            this.btnListen.UseVisualStyleBackColor = true;
            this.btnListen.Click += new System.EventHandler(this.btnListen_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(211, 20);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(66, 16);
            this.label2.TabIndex = 4;
            this.label2.Text = "Baudrate:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(41, 23);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 16);
            this.label1.TabIndex = 3;
            this.label1.Text = "Port:";
            // 
            // txtBaudRate
            // 
            this.txtBaudRate.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtBaudRate.Location = new System.Drawing.Point(285, 14);
            this.txtBaudRate.Margin = new System.Windows.Forms.Padding(4);
            this.txtBaudRate.Name = "txtBaudRate";
            this.txtBaudRate.Size = new System.Drawing.Size(84, 26);
            this.txtBaudRate.TabIndex = 1;
            // 
            // cmbSerialPorts
            // 
            this.cmbSerialPorts.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmbSerialPorts.FormattingEnabled = true;
            this.cmbSerialPorts.Location = new System.Drawing.Point(84, 12);
            this.cmbSerialPorts.Margin = new System.Windows.Forms.Padding(4);
            this.cmbSerialPorts.Name = "cmbSerialPorts";
            this.cmbSerialPorts.Size = new System.Drawing.Size(113, 28);
            this.cmbSerialPorts.TabIndex = 0;
            this.cmbSerialPorts.SelectedIndexChanged += new System.EventHandler(this.cmbSerialPorts_SelectedIndexChanged);
            // 
            // pnlBottom
            // 
            this.pnlBottom.Controls.Add(this.txtCommand);
            this.pnlBottom.Controls.Add(this.panel1);
            this.pnlBottom.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.pnlBottom.Location = new System.Drawing.Point(0, 476);
            this.pnlBottom.Margin = new System.Windows.Forms.Padding(4);
            this.pnlBottom.Name = "pnlBottom";
            this.pnlBottom.Size = new System.Drawing.Size(1059, 46);
            this.pnlBottom.TabIndex = 3;
            // 
            // txtCommand
            // 
            this.txtCommand.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtCommand.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtCommand.Location = new System.Drawing.Point(0, 0);
            this.txtCommand.Margin = new System.Windows.Forms.Padding(4);
            this.txtCommand.Multiline = true;
            this.txtCommand.Name = "txtCommand";
            this.txtCommand.Size = new System.Drawing.Size(555, 46);
            this.txtCommand.TabIndex = 4;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.btnFile);
            this.panel1.Controls.Add(this.grpEOL);
            this.panel1.Controls.Add(this.btnSendAscii);
            this.panel1.Controls.Add(this.btnSendHex);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Right;
            this.panel1.Location = new System.Drawing.Point(555, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(504, 46);
            this.panel1.TabIndex = 3;
            // 
            // btnFile
            // 
            this.btnFile.Location = new System.Drawing.Point(229, 8);
            this.btnFile.Margin = new System.Windows.Forms.Padding(4);
            this.btnFile.Name = "btnFile";
            this.btnFile.Size = new System.Drawing.Size(97, 30);
            this.btnFile.TabIndex = 1;
            this.btnFile.Text = "Send File";
            this.btnFile.UseVisualStyleBackColor = true;
            this.btnFile.Click += new System.EventHandler(this.btnFile_Click);
            // 
            // grpEOL
            // 
            this.grpEOL.Controls.Add(this.chkLF);
            this.grpEOL.Controls.Add(this.chkCR);
            this.grpEOL.Location = new System.Drawing.Point(333, 0);
            this.grpEOL.Name = "grpEOL";
            this.grpEOL.Size = new System.Drawing.Size(164, 40);
            this.grpEOL.TabIndex = 4;
            this.grpEOL.TabStop = false;
            this.grpEOL.Text = "Suffix - End Of Line";
            // 
            // chkLF
            // 
            this.chkLF.AutoSize = true;
            this.chkLF.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkLF.Location = new System.Drawing.Point(110, 17);
            this.chkLF.Name = "chkLF";
            this.chkLF.Size = new System.Drawing.Size(47, 19);
            this.chkLF.TabIndex = 3;
            this.chkLF.Text = "+LF";
            this.chkLF.UseVisualStyleBackColor = true;
            // 
            // chkCR
            // 
            this.chkCR.AutoSize = true;
            this.chkCR.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkCR.Location = new System.Drawing.Point(25, 17);
            this.chkCR.Name = "chkCR";
            this.chkCR.Size = new System.Drawing.Size(50, 19);
            this.chkCR.TabIndex = 2;
            this.chkCR.Text = "+CR";
            this.chkCR.UseVisualStyleBackColor = true;
            // 
            // btnSendAscii
            // 
            this.btnSendAscii.Location = new System.Drawing.Point(7, 8);
            this.btnSendAscii.Name = "btnSendAscii";
            this.btnSendAscii.Size = new System.Drawing.Size(106, 30);
            this.btnSendAscii.TabIndex = 1;
            this.btnSendAscii.Text = "Send Ascii";
            this.btnSendAscii.UseVisualStyleBackColor = true;
            this.btnSendAscii.Click += new System.EventHandler(this.btnSendAscii_Click);
            // 
            // btnSendHex
            // 
            this.btnSendHex.Location = new System.Drawing.Point(119, 8);
            this.btnSendHex.Name = "btnSendHex";
            this.btnSendHex.Size = new System.Drawing.Size(103, 30);
            this.btnSendHex.TabIndex = 0;
            this.btnSendHex.Text = "Send Hex";
            this.btnSendHex.UseVisualStyleBackColor = true;
            this.btnSendHex.Click += new System.EventHandler(this.btnSendHex_Click);
            // 
            // txtLog
            // 
            this.txtLog.BackColor = System.Drawing.Color.White;
            this.txtLog.ContextMenuStrip = this.contextMenuStrip1;
            this.txtLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtLog.Location = new System.Drawing.Point(0, 90);
            this.txtLog.Margin = new System.Windows.Forms.Padding(4);
            this.txtLog.Name = "txtLog";
            this.txtLog.ReadOnly = true;
            this.txtLog.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.txtLog.Size = new System.Drawing.Size(1059, 386);
            this.txtLog.TabIndex = 4;
            this.txtLog.Text = "";
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuClear,
            this.toolStripSeparator3,
            this.mnuSelectAll,
            this.toolStripSeparator1,
            this.mnuCopiar,
            this.toolStripSeparator2,
            this.mnuSave});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(153, 132);
            // 
            // mnuSelectAll
            // 
            this.mnuSelectAll.Name = "mnuSelectAll";
            this.mnuSelectAll.Size = new System.Drawing.Size(152, 22);
            this.mnuSelectAll.Text = "Select All";
            this.mnuSelectAll.Click += new System.EventHandler(this.mnuSelectAll_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(149, 6);
            // 
            // mnuCopiar
            // 
            this.mnuCopiar.Name = "mnuCopiar";
            this.mnuCopiar.Size = new System.Drawing.Size(152, 22);
            this.mnuCopiar.Text = "Copy";
            this.mnuCopiar.Click += new System.EventHandler(this.mnuCopiar_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(149, 6);
            // 
            // mnuSave
            // 
            this.mnuSave.Name = "mnuSave";
            this.mnuSave.Size = new System.Drawing.Size(152, 22);
            this.mnuSave.Text = "Save Log file";
            this.mnuSave.Click += new System.EventHandler(this.mnuSave_Click);
            // 
            // mnuClear
            // 
            this.mnuClear.Name = "mnuClear";
            this.mnuClear.Size = new System.Drawing.Size(152, 22);
            this.mnuClear.Text = "Clear";
            this.mnuClear.Click += new System.EventHandler(this.mnuClear_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(149, 6);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1059, 522);
            this.Controls.Add(this.txtLog);
            this.Controls.Add(this.pnlBottom);
            this.Controls.Add(this.pnlTop);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "frmMain";
            this.Text = "AT Serial Emulator";
            this.pnlTop.ResumeLayout(false);
            this.pnlTop.PerformLayout();
            this.pnlBottom.ResumeLayout(false);
            this.pnlBottom.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.grpEOL.ResumeLayout(false);
            this.grpEOL.PerformLayout();
            this.contextMenuStrip1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel pnlTop;
        private System.Windows.Forms.Panel pnlBottom;
        private System.Windows.Forms.RichTextBox txtLog;
        private System.Windows.Forms.Button btnFile;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtBaudRate;
        private System.Windows.Forms.ComboBox cmbSerialPorts;
        private System.Windows.Forms.Button btnDictionary;
        private System.Windows.Forms.Button btnListen;
        private System.Windows.Forms.TextBox txtCommand;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.GroupBox grpEOL;
        private System.Windows.Forms.CheckBox chkLF;
        private System.Windows.Forms.CheckBox chkCR;
        private System.Windows.Forms.Button btnSendAscii;
        private System.Windows.Forms.Button btnSendHex;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtDictionaryPath;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem mnuSelectAll;
        private System.Windows.Forms.ToolStripMenuItem mnuCopiar;
        private System.Windows.Forms.ToolStripMenuItem mnuSave;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.Button btnEditDictionary;
        private System.Windows.Forms.ToolStripMenuItem mnuClear;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
    }
}

