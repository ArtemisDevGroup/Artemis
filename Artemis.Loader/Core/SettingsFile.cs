using System;
using System.IO;

namespace Artemis.Loader.Core
{
    public class SettingsFile
    {
        private const string c_StringPath = "Artemis_Settings.ats";

        private string _gameDir;

        public string GameDir
        {
            get { return _gameDir; }
            set
            {
                _gameDir = value;

                using (FileStream file = new FileStream(c_StringPath, FileMode.Create))
                {
                    using (StreamWriter sw = new StreamWriter(file))
                    {
                        sw.Write(_gameDir);
                        sw.Close();
                    }

                    file.Close();
                }
                    
            }
        }

        public SettingsFile()
        {
            if (!File.Exists(c_StringPath))
            {
                _gameDir = "";
            }
            else
            {
                string? result;
                using (FileStream file = new FileStream(c_StringPath, FileMode.Open))
                {
                    using (StreamReader sr = new StreamReader(file))
                    {
                        result = sr.ReadLine();
                        sr.Close();
                    }
                    file.Close();
                }

                if (result != null) _gameDir = result;
                else _gameDir = "";
            }
        }
    }
}
