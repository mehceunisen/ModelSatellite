using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TelemetryModelSatellite.source
{
    public struct MACROS
    {
        public const int CHUNK_SIZE = 1005;
        public const byte HEADER_SIZE = 5;
        public readonly static int ESP_UDP_PORT_NUMBER = 8888;
        public static string ESP_IP { get; set; }
        public readonly static int ESP_UDP_RECEIVE_PORT = 8080;
        public readonly static int ESP_TCP_TRANSMIT_PORT = 3333; 
        public readonly static int ESP_FTP_PORT = 21;
        public readonly static string ESP_FTP_USER_ID = "digital";
        public readonly static string ESP_FTP_PASSWORD = "aesk";
        public static string telemetryPath { get; set; }
        public static string videoPath { get; set; }
        public static string videoExtension { get; set; }
    }
}
