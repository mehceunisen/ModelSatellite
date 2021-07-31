using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;
using TelemetryModelSatellite.source;
using GMap.NET;
using GMap.NET.MapProviders;
using GMap.NET.WindowsForms;
using System.Windows.Forms.DataVisualization.Charting;
using System.IO;

namespace TelemetryModelSatellite
{
    public partial class Form1 : Form
    {
        bool isConnected = false;


        GlController glController;
        OpenFileDialog ofd;
        GmapController gmapController;
        VideoSaver videoSaver;
        DataLogger dataLogger;
        TcpServer tcpServer;
        VideoReceiver videoReceiver;
        UdpServer udpServer;
        TaskManager taskManager;

        public static List<Label> labels;
        public static List<Chart> charts;
        public static List<Button> buttons;
        public static List<ToolStripMenuItem> dropdownButtons;

        public Form1()
        {
            InitializeComponent();

            #region object referencing
            glController = new GlController(glControl);
            gmapController = new GmapController(gMapControl);
            videoSaver = new VideoSaver();
            
            dataLogger = new DataLogger();
            tcpServer = new TcpServer();
            videoReceiver = new VideoReceiver(this.pictureBox);
            udpServer = new UdpServer();

            TaskManager.tcpServer = tcpServer;
            taskManager = new TaskManager();
            #endregion

            #region setting lists
            labels = new List<Label>()
            {
               noLabel, timeLabel, packetLabel, pLabel,
               heightLabel,speedLabel, tempLabel, batteryLabel, 
               latLabel, lngLabel, altLabel, mvlnLabel, 
               ftpStatLabel, satStateLabel, pitchLabel, rollLabel,
               yawLabel, lastCmdLabel, rssiValueLabel
            };

            charts = new List<Chart>()
            {
                hAndAltChart,
                yawPitchRollChart
            };

            buttons = new List<Button>()
            {
                connectButton, fileTransferButton,
                detachPayloadButton, modeButton
            };

            dropdownButtons = new List<ToolStripMenuItem>() 
            { 
                uartTaskToolStripMenuItem, dataProcessToolStripMenuItem, telemetryTransmitToolStripMenuItem,
                videoStreamToolStripMenuItem, videoRecorderToolStripMenuItem, fTPTaskToolStripMenuItem
            };
            #endregion

            #region getter setters
            DataManager.charts = charts;
            DataManager.labels = labels;

            ModeManager.leftPanel = this.leftPanel;
            ModeManager.form1 = this;
            ModeManager.buttons = buttons;

            TcpServer.consoleTextBox = this.consoleTextBox;
            FtpUploader.consoleTextBox = this.consoleTextBox;

            UdpServer.consoleTextBox = this.consoleTextBox;

            TaskManager.dropdownButtons = dropdownButtons;
            
            #endregion

            Control.CheckForIllegalCrossThreadCalls = false;

        }

        #region .. Double Buffered function ..
        public static void SetDoubleBuffered(System.Windows.Forms.Control c)
        {
            if (System.Windows.Forms.SystemInformation.TerminalServerSession)
                return;
            System.Reflection.PropertyInfo aProp = typeof(System.Windows.Forms.Control).GetProperty("DoubleBuffered", System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance);
            aProp.SetValue(c, true, null);
        }
        #endregion

