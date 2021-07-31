using OpenTK;
using OpenTK.Graphics.OpenGL;
using System;
using System.Drawing;
using System.Threading.Tasks;

namespace TelemetryModelSatellite.source
{
    class GlController
    {
        
        static GLControl glControl;
        public GlController(GLControl _glControl)
        {
            glControl = _glControl;
        }

        static float x = 0, y = 0, z = 0;

        public void GlControlLoad()
        {
            GL.ClearColor(Color.FromArgb(30, 30, 50));
            
        }

        public void PaintGLControl()
        {
            //aesk logosu bas ilerinin argesi
            float step = 1.0f;
            float topla = step;
            float radius = 3.0f;
            float dikey1 = 2 * radius, dikey2 = -2 * radius;
            GL.Clear(ClearBufferMask.ColorBufferBit);
            GL.Clear(ClearBufferMask.DepthBufferBit);

            Matrix4 perspective = Matrix4.CreatePerspectiveFieldOfView(1.04f, 4 / 3, 1, 10000);
            Matrix4 lookat = Matrix4.LookAt(25, 0, 0, 0, 0, 0, 0, 1, 0);
            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadIdentity();
            GL.LoadMatrix(ref perspective);
            GL.MatrixMode(MatrixMode.Modelview);
            GL.LoadIdentity();
            GL.LoadMatrix(ref lookat);
            GL.Viewport(0, 0, glControl.Width, glControl.Height);
            GL.Enable(EnableCap.DepthTest);
            GL.DepthFunc(DepthFunction.Less);

            GL.Rotate(x, 1.0, 0.0, 0.0);//ÖNEMLİ
            GL.Rotate(z, 0.0, 1.0, 0.0);
            GL.Rotate(y, 0.0, 0.0, 1.0);

            silindir(step, topla, radius, 3, -5);
            silindir(0.01f, topla, 0.5f, 9, 9.7f);
            silindir(0.01f, topla, 0.1f, 5, dikey1 + 5);

            Pervane(9f, 11.0f, 0.7f, 0.7f);

            GL.Begin(BeginMode.Lines);

            GL.Color3(Color.FromArgb(250, 0, 0));
            GL.Vertex3(-30.0, 0.0, 0.0);
            GL.Vertex3(30.0, 0.0, 0.0);


            GL.Color3(Color.FromArgb(0, 0, 0));
            GL.Vertex3(0.0, 30.0, 0.0);
            GL.Vertex3(0.0, -30.0, 0.0);

            GL.Color3(Color.FromArgb(0, 0, 250));
            GL.Vertex3(0.0, 0.0, 30.0);
            GL.Vertex3(0.0, 0.0, -30.0);

            GL.End();
            glControl.SwapBuffers();
            
        }

