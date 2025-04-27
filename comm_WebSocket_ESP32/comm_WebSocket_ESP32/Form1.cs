using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using WebSocketSharp;

namespace comm_WebSocket_ESP32
{
    public partial class Form1 : Form
    {
        WebSocket nossows;  //nosso websocket
        bool statusLED = false;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            nossows = new WebSocket("ws://192.168.15.7:81");
            nossows.OnMessage += (s, ev) =>
            {
                string mensagem = ev.Data.Trim();
                Invoke((MethodInvoker)delegate
                {
                    if (mensagem == "LED:on")
                    {
                        label1.Text = "LED LIGADO";
                        statusLED = true;
                    }
                    else if(mensagem == "LED:off")
                    {
                        label1.Text = "LED DESLIGADO";
                        statusLED = false;
                    }
                });
            };
            nossows.Connect();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if(nossows != null && nossows.IsAlive)
            {
                nossows.Send("TOGGLE_LED");
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if(nossows != null) { 
                nossows.Close();
            }
        }
    }
}
