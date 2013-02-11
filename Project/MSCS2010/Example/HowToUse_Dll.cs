// MediaInfoDLL - All info about media files
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Microsoft Visual C# example
//
// To make this example working, you must put MediaInfo.Dll and Example.ogg
// in the "./Bin/__ConfigurationName__" folder
// and add MediaInfoDll.cs to your project
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Text;
using System.IO;
using MediaInfoLib;

namespace MediaInfoLib_MSCS
{
    /// <summary>
    /// Summary description for Form1.
    /// </summary>
    public class Form1 : System.Windows.Forms.Form
    {
        private System.Windows.Forms.RichTextBox richTextBox1;
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.Container components = null;

        public Form1()
        {
            //
            // Required for Windows Form Designer support
            //
            InitializeComponent();

            //
            // TODO: Add any constructor code after InitializeComponent call
            //
        }

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        protected override void Dispose( bool disposing )
        {
            if( disposing )
            {
                if (components != null)
                {
                    components.Dispose();
                }
            }
            base.Dispose( disposing );
        }

        #region Windows Form Designer generated code
        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.SuspendLayout();
            //
            // richTextBox1
            //
            this.richTextBox1.Location = new System.Drawing.Point(0, 0);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(768, 512);
            this.richTextBox1.TabIndex = 0;
            this.richTextBox1.Text = "";
            //
            // Form1
            //
            this.ClientSize = new System.Drawing.Size(770, 514);
            this.Controls.Add(this.richTextBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "How to use MediaInfo.Dll";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }
        #endregion

        /// <summary>
        /// The main entry point for the application.
        /// </summary>


        [STAThread]
        static void Main()
        {
            Application.Run(new Form1());
        }


        public enum InitialCrcValue { Zeros, NonZero1 = 0xffff, NonZero2 = 0x1D0F }

        public class Crc16Ccitt {
            const ushort poly = 4129;
            ushort[] table = new ushort[256];
            ushort initialValue = 0;

            public ushort ComputeChecksum(byte[] bytes) {
                ushort crc = this.initialValue;
                for(int i = 0; i < bytes.Length; ++i) {
                    crc = (ushort)((crc << 8) ^ table[((crc >> 8) ^ (0xff & bytes[i]))]);
                }
                return crc;
            }

            public byte[] ComputeChecksumBytes(byte[] bytes) {
                ushort crc = ComputeChecksum(bytes);
                return BitConverter.GetBytes(crc);
            }

            public Crc16Ccitt(InitialCrcValue initialValue) {
                this.initialValue = (ushort)initialValue;
                ushort temp, a;
                for(int i = 0; i < table.Length; ++i) {
                    temp = 0;
                    a = (ushort)(i << 8);
                    for(int j = 0; j < 8; ++j) {
                        if(((temp ^ a) & 0x8000) != 0) {
                            temp = (ushort)((temp << 1) ^ poly);
                        } else {
                            temp <<= 1;
                        }
                        a <<= 1;
                    }
                    table[i] = temp;
                }
            }
        }

