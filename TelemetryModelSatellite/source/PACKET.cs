using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UINT8 = System.Byte;
using INT8 = System.SByte;

namespace TelemetryModelSatellite.source
{
    class PACKET
    {
        
        //Direkt Uydudan gelmesi gerekenler Yarışma:

        public static UInt16 teamNumber { get; set; }

        //esp halleder
        public static string transmitTime { get; set; }

        public static UInt16 packetNumber { get; set; }

        public static float pressure { get; set; }
        public static float height { get; set; }

        //çarpanlı yollamak 
        //uint16 to float
        public static float speed { get; set; }

        public static float temperature { get; set; }

        public static float batteryPercentage { get; set; }

        public static double gpsLatitude { get; set; }
        public static double gpsLongitude { get; set; }
        public static double gpsAltitude { get; set; }

        public static UInt16 satelliteState { get; set; }
        // 16 tane boolean value
        public static float pitch { get; set; }
        public static float roll { get; set; }
        public static float yaw { get; set; }

        
        public static UInt16 mevlanaCount { get; set; }

        //video aktarım bilgisi eprom dan okuyarak 
        public static UInt16 didFtpTransfered { get; set; }

        public static UInt16 enginePower { get; set; }

        public static INT8 rssiValue { get; set; }

        public static byte[] uartReceiveStartCmd { get; } = { 10, 71, 30, 1, 5 };
        public static byte[] uartReceiveStopCmd { get; } = { 10, 71, 30, 1, 13 };

        public static byte[] dataProcessStartCmd { get; } = { 10, 71, 30, 2, 5 };
        public static byte[] dataProcessStopCmd { get; } = { 10, 71, 30, 2, 13 };

        public static byte[] telemetryTransmitStartCmd { get; } = { 10, 71, 30, 3, 5 };
        public static byte[] telemetryTransmitStopCmd { get; } = { 10, 71, 30, 3, 13 };

        //public static byte[] commandListenStartCmd { get; } = { 10, 71, 30, 4, 5 };
        //public static byte[] commandListenStopCmd { get; } = { 10, 71, 30, 4, 13 };

        public static byte[] videoStreamStartCmd { get; } = { 10, 71, 30, 4, 5 };
        public static byte[] videoStreamStopCmd { get; } = { 10, 71, 30, 4, 13 };

        public static byte[] videoRecorderStartCmd { get; } = { 10, 71, 30, 5, 5 };
        public static byte[] videoRecorderStopCmd { get; } = { 10, 71, 30, 5, 13 };

        public static byte[] ftpTaskStartCmd { get; } = { 10, 71, 30, 6, 5 };
        public static byte[] ftpTaskStopCmd { get; } = { 10, 71, 30, 6, 13 };

        public static bool uartTaskState = true;
        public static bool dataProcessState = true;
        public static bool telemetryTaskState = true;
        public static bool videoStreamState = true;
        public static bool videoRecorderState = true;
        public static bool ftpTaskState = true;

    }
}
