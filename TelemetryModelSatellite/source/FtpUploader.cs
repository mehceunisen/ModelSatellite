using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using FluentFTP;

namespace TelemetryModelSatellite.source
{
    class FtpUploader
    {
        public static RichTextBox consoleTextBox { get; set; }
        public static TextBox ftpTextBox { get; set; }


        #region eski kod
        //public static string ReadWriteFiles(string rootPath, string folderToCreate, string addressToSend, string userName, string password)
        //{


        //    //standart bi dosya ismimin olcak
        //    string defFileName = "ftpfile";

        //    folderToCreate = CreateFolder(folderToCreate, addressToSend, userName, password);


        //    //gonderecegimiz dosyanin icindeki dosyalari okuyoruz
        //    string[] files = Directory.GetFiles(rootPath);
        //    int i = 0;

        //    //Sureyya Hanim'in muhtesem degisimi!! dosyalarin ismi degismis
        //    //"yeni" dosya isimlerini aliyoruz
        //    string[] newFiles = Directory.GetFiles(rootPath);

        //    foreach (var fileName in newFiles)
        //    {
        //        //karsi tarafta standart isimde bir dosya olusturuyoruz
        //        FtpWebRequest ftpFileRequest = (FtpWebRequest)FtpWebRequest.Create(addressToSend + "/" + folderToCreate + "/" + defFileName + i.ToString() + Path.GetExtension(fileName).ToLower());

        //        //ftpFileRequest.UsePassive = true;
        //        //ftpFileRequest.KeepAlive = true;
        //        //ftpFileRequest.UseBinary = true;

        //        ftpFileRequest.Method = WebRequestMethods.Ftp.UploadFile;

        //        ftpFileRequest.Credentials = new NetworkCredential(userName, password);


        //        Stream stream = ftpFileRequest.GetRequestStream();
        //        //dosyayi ac icerde oku
        //        FileStream fileStream = File.OpenRead(fileName);


        //        int byteRead = 1;
        //        byte[] buffer = new byte[1024];

        //        while (byteRead > 0)
        //        {
        //            //OKU YARATAN RABBIN ADIYLA OKU
        //            byteRead = fileStream.Read(buffer, 0, 1024);
        //            stream.Write(buffer, 0, 1024);
        //        }
        //        i++;
        //    }


        //    return folderToCreate;
        //}

        //public static int j = 1;
        //private static string CreateFolder(string folderToCreate, string addressToSend, string userName, string password)
        //{
        //    bool didFolderCreated = false;
        //    string defFolderName = "ftpfolder";

        //    Stream ftpFolderStream;
        //    FtpWebRequest ftpFolderRequest;
        //    FtpWebResponse response;



        //    while (!didFolderCreated && j <= 10)
        //    {

        //        try
        //        {
        //            folderToCreate = defFolderName + j.ToString();
        //            //client'e soruyoruz bi cay icmek isterlerse kendilerine bi dosya acıcaz diyoruz
        //            ftpFolderRequest = (FtpWebRequest)FtpWebRequest.Create(addressToSend + "/" + folderToCreate);

        //            ftpFolderRequest.UsePassive = true;
        //            ftpFolderRequest.KeepAlive = true;
        //            ftpFolderRequest.UseBinary = true;

        //            //client da bize karsi bos degilmis ki dosyayi aciyor
        //            ftpFolderRequest.Method = WebRequestMethods.Ftp.MakeDirectory;
        //            //ben buyum diyoruz
        //            ftpFolderRequest.Credentials = new NetworkCredential(userName, password);

        //            response = (FtpWebResponse)ftpFolderRequest.GetResponse();
        //            ftpFolderStream = response.GetResponseStream();
        //            ftpFolderStream.Close();
        //            response.Close();
        //            didFolderCreated = true;
        //        }
        //        catch (Exception)
        //        { }
        //        j++;
        //    }
        //    didFolderCreated = false;
        //    j = 1;
        //    return folderToCreate;
        //}

        //public async void TransferFolder(FolderBrowserDialog fbd)
        //{
        //    await Task.Run(() =>
        //    {
        //        try
        //        {
        //            string folderCreated = ReadWriteFiles(fbd.SelectedPath, "ftpfolder",
        //            "ftp://" + MACROS.ESP_IP + ":" + MACROS.ESP_FTP_PORT.ToString(),
        //            MACROS.ESP_FTP_USER_ID, MACROS.ESP_FTP_PASSWORD);

        //            consoleTextBox.Text += "\n" + DateTime.Now.ToShortTimeString() + " " + folderCreated + " Created Succesfully";
        //            consoleTextBox.Text += "\nFile Transfered Succesfully";
        //        }
        //        catch (Exception)
        //        {
        //            consoleTextBox.Text += "\n" + DateTime.Now.ToShortTimeString() + " File Couldn't Transfered";
        //        }
        //    });

        //}
        #endregion

        FtpClient client = new FtpClient(MACROS.ESP_IP, new NetworkCredential(MACROS.ESP_FTP_USER_ID, MACROS.ESP_FTP_PASSWORD));

        public void SendFileFtpServer(string localFile)
        {
            try
            {
                client.AutoConnect();
                client.DataConnectionType = FtpDataConnectionType.PASV;
                client.EncryptionMode = FtpEncryptionMode.None;
                if (!client.DirectoryExists(@"/ftp"))
                {
                    client.CreateDirectory(@"/ftp");
                }
            }
            catch (Exception ex)
            {
                consoleTextBox.Text = "\n" + DateTime.Now.ToShortTimeString() + "You are already connected to the server";
            }
            client.TransferChunkSize = 2048;

            byte[] readBytes = File.ReadAllBytes(localFile);
            for (int i = 0; i < 20; i++)
            {
                if (client.FileExists(@"/ftp/vid" + i.ToString() + MACROS.videoExtension))
                {
                    i++;
                }
                else
                {
                    client.Upload(readBytes, @"/ftp/vid" + i.ToString() + MACROS.videoExtension);
                    break;
                }
            }
            
            consoleTextBox.Text += "\n" + DateTime.Now.ToShortTimeString() + "File transfered succesfully";
        }
            
    }
}
 