using System;
using System.Drawing;
using System.Windows.Forms;

namespace ATSerialEmulator
{

    public class MyApplicationContext : ApplicationContext
    {
        //Component declarations
     private NotifyIcon TrayIcon;
    private ContextMenuStrip TrayIconContextMenu;
    private ToolStripMenuItem CloseMenuItem;


    public MyApplicationContext()
    {
        Application.ApplicationExit += new EventHandler(this.OnApplicationExit);
        InitializeComponent();
        TrayIcon.Visible = true;
 
    }

        private void InitializeComponent()
        {
            TrayIcon = new NotifyIcon();

   
            //Optional - handle doubleclicks on the icon:
            TrayIcon.DoubleClick += TrayIcon_DoubleClick;

            //Optional - Add a context menu to the TrayIcon:
            TrayIconContextMenu = new ContextMenuStrip();
            CloseMenuItem = new ToolStripMenuItem();
            TrayIconContextMenu.SuspendLayout();

            // 
            // TrayIconContextMenu
            // 
            this.TrayIconContextMenu.Items.AddRange(new ToolStripItem[] {
            this.CloseMenuItem});
            this.TrayIconContextMenu.Name = "TrayIconContextMenu";
            this.TrayIconContextMenu.Size = new Size(153, 70);
            // 
            // CloseMenuItem
            // 
            this.CloseMenuItem.Name = "CloseMenuItem";
            this.CloseMenuItem.Size = new Size(152, 22);
            this.CloseMenuItem.Text = "Close the tray icon program";
            this.CloseMenuItem.Click += new EventHandler(this.CloseMenuItem_Click);

            TrayIconContextMenu.ResumeLayout(false);
            TrayIcon.ContextMenuStrip = TrayIconContextMenu;

           
        }

    private void OnApplicationExit(object sender, EventArgs e)
    {
            //Cleanup so that the icon will be removed when the application is closed
            TrayIcon.Visible = false;
            ShutDown();
        }

    private void TrayIcon_DoubleClick(object sender, EventArgs e)
    {
            //Cleanup so that the icon will be removed when the application is closed
            TrayIcon.Visible = false;
            //Here you can do stuff if the tray icon is doubleclicked
            MainForm.Show();
    }


    public void HideApp()
        {
            //Cleanup so that the icon will be removed when the application is closed
            TrayIcon.Visible = true;
            //Here you can do stuff if the tray icon is doubleclicked
            MainForm.Hide();

            TrayIcon.BalloonTipIcon = ToolTipIcon.Info;
            TrayIcon.BalloonTipText =
              "AT Serial emulator is running , double click to show form.";
            TrayIcon.BalloonTipTitle = "AT Serial Emulator";
            TrayIcon.Text = "AT Serial emulator is running, double click to show form.";


            //The icon is added to the project resources.
            //Here I assume that the name of the file is 'TrayIcon.ico'
            TrayIcon.Icon = Properties.Resources.chip;

            TrayIcon.ShowBalloonTip(100000);

        }
        public void  ShutDown()
    {
        if (MainForm!=null && !MainForm.IsDisposed)
            MainForm.Close();
        Application.Exit();
    }

    private void CloseMenuItem_Click(object sender, EventArgs e)
    {
        if (MessageBox.Show("Do you really want to close me?",
                "Are you sure?", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation,
                MessageBoxDefaultButton.Button2) == DialogResult.Yes)
        {
            Application.Exit();
        }
    }
}
}
