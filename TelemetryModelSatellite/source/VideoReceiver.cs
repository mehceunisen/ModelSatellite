using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;

namespace TelemetryModelSatellite.source
{
    class VideoReceiver
    {

        static private PictureBox pictureBox;

        public VideoReceiver(PictureBox _pictureBox)
        {
            pictureBox = _pictureBox;
        }
        public VideoReceiver()
        {

        }
        VideoSaver videoSaver = new VideoSaver();



        UdpClient Client = new UdpClient(MACROS.ESP_UDP_PORT_NUMBER);

        byte[] displaying_frame;

        List<Frame> Frames = new List<Frame>();


        int lastframe = -1;
        int last_index = -1;


        private UInt16 BAToUInt16(byte[] bytes, int index)
        {
            return BitConverter.ToUInt16(bytes, index);

        }

        byte[] ChunkWithoutHeader(byte[] rawchunk, int offset, int length)
        {
            byte[] temp = new byte[length];


            if ((rawchunk.Length < offset) || (offset < 0) || (rawchunk.Length < length))
            {
                return new byte[] { 0 };
            }

            for (int i = offset, j = 0; i < rawchunk.Length; i++, j++)
            {
                temp[j] = rawchunk[i];
            }
            return temp;
        }

        public void StartVideoTransfer()
        {
            try
            {
                Client.BeginReceive(new AsyncCallback(Receive), null);
            }
            catch (Exception)
            {

                
            }

        }


        public void StopVideoTransfer()
        {
            Client.Close();
            Client.Dispose();
        }

        private void Receive(IAsyncResult asyncResult)
        {
            try
            {
                IPEndPoint RemoteIP = new IPEndPoint(IPAddress.Any, MACROS.ESP_UDP_PORT_NUMBER);


                byte[] ReceivedData = Client.EndReceive(asyncResult, ref RemoteIP);


                #region After_Receive_Tasks            
                if (ReceivedData.Length == 0)
                {

                }
                else if (ReceivedData.Length == MACROS.CHUNK_SIZE)
                {
                    //burada yeni bir chunk olustur 
                    Chunk temp_chunk = new Chunk
                    {
                        chunk_count = BAToUInt16(ReceivedData, 0),
                        frame_number = BAToUInt16(ReceivedData, 2),
                        chunk_number = ReceivedData[4],
                        jpegData = ChunkWithoutHeader(ReceivedData, MACROS.HEADER_SIZE, MACROS.CHUNK_SIZE)
                    };

                    //yeni frameye geldik
                    if (BAToUInt16(ReceivedData, 2) > lastframe)
                    {
                        lastframe = BAToUInt16(ReceivedData, 2);
                        last_index++;
                        Chunk[] temp = new Chunk[temp_chunk.chunk_count];
                        temp[temp_chunk.chunk_number] = temp_chunk;

                        bool[] avaible_temp = new bool[temp_chunk.chunk_count];
                        avaible_temp[temp_chunk.chunk_number] = true;
                        Frame tempFrame = new Frame
                        {
                            Chunks = temp,
                            chunk_count = temp_chunk.chunk_count,
                            frame_number = temp_chunk.frame_number,
                            chunks_avaible = avaible_temp,
                            is_rendered = false
                        };
                        Frames.Add(tempFrame);

                    }//ornek senaryo onceki frameden chunlar geliyor
                    else if ((last_index > 0) && (Frames[last_index - 1].frame_number == temp_chunk.frame_number))
                    {
                        Frames[last_index - 1].Chunks[temp_chunk.chunk_number] = temp_chunk;
                        Frames[last_index - 1].chunks_avaible[temp_chunk.chunk_number] = true;
                    }
                    else
                    {
                        Frames[last_index].Chunks[temp_chunk.chunk_number] = temp_chunk;
                        Frames[last_index].chunks_avaible[temp_chunk.chunk_number] = true;
                    }

                    //biseyler olmus olması lazim buraya kadar
                    if (last_index > 0)
                    {
                        if (Frames[last_index].is_render_able(Frames[last_index].chunks_avaible))
                        {
                            displaying_frame = Frames[last_index].GetFrameOrder();
                            MemoryStream imageMemoryStream = new MemoryStream(displaying_frame);
                            Image imgFromStream = Image.FromStream(imageMemoryStream);
                            Image imgParameter = Image.FromStream(imageMemoryStream);
                            pictureBox.Image = imgFromStream;

                            Bitmap imgClone = new Bitmap(imageMemoryStream);
                            
                            videoSaver.AddFrameToVideo(imgClone);
                            
                        }
                        else if (Frames[last_index - 1].is_render_able(Frames[last_index - 1].chunks_avaible))
                        {
                            bool temp = Frames[last_index - 1].is_render_able(Frames[last_index - 1].chunks_avaible);
                            Console.WriteLine(temp);

                            displaying_frame = Frames[last_index - 1].GetFrameOrder();
                            MemoryStream imageMemoryStream = new MemoryStream(displaying_frame);
                            Image imgParameter = Image.FromStream(imageMemoryStream);
                            Image imgFromStream = Image.FromStream(imageMemoryStream);
                            pictureBox.Image = imgFromStream;

                            Bitmap bmpImage = (Bitmap)Image.FromStream(imageMemoryStream);
                            
                            videoSaver.AddFrameToVideo(bmpImage);



                        }
                        else
                        {
                            //son 2 frame patlak
                        }
                    }
                    else
                    {

                        //telemetry datas
                    }
                    #endregion
                    
                    Client.BeginReceive(new AsyncCallback(Receive), null);
                }
            }

            catch (Exception ex)
            {
                //MessageBox.Show(ex.Message);
                Client.BeginReceive(new AsyncCallback(Receive), null);
                //lastframe = 0;
                //last_index = 0;

            }


        }

    }
}


