using Accord.Video.FFMPEG;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TelemetryModelSatellite.source
{
    class VideoSaver
    {
        
        VideoFileWriter videoWriter = new VideoFileWriter();
        int fps = 60;


        public VideoSaver()
        {
            CreateVideoFile();
        }
        
        private void CreateVideoFile()
        {
            videoWriter.Open(@"..\PayloadVideoRec.avi", 400, 296, fps, VideoCodec.H264);
        }
        
        public void AddFrameToVideo(Bitmap imgFromBitmap)
        {

            videoWriter.WriteVideoFrame(imgFromBitmap);
            
        }

        public void StopVideoRecord()
        {
            videoWriter.Close();
        }
        
    
    }
}