        private void Form1_Load(object sender, System.EventArgs e)
        {
            //Test if version of DLL is compatible : 3rd argument is "version of DLL tested;Your application name;Your application version"
            String ToDisplay;
            MediaInfo MI = new MediaInfo();

            ToDisplay = MI.Option("Info_Version", "0.7.0.0;MediaInfoDLL_Example_CS;0.7.0.0");
            if (ToDisplay.Length == 0)
            {
                richTextBox1.Text = "MediaInfo.Dll: this version of the DLL is not compatible";
                return;
            }

            //Information about MediaInfo
            ToDisplay += "\r\n\r\nInfo_Parameters\r\n";
            ToDisplay += MI.Option("Info_Parameters");

            ToDisplay += "\r\n\r\nInfo_Capacities\r\n";
            ToDisplay += MI.Option("Info_Capacities");

            ToDisplay += "\r\n\r\nInfo_Codecs\r\n";
            ToDisplay += MI.Option("Info_Codecs");

            //An example of how to use the library
            ToDisplay += "\r\n\r\nOpen\r\n";
            MI.Open("Example.ogg");

            ToDisplay += "\r\n\r\nInform with Complete=false\r\n";
            MI.Option("Complete");
            ToDisplay += MI.Inform();

            ToDisplay += "\r\n\r\nInform with Complete=true\r\n";
            MI.Option("Complete", "1");
            ToDisplay += MI.Inform();

            ToDisplay += "\r\n\r\nCustom Inform\r\n";
            MI.Option("Inform", "General;File size is %FileSize% bytes");
            ToDisplay += MI.Inform();

            ToDisplay += "\r\n\r\nGet with Stream=General and Parameter='FileSize'\r\n";
            ToDisplay += MI.Get(0, 0, "FileSize");

            ToDisplay += "\r\n\r\nGet with Stream=General and Parameter=46\r\n";
            ToDisplay += MI.Get(0, 0, 46);

            ToDisplay += "\r\n\r\nCount_Get with StreamKind=Stream_Audio\r\n";
            ToDisplay += MI.Count_Get(StreamKind.Audio);

            ToDisplay += "\r\n\r\nGet with Stream=General and Parameter='AudioCount'\r\n";
            ToDisplay += MI.Get(StreamKind.General, 0, "AudioCount");

            ToDisplay += "\r\n\r\nGet with Stream=Audio and Parameter='StreamCount'\r\n";
            ToDisplay += MI.Get(StreamKind.Audio, 0, "StreamCount");

            ToDisplay += "\r\n\r\nClose\r\n";
            MI.Close();

            //Example with a stream
            //ToDisplay+="\r\n"+ExampleWithStream()+"\r\n";

            Crc16Ccitt A=new Crc16Ccitt(InitialCrcValue.Zeros);
            byte[] B = { 0x84, 0x00, 0x00, 0x00, 0x6B, 0x00, 0x00, 0x00, 0x67, 0x1F, 0x20, 0x00, 0x00, 0x62, 0x0C, 0x9B, 0x35, 0x35, 0x38, 0x3B, 0x34, 0x31, 0x36, 0x20, 0x56, 0x9B, 0x38, 0x31, 0x3B, 0x33, 0x32, 0x20, 0x5F, 0x90, 0x6F, 0x90, 0x20, 0x41, 0x90, 0x7E, 0x9B, 0x31, 0x3B, 0x30, 0x30, 0x30, 0x30, 0x20, 0x63, 0x9B, 0x30, 0x20, 0x58, 0x1C, 0x40, 0x40, 0x1C, 0x46, 0x40, 0x90, 0x20, 0x40, 0x90, 0x50, 0xC9, 0xB3, 0xC7, 0x89, 0x20, 0x1D, 0x61, 0x1B, 0x7E, 0xC0, 0xA4, 0xE9, 0xF3, 0xC1, 0xF3, 0x8A, 0x24, 0x72, 0x21, 0x29, 0x88, 0x1C, 0x4F, 0x41, 0x89, 0xE1, 0xAD, 0xB7, 0xB3, 0x8A, 0x21, 0x29, 0x89, 0x20, 0x8A, 0x21, 0x44, 0x89, 0xA4, 0xF3, 0xBF, 0xF9, 0xCD, 0xC3, 0xC8, 0x8A, 0x21, 0x29 };
            ushort C=A.ComputeChecksum(B);

            //Displaying the text
            richTextBox1.Text = ToDisplay;
        }

        String ExampleWithStream()
        {
            //Initilaizing MediaInfo
            MediaInfo MI = new MediaInfo();

            //From: preparing an example file for reading
            FileStream From = new FileStream("Example.ogg", FileMode.Open, FileAccess.Read);

            //From: preparing a memory buffer for reading
            byte[] From_Buffer = new byte[64*1024];
            int    From_Buffer_Size; //The size of the read file buffer

            //Preparing to fill MediaInfo with a buffer
            MI.Open_Buffer_Init(From.Length, 0);

            //The parsing loop
            do
            {
                //Reading data somewhere, do what you want for this.
                From_Buffer_Size=From.Read(From_Buffer, 0, 64*1024);

                //Sending the buffer to MediaInfo
                System.Runtime.InteropServices.GCHandle GC = System.Runtime.InteropServices.GCHandle.Alloc(From_Buffer, System.Runtime.InteropServices.GCHandleType.Pinned);
                IntPtr From_Buffer_IntPtr = GC.AddrOfPinnedObject();
                if (MI.Open_Buffer_Continue(From_Buffer_IntPtr, (IntPtr)From_Buffer_Size) == 0) //Note: How to provide a buffer[]?
                {
                    GC.Free();
                    break;
                }
                GC.Free();

                //Testing if MediaInfo request to go elsewhere
                if (MI.Open_Buffer_Continue_GoTo_Get()!=-1)
                {
                    Int64 Position=From.Seek(MI.Open_Buffer_Continue_GoTo_Get(), SeekOrigin.Begin); //Position the file
                    MI.Open_Buffer_Init(From.Length, Position); //Informing MediaInfo we have seek
                }
            }
            while (From_Buffer_Size>0);

            //Finalizing
            MI.Open_Buffer_Finalize(); //This is the end of the stream, MediaInfo must finnish some work

            //Get() example
            return MI.Get(StreamKind.General, 0, "Format");
        }
    }
}