        private void silindir(float step, float topla, float radius, float dikey1, float dikey2)
        {
            float eski_step = 0.1f;
            GL.Begin(BeginMode.Quads);//Y EKSEN CIZIM DAİRENİN
            while (step <= 360)
            {
                if (step < 45)
                    GL.Color3(Color.FromArgb(0, 0, 255));
                else if (step < 90)
                    GL.Color3(Color.FromArgb(250, 250, 250));
                else if (step < 135)
                    GL.Color3(Color.FromArgb(0, 0, 255));
                else if (step < 180)
                    GL.Color3(Color.FromArgb(250, 250, 250));
                else if (step < 225)
                    GL.Color3(Color.FromArgb(0, 0, 255));
                else if (step < 270)
                    GL.Color3(Color.FromArgb(250, 250, 250));
                else if (step < 315)
                    GL.Color3(Color.FromArgb(0, 0, 255));
                else if (step < 360)
                    GL.Color3(Color.FromArgb(250, 250, 250));


                float ciz1_x = (float)(radius * Math.Cos(step * Math.PI / 180F));
                float ciz1_y = (float)(radius * Math.Sin(step * Math.PI / 180F));
                GL.Vertex3(ciz1_x, dikey1, ciz1_y);

                float ciz2_x = (float)(radius * Math.Cos((step + 2) * Math.PI / 180F));
                float ciz2_y = (float)(radius * Math.Sin((step + 2) * Math.PI / 180F));
                GL.Vertex3(ciz2_x, dikey1, ciz2_y);

                GL.Vertex3(ciz1_x, dikey2, ciz1_y);
                GL.Vertex3(ciz2_x, dikey2, ciz2_y);
                step += topla;
            }
            GL.End();
            GL.Begin(BeginMode.Lines);
            step = eski_step;
            topla = step;
            while (step <= 180)// UST KAPAK
            {
                if (step < 45)
                    GL.Color3(Color.FromArgb(0, 0, 255));
                else if (step < 90)
                    GL.Color3(Color.FromArgb(250, 250, 250));
                else if (step < 135)
                    GL.Color3(Color.FromArgb(0, 0, 255));
                else if (step < 180)
                    GL.Color3(Color.FromArgb(250, 250, 250));
                else if (step < 225)
                    GL.Color3(Color.FromArgb(0, 0, 255));
                else if (step < 270)
                    GL.Color3(Color.FromArgb(250, 250, 250));
                else if (step < 315)
                    GL.Color3(Color.FromArgb(0, 0, 255));
                else if (step < 360)
                    GL.Color3(Color.FromArgb(250, 250, 250));


                float ciz1_x = (float)(radius * Math.Cos(step * Math.PI / 180F));
                float ciz1_y = (float)(radius * Math.Sin(step * Math.PI / 180F));
                GL.Vertex3(ciz1_x, dikey1, ciz1_y);

                float ciz2_x = (float)(radius * Math.Cos((step + 180) * Math.PI / 180F));
                float ciz2_y = (float)(radius * Math.Sin((step + 180) * Math.PI / 180F));
                GL.Vertex3(ciz2_x, dikey1, ciz2_y);

                GL.Vertex3(ciz1_x, dikey1, ciz1_y);
                GL.Vertex3(ciz2_x, dikey1, ciz2_y);
                step += topla;
            }
            step = eski_step;
            topla = step;
            while (step <= 180)//ALT KAPAK
            {
                if (step < 45)
                    GL.Color3(Color.FromArgb(0, 0, 255));
                else if (step < 90)
                    GL.Color3(Color.FromArgb(250, 250, 250));
                else if (step < 135)
                    GL.Color3(Color.FromArgb(0, 0, 255));
                else if (step < 180)
                    GL.Color3(Color.FromArgb(250, 250, 250));
                else if (step < 225)
                    GL.Color3(Color.FromArgb(0, 0, 255));
                else if (step < 270)
                    GL.Color3(Color.FromArgb(250, 250, 250));
                else if (step < 315)
                    GL.Color3(Color.FromArgb(0, 0, 255));
                else if (step < 360)
                    GL.Color3(Color.FromArgb(250, 250, 250));

                float ciz1_x = (float)(radius * Math.Cos(step * Math.PI / 180F));
                float ciz1_y = (float)(radius * Math.Sin(step * Math.PI / 180F));
                GL.Vertex3(ciz1_x, dikey2, ciz1_y);

                float ciz2_x = (float)(radius * Math.Cos((step + 180) * Math.PI / 180F));
                float ciz2_y = (float)(radius * Math.Sin((step + 180) * Math.PI / 180F));
                GL.Vertex3(ciz2_x, dikey2, ciz2_y);

                GL.Vertex3(ciz1_x, dikey2, ciz1_y);
                GL.Vertex3(ciz2_x, dikey2, ciz2_y);
                step += topla;
            }
            GL.End();
        }

        private void Pervane(float yukseklik, float uzunluk, float kalinlik, float egiklik)
        {
            float radius = 10, angle = 45.0f;
            GL.Begin(BeginMode.Quads);

            GL.Color3(Color.FromArgb(0, 0, 255));
            GL.Vertex3(uzunluk, yukseklik, kalinlik);
            GL.Vertex3(uzunluk, yukseklik + egiklik, -kalinlik);
            GL.Vertex3(0.0, yukseklik + egiklik, -kalinlik);
            GL.Vertex3(0.0, yukseklik, kalinlik);

            GL.Color3(Color.FromArgb(0, 0, 255));
            GL.Vertex3(-uzunluk, yukseklik + egiklik, kalinlik);
            GL.Vertex3(-uzunluk, yukseklik, -kalinlik);
            GL.Vertex3(0.0, yukseklik, -kalinlik);
            GL.Vertex3(0.0, yukseklik + egiklik, kalinlik);

            GL.Color3(Color.White);
            GL.Vertex3(kalinlik, yukseklik, -uzunluk);
            GL.Vertex3(-kalinlik, yukseklik + egiklik, -uzunluk);
            GL.Vertex3(-kalinlik, yukseklik + egiklik, 0.0);//+
            GL.Vertex3(kalinlik, yukseklik, 0.0);//-

            GL.Color3(Color.White);
            GL.Vertex3(kalinlik, yukseklik + egiklik, +uzunluk);
            GL.Vertex3(-kalinlik, yukseklik, +uzunluk);
            GL.Vertex3(-kalinlik, yukseklik, 0.0);
            GL.Vertex3(kalinlik, yukseklik + egiklik, 0.0);
            GL.End();

        }

        public static async void RedrawGlControlAsync(float _x, float _y, float _z)
        {
            await Task.Run(() =>
           {
               x = _x;
               y = _y;
               z = _z;
               glControl.Invalidate();
           });

        }
    }
}
