using System;
using System.Windows;
using System.Collections.Generic;
using System.IO;
using System.Threading;
using System.Linq;

using Artemis.Loader.Core;

namespace Artemis.Loader.MVVM.ViewModel
{
    class SettingsViewModel : ObservableObject
    {
        public RelayCommand SetUsernameCommand { get; set; }

        public RelayCommand NavigateGamePathCommand { get; set; }
        public RelayCommand ResetGamePathCommand { get; set; }

        public string GameDirectory
        {
            get { return Globals.file.GameDir; }
            set {
                Globals.file.GameDir = value;
                OnPropertyChanged();
            }
        }

        private string _userName;

        public string Username
        {
            get { return _userName; }
            set {
                _userName = value;
                OnPropertyChanged();
            }
        }

        public SettingsViewModel()
        {
            GameDirectory = Globals.file.GameDir;
            Username = Globals.CurrentUsername;

            SetUsernameCommand = new RelayCommand(o =>
            {
                if (File.Exists(GameDirectory + @"\LumaPlayFiles\LumaPlay.ini"))
                {
                    using (FileStream fs = new FileStream(GameDirectory + @"\LumaPlayFiles\LumaPlay.ini", FileMode.Create))
                    {
                        using (StreamWriter sw = new StreamWriter(fs))
                        {
                            sw.Write($"[LumaPlay]\nGameExe = RainbowSix.exe /belaunch -be\nPlayerName = {Username}\nOnlyOwnThisGame = 1\nUnlockDLC = 0\nOfflineMode = 1\nen-US\nLanguage = en-US\nAccountID = 11111111-1111-1111-1111-111111111111\nCDkey = 1111-1111-1111-1111\nEmail = LumaPlay@LumaPlay.com\nPassword = LumaPlay\nTicket = LumaPlay\n[DLC]\nDLC_ID = 14633\n[Chunks]\nNumChunks = 0\n[Achievements]\nGenerateAchievementReportOnExit = 0\nNumberOfAchievements = 2\nUplayGameID = 1843\nAchievement_1 = Test1\nAchievement_2 = Test2\n[Log]\nLogFile = 1\n[UplayConnection]\nAchievements = 0\nAvatar = 0\nChat = 0\nFriends = 0\nOverlay = 0\nParty = 0\nWin = 0\nUser = 0\nOptions = 1\nStore = 1\n");
                            sw.Close();
                        }
                        fs.Close();
                    }

                    Globals.CurrentUsername = Username;
                }
                else MessageBox.Show("LumaPlay.ini not found.");
            });

            NavigateGamePathCommand = new RelayCommand(o =>
            {
                using (System.Windows.Forms.FolderBrowserDialog dialog = new())
                {
                    System.Windows.Forms.DialogResult result = dialog.ShowDialog();

                    if (result == System.Windows.Forms.DialogResult.OK)
                    {
                        if (File.Exists(dialog.SelectedPath + @"\LumaPlay_x64.exe")) GameDirectory = dialog.SelectedPath;
                        else MessageBox.Show($"Path {dialog.SelectedPath} does not contain the game.");
                    }
                }
            });

            ResetGamePathCommand = new RelayCommand(o => { GameDirectory = ""; });
        }
    }
}
