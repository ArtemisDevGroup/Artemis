using System;
using System.Diagnostics;
using System.IO;

namespace Artemis.Loader.Core
{
    public static class Globals
    {
        public static SettingsFile file = new SettingsFile();

        public static string? GetCurrentDirectory()
        {
            string? file = Process.GetCurrentProcess().Modules[0].FileName;
            return file?.Remove(file.LastIndexOf('\\'));
        }

        public static string CurrentUsername { get; set; }
    }
}