        private void Form1_Load(object sender, EventArgs e)
        {
            #region Setting Double Buffer
            SetDoubleBuffered(MainPanel);
            SetDoubleBuffered(glGmapPanel);
            SetDoubleBuffered(pictureBox);
            SetDoubleBuffered(tableLayoutPanel1);
            SetDoubleBuffered(tableLayoutPanel2);
            SetDoubleBuffered(tableLayoutPanel3);
            SetDoubleBuffered(tableLayoutPanel4);
            SetDoubleBuffered(tableLayoutPanel5);
            SetDoubleBuffered(tableLayoutPanel6);
            SetDoubleBuffered(tableLayoutPanel7);
            SetDoubleBuffered(tableLayoutPanel8);
            SetDoubleBuffered(tableLayoutPanel9);
            SetDoubleBuffered(tableLayoutPanel10);
            SetDoubleBuffered(tableLayoutPanel11);
            SetDoubleBuffered(tableLayoutPanel12);
            SetDoubleBuffered(tableLayoutPanel13);
            SetDoubleBuffered(tableLayoutPanel14);
            SetDoubleBuffered(tableLayoutPanel15);
            SetDoubleBuffered(tableLayoutPanel16);
            SetDoubleBuffered(tableLayoutPanel17);
            SetDoubleBuffered(tableLayoutPanel18);
            SetDoubleBuffered(tableLayoutPanel19);
            SetDoubleBuffered(tableLayoutPanel20);
            SetDoubleBuffered(tableLayoutPanel21);
            SetDoubleBuffered(leftPanel);

            foreach (var label in labels)
            {
                SetDoubleBuffered(label);
            }
            foreach (var button in buttons)
            {
                SetDoubleBuffered(button);
            }
            foreach (var chart in charts)
            {
                SetDoubleBuffered(chart);
            }

            #endregion
        }

        private void connectButton_Click(object sender, EventArgs e)
        {
            if (!isConnected)
            {
                try
                {
                    MACROS.ESP_IP = ipTextBox.Text;
                    udpServer.OpenConnection();
                    videoReceiver.StartVideoTransfer();
                    consoleTextBox.Text += "\nServer Started To Listen!";
                    isConnected = true;
                    ipTextBox.Enabled = false;
                    Task.Run(() => gmapController.InitGmap(41.025948, 28.889465));
                    timer.Start();

                }
                catch (Exception ex)
                {
                    consoleTextBox.Text += DateTime.Now.ToShortTimeString() + "\nA Problem Occured While Starting The Server!";
                    consoleTextBox.Text += "\n" + ex.Message;
                }
            }
        }


        public async void fileTransferButton_Click(object sender, EventArgs e)
        {
            
            ofd = new OpenFileDialog();

            if (ofd.ShowDialog() == DialogResult.OK)
            {


                string[] paths = ofd.FileNames;
                string localFile = paths[0];
                FtpUploader ftpUploader = new FtpUploader();
                MACROS.videoExtension = Path.GetExtension(localFile);
                await Task.Run(() => ftpUploader.SendFileFtpServer(localFile));
            }
        }

        private void detachPayloadButton_Click(object sender, EventArgs e)
        {
            
        }


        private void glControl_Load(object sender, EventArgs e)
        {
            glController.GlControlLoad();
        }

        private void glControl_Paint(object sender, PaintEventArgs e)
        {
            glController.PaintGLControl();
        }

        private void gMapControl_Load(object sender, EventArgs e)
        {
            gmapController.InitGmap(41.025948, 28.889465);
        }

        private void gMapControl_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                PointLatLng mousePos = gMapControl.FromLocalToLatLng(e.X, e.Y);
                gmapController.PinLocation(mousePos);
            }
            
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            videoSaver.StopVideoRecord();
            dataLogger.StopLogging();
        }

        private void gMapControl_MouseClick(object sender, MouseEventArgs e)
        {
            
        }

        private void modeButton_Click(object sender, EventArgs e)
        {
            consoleTextBox.Text = "";
        }

        private void uartTaskToolStripMenuItem_Click(object sender, EventArgs e)
        {
            taskManager.UartRecieveAsync();
        }

        private void dataProcessToolStripMenuItem_Click(object sender, EventArgs e)
        {
            taskManager.DataProcessAsync();
        }

        private void telemetryTransmitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            taskManager.TelemetryTransmitAsync();
        }

        private void videoStreamToolStripMenuItem_Click(object sender, EventArgs e)
        {
            taskManager.VideoStreamAsync();
        }

        private void videoRecorderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            taskManager.VideoRecorderAsync();
        }

        private void fTPTaskToolStripMenuItem_Click(object sender, EventArgs e)
        {
            taskManager.FtpTaskAsync();
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            tcpServer.CloseTransmit();
            tcpServer.ReOpenTransmit();
        }
    }
}
