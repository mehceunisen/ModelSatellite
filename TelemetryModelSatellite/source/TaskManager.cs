using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TelemetryModelSatellite.source
{
    class TaskManager
    {
        public static TcpServer tcpServer { get; set; }
        public static List<ToolStripMenuItem> dropdownButtons { get; set; }

        public TaskManager()
        {
            tcpServer.OpenTransmit();
        }

        public async void UartRecieveAsync()
        {
            await Task.Run(() =>
            {
                if (PACKET.uartTaskState)
                {
                    tcpServer.TransmitData(PACKET.uartReceiveStopCmd);
                    dropdownButtons[0].Text = "Uart Task Stopped";
                }
                else
                {
                    tcpServer.TransmitData(PACKET.uartReceiveStartCmd);
                    dropdownButtons[0].Text = "Uart Task Started";
                }
                PACKET.uartTaskState = !PACKET.uartTaskState;
            });
        }

        public async void DataProcessAsync()
        {
            await Task.Run(() =>
            {
                if (PACKET.dataProcessState)
                {
                    tcpServer.TransmitData(PACKET.dataProcessStopCmd);
                    dropdownButtons[1].Text = "Data Process Stopped";
                }
                else
                {
                    tcpServer.TransmitData(PACKET.dataProcessStartCmd);
                    dropdownButtons[1].Text = "Data Processing";
                }
                PACKET.dataProcessState = !PACKET.dataProcessState;
            });
        }

        public async void TelemetryTransmitAsync()
        {
            await Task.Run(() =>
            {
                if (PACKET.telemetryTaskState)
                {
                    tcpServer.TransmitData(PACKET.telemetryTransmitStopCmd);
                    dropdownButtons[2].Text = "Telemetry Transmit Stopped";
                }
                else
                {
                    tcpServer.TransmitData(PACKET.telemetryTransmitStartCmd);
                    dropdownButtons[2].Text = "Telemetry Transmitting";
                }
                PACKET.telemetryTaskState = !PACKET.telemetryTaskState;
            });
        }

        public async void VideoStreamAsync()
        {
            await Task.Run(() =>
            {
                if (PACKET.videoStreamState)
                {
                    tcpServer.TransmitData(PACKET.videoStreamStopCmd);
                    dropdownButtons[3].Text = "Video Stream Stopped";
                }
                else
                {
                    tcpServer.TransmitData(PACKET.videoStreamStartCmd);
                    dropdownButtons[3].Text = "Video Streaming";
                }
                PACKET.videoStreamState = !PACKET.videoStreamState;
            });
        }

        public async void VideoRecorderAsync()
        {
            await Task.Run(() =>
            {
                if (PACKET.videoRecorderState)
                {
                    tcpServer.TransmitData(PACKET.videoRecorderStopCmd);
                    dropdownButtons[4].Text = "Video Recorder Stopped";
                }
                else
                {
                    tcpServer.TransmitData(PACKET.videoRecorderStartCmd);
                    dropdownButtons[4].Text = "Video Recording";
                }
                PACKET.videoRecorderState = !PACKET.videoRecorderState;
            });
        }

        public async void FtpTaskAsync()
        {
            await Task.Run(() =>
            {
                if (PACKET.ftpTaskState)
                {
                    tcpServer.TransmitData(PACKET.ftpTaskStopCmd);
                    dropdownButtons[5].Text = "FTP Stopped";
                }
                else
                {
                    tcpServer.TransmitData(PACKET.ftpTaskStartCmd);
                    dropdownButtons[5].Text = "FTP Started";
                }
                PACKET.ftpTaskState = !PACKET.ftpTaskState;
            });
        }
    }
}
