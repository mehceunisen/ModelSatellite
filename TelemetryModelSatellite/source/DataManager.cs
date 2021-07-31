using OpenTK;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace TelemetryModelSatellite.source
{
    class DataManager
    {
        
        public static List<Label> labels { get; set; }
        public static List<Chart> charts { get; set; }
        
        private static void DecodeBuffer(byte[] receivedBuffer, ref int startingIndex)
        {
            PACKET.teamNumber =         BufferConverter.ConvertToUInt16(receivedBuffer, ref startingIndex);
            PACKET.packetNumber =       BufferConverter.ConvertToUInt16(receivedBuffer, ref startingIndex);
            PACKET.transmitTime =       BufferConverter.ConvertU32ToDateTime(receivedBuffer, ref startingIndex);
            PACKET.pressure =           BufferConverter.ConvertToUInt32(receivedBuffer, ref startingIndex);
            PACKET.height =             BufferConverter.ConvertU16ToFloat(receivedBuffer, ref startingIndex) / 10;
            PACKET.speed =              BufferConverter.ConvertU16ToFloat(receivedBuffer, ref startingIndex) / 100;
            PACKET.temperature =        BufferConverter.ConvertU16ToFloat(receivedBuffer, ref startingIndex);
            PACKET.batteryPercentage =  BufferConverter.ConvertU16ToFloat(receivedBuffer, ref startingIndex) / 20;
            PACKET.gpsLatitude =        BufferConverter.ConvertU32ToDouble(receivedBuffer, ref startingIndex) / 1_000_000;
            PACKET.gpsLongitude =       BufferConverter.ConvertU32ToDouble(receivedBuffer, ref startingIndex) / 1_000_000;
            PACKET.gpsAltitude =        BufferConverter.ConvertU32ToDouble(receivedBuffer, ref startingIndex) / 1_000_000;
            PACKET.satelliteState =     BufferConverter.ConvertToUInt16(receivedBuffer, ref startingIndex);
            PACKET.pitch =              BufferConverter.ConvertU16ToFloat(receivedBuffer, ref startingIndex) / 10 - 90;
            PACKET.roll =               BufferConverter.ConvertU16ToFloat(receivedBuffer, ref startingIndex) / 10 - 90; 
            PACKET.yaw =                BufferConverter.ConvertU16ToFloat(receivedBuffer, ref startingIndex) / 10 - 90;
            PACKET.mevlanaCount =       BufferConverter.ConvertToUInt16(receivedBuffer, ref startingIndex);
            PACKET.enginePower =        BufferConverter.ConvertToUInt16(receivedBuffer, ref startingIndex);
            PACKET.rssiValue =          BufferConverter.ConvertToInt8(receivedBuffer, ref startingIndex);
        }


        private static async void UpdateFrontAsync()
        {
            await Task.Run(() =>
           {
               labels[0].Text =  PACKET.teamNumber.ToString();
               labels[1].Text =  PACKET.transmitTime;
               labels[2].Text =  PACKET.packetNumber.ToString();
               labels[3].Text =  PACKET.pressure.ToString() + " Pa";
               labels[4].Text =  PACKET.height.ToString() + " m";
               labels[5].Text =  PACKET.speed.ToString() + " m/s";
               labels[6].Text =  PACKET.temperature.ToString() + "C";
               labels[7].Text =  PACKET.batteryPercentage.ToString() + "V";
               labels[8].Text =  PACKET.gpsLatitude.ToString();
               labels[9].Text =  PACKET.gpsLongitude.ToString();
               labels[10].Text = PACKET.gpsAltitude.ToString(); 
               labels[11].Text = PACKET.mevlanaCount.ToString();
               labels[12].Text = PACKET.didFtpTransfered.ToString();
               labels[13].Text = PACKET.satelliteState.ToString();
               labels[14].Text = PACKET.pitch.ToString() + " °";
               labels[15].Text = PACKET.roll.ToString() + " °";
               labels[16].Text = PACKET.yaw.ToString() + " °";
               labels[18].Text = (PACKET.rssiValue + 100).ToString() + "dB";
               
               charts[1].Series["Pitch"].Points.AddXY(PACKET.transmitTime, PACKET.pitch);
               charts[1].Series["Roll"].Points.AddXY(PACKET.transmitTime, PACKET.roll);
               charts[1].Series["Yaw"].Points.AddXY(PACKET.transmitTime, PACKET.yaw);

               charts[0].Series["H"].Points.AddXY(PACKET.transmitTime, PACKET.height);
               charts[0].Series["Alt"].Points.AddXY(PACKET.transmitTime, PACKET.gpsAltitude);
           });
        }


        public static void UpdateAllDataAsync(byte[] receivedBuffer, ref int strartingIndex)
        {
            DecodeBuffer(receivedBuffer, ref strartingIndex);
            GlController.RedrawGlControlAsync(PACKET.yaw, PACKET.pitch, PACKET.roll);
            GmapController.UpdateGmapAsync(PACKET.gpsLatitude, PACKET.gpsLongitude);
            DataLogger.LogDataAsync();
            UpdateFrontAsync();
        }

    }
}
