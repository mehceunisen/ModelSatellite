using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Forms;


namespace TelemetryModelSatellite.source
{

    class DataLogger
    {
        static UInt16 lastNumber;
        public DataLogger()
        {
            lastNumber = 0;
            AddCaptions();
            
        }
        static StreamWriter streamWriter = new StreamWriter(@"..\TelemetryDataLogs.csv");

        public static void LogDataAsync()
        {
            
            while(lastNumber != PACKET.packetNumber)
            {
                streamWriter.WriteLine();
                lastNumber++;
            }
            
            streamWriter.WriteLine(PACKET.teamNumber + "," + PACKET.packetNumber.ToString() + "," + PACKET.transmitTime.ToString() + "," +
            PACKET.pressure.ToString() + "," + PACKET.height.ToString() + "," + PACKET.speed.ToString() + "," + PACKET.temperature.ToString() + "," + PACKET.batteryPercentage.ToString() + "," +
            PACKET.gpsLatitude.ToString() + "," + PACKET.gpsLongitude.ToString() + "," + PACKET.gpsAltitude.ToString() + "," + PACKET.satelliteState.ToString() + "," + PACKET.pitch.ToString() + "," +
            PACKET.roll.ToString() + "," + PACKET.yaw.ToString() + "," + PACKET.mevlanaCount.ToString() + "," + PACKET.didFtpTransfered.ToString());
        }

        private void AddCaptions()
        {

            streamWriter.WriteLine("Team Number," + "Packet Number," + "Receive Date,"
                + "Pressure," + "Height," + "Velocity," + "Temperature," + "Battery,"
                + "Gps Lat," + "Gps Long," + "Gps Alt," + "Pitch," + "Roll,"
                + "Yaw," + "Rotation," + "Did Folder Transfered,");
        }
        
        public void StopLogging()
        {
            streamWriter.Close();
            streamWriter.Dispose();

        }
    }
}
