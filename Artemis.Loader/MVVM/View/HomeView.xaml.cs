using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Interop;

namespace Artemis.Loader.MVVM.View
{
    /// <summary>
    /// Interaction logic for HomeView.xaml
    /// </summary>
    public partial class HomeView : UserControl
    {
        public HomeView()
        {
            InitializeComponent();
        }

        private void StartGameAndInjectThread(object? obj)
        {
            Process.Start(Core.Globals.file.GameDir + @"\LumaPlay_x64.exe");
            Thread.Sleep(1000);
            uint result;
            if ((result = Helper.Helpers.Inject()) != 0)
            {
                MessageBox.Show($"{result}");
            }
        }

        private void OnClick_Start(object sender, RoutedEventArgs e)
        {
            if (File.Exists(Core.Globals.file.GameDir + @"\LumaPlay_x64.exe"))
            {
                if (File.Exists(Core.Globals.GetCurrentDirectory() + @"\Artemis.dll"))
                {
                    Thread t = new Thread(StartGameAndInjectThread);
                    t.Start();
                }
                else
                {
                    MessageBox.Show("Artemis.dll missing in directory.");
                }
            }
            else
            {
                MessageBox.Show("Invalid game directory path. Set your game directory in the settings tab.");
            }
        }
    }
}
