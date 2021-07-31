using GMap.NET;
using GMap.NET.MapProviders;
using GMap.NET.WindowsForms;
using GMap.NET.WindowsForms.Markers;
using System.Threading.Tasks;

namespace TelemetryModelSatellite.source
{
    class GmapController
    {
        static GMapControl _gMap;
        public GmapController(GMapControl gMapControl)
        {
            _gMap = gMapControl;
        }


        public void InitGmap(double lat, double longt)
        {
            _gMap.ShowCenter = false;
            _gMap.MapProvider = GMapProviders.BingSatelliteMap;
            _gMap.Position = new PointLatLng(lat, longt);
            _gMap.MinZoom = 10;
            _gMap.MaxZoom = 20;
            _gMap.Zoom = 18;

        }

        public static async void UpdateGmapAsync(double lat, double longt)
        {
            await Task.Run(() =>
            {
                _gMap.Position = new PointLatLng(lat, longt);
            });
        }

        public void PinLocation(PointLatLng mousePos)
        {
            PointLatLng receivedPosition = new PointLatLng(mousePos.Lat, mousePos.Lng);   // burdaki mouse position yrine gelen verileri yaz gps takip etsin.

            GMapMarker marker = new GMarkerGoogle(receivedPosition, GMarkerGoogleType.red_small);
            GMapOverlay markers = new GMapOverlay("markers");

            markers.Markers.Add(marker);
            _gMap.Overlays.Add(markers);
            _gMap.Zoom += 0.0000001;
            _gMap.Zoom -= 0.0000001;
            
        }
        
        public void UnpinLocation(PointLatLng mousePos)
        {
            PointLatLng receivedPosition = new PointLatLng(mousePos.Lat, mousePos.Lng);
            GMapMarker marker = new GMarkerGoogle(receivedPosition, GMarkerGoogleType.red_small);
            GMapOverlay markers = new GMapOverlay("markers");

            markers.Markers.Remove(marker);
            _gMap.Overlays.Remove(markers);
            _gMap.Zoom += 0.0000001;
            _gMap.Zoom -= 0.0000001;
        }
    }
}
