using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.Windows.Forms;

namespace TelemetryModelSatellite.source
{
    class TcpServer
    {
        private static int bufferSize = 420;

        public static RichTextBox consoleTextBox;
        Socket server;
        

        public void OpenConnection()
        {
            var endpoint = new IPEndPoint(IPAddress.Any, MACROS.ESP_UDP_RECEIVE_PORT);

            server = new Socket(endpoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Bind(endpoint);
                server.Listen(128);
                _ = Task.Run(() => ListenToClient(server));
            }
            catch (Exception)
            {
                _ = Task.Run(() => ListenToClient(server));
            }
            
            
        }

        private async Task ListenToClient(Socket server)
        {
            do
            {
                var clientSocket = await Task.Factory.FromAsync(
                    new Func<AsyncCallback, object, IAsyncResult>(server.BeginAccept),
                    new Func<IAsyncResult, Socket>(server.EndAccept), null).ConfigureAwait(false);

                consoleTextBox.Text += "\n" + DateTime.Now.ToShortTimeString() + " Receive Port Has Opened!";

                using (var stream = new NetworkStream(clientSocket, true))
                {
                    var receivedBuffer = new byte[bufferSize];

                    do
                    {
                        int bytesRead = await stream.ReadAsync(receivedBuffer, 0, receivedBuffer.Length).ConfigureAwait(false);

                        int startingIndex = 0;

                        if(bytesRead == 0)
                        {
                            break;
                        }
                        
                        DataManager.UpdateAllDataAsync(receivedBuffer, ref startingIndex);
                        

                        receivedBuffer = new byte[bufferSize];

                    } while (true);
                    consoleTextBox.Text += "\n" + DateTime.Now.ToShortTimeString() + " Receive Port Has Closed!";
                }

            } while (true);
        }

       

        Socket transmitClient;
        Socket transmitSocket;
        IPEndPoint transmitIp;

        public async void OpenTransmit()
        {
            await Task.Run(() =>
            {
                transmitIp = new IPEndPoint(IPAddress.Any, MACROS.ESP_TCP_TRANSMIT_PORT);
                transmitSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                transmitSocket.Bind(transmitIp);
                transmitSocket.Listen(10);

                transmitClient = transmitSocket.Accept();
                consoleTextBox.Text += "\n" + DateTime.Now.ToShortTimeString() + " Client Connected To TCP";
                var clientep = (IPEndPoint)transmitClient.RemoteEndPoint;
            });

        }

        
        public async void ReOpenTransmit()
        {
            await Task.Run(() =>
            {
                transmitClient = transmitSocket.Accept();
                consoleTextBox.Text += "\n" + DateTime.Now.ToShortTimeString() + " Client Connected To TCP";
                var clientep = (IPEndPoint)transmitClient.RemoteEndPoint;

            });
                
        }

        public async void TransmitData(byte[] sendBuffer)
        {
            await Task.Run(() =>
            {
                if (transmitClient.Connected)
                {
                    transmitClient.Send(sendBuffer, sendBuffer.Length, SocketFlags.None);
                }
                else
                {
                    consoleTextBox.Text += "\n" + DateTime.Now.ToShortTimeString() + " There Isn't Any Client Connected To TCP";
                }
               
                
            });
            
        }
        
        public async void CloseTransmit()
        {
            await Task.Run(() =>
            {
                try
                {
                    transmitClient.Disconnect(true);
                    transmitClient.Close();
                }
                catch (Exception ex)
                {
                    consoleTextBox.Text += "\n" + DateTime.Now.ToShortTimeString() + ex.Message;
                }
                
            });
        }
    }
}
