using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;

namespace TelemetryModelSatellite.source
{
    class UdpServer
    {
        public static RichTextBox consoleTextBox;
        static int bufferCounter = 0;

        Socket server = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

        IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, MACROS.ESP_UDP_RECEIVE_PORT);

        
        byte[] receivedBuffer = new byte[MACROS.CHUNK_SIZE];


        public void OpenConnection()
        {
            server.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.ReuseAddress, true);
            server.Bind(endPoint);


            server.BeginReceive(receivedBuffer, 0, receivedBuffer.Length, SocketFlags.None, new AsyncCallback(Receive), null);
        }

        private void Receive(IAsyncResult result)
        {
            int startingIndex = 0;
            bufferCounter++;
            if (bufferCounter % 4 == 0)
            {
                
                server.EndReceive(result);
                try
                {
                    consoleTextBox.Text += "\n " + DateTime.Now.ToShortTimeString() + " New buffer received ";
                    DataManager.UpdateAllDataAsync(receivedBuffer, ref startingIndex);
                }
                catch (Exception)
                { }

                    
            }
            if (bufferCounter % 80 == 0)
            {
                consoleTextBox.Text = "";
                consoleTextBox.Text += "\n " + DateTime.Now.ToShortTimeString() + " " + bufferCounter / 4 + " buffer received ";
                
            }
            
            receivedBuffer = new byte[MACROS.CHUNK_SIZE];


            startingIndex = 0;
            
            server.BeginReceive(receivedBuffer, 0, receivedBuffer.Length, SocketFlags.None, new AsyncCallback(Receive), null);


        }


    }
}
