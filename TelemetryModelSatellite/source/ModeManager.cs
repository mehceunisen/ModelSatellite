using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TelemetryModelSatellite.source
{
    class ModeManager
    {
        public static Form1 form1 { get; set; }
        public static TableLayoutPanel leftPanel { get; set; }
        public static List<Button> buttons { get; set; }

        public static void SwitchBrightMode()
        {
            form1.BackColor = Color.FromArgb(150, 190, 200);
            leftPanel.BackColor = Color.FromArgb(40, 80, 90);
            foreach (var button in buttons)
            {
                button.BackColor = Color.FromArgb(40, 80, 90);
            }

        }
        
        public static void SwitchDarkMode()
        {

        }
    }
}
