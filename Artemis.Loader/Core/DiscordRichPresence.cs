using System;
using System.Threading;
using DiscordRPC;

namespace Artemis.Loader.Core
{
    class DiscordRichPresence
    {
        private static string GetStateString(byte value)
        {
            switch (value)
            {
                case 0: return "";
                case 1: return "";
                default: return "";
            }
        }

        public void RpcThread(object? obj)
        {
            DiscordRpcClient _client = new DiscordRpcClient("1004050413517017239");
            _client.Initialize();

            Helper.PipeServer pipe = new Helper.PipeServer();

            Assets a = new Assets();
            a.LargeImageKey = "just_logo";
            a.LargeImageText = "Artemis";

            byte[] buf;

            _client.SetPresence(new RichPresence()
                    .WithDetails("Playing Shadow Legacy")
                    .WithState("In Loader")
                    .WithAssets(a));
            _client.Invoke();

            try
            {
                pipe.WaitForConnection();

                while (true)
                {
                    buf = new byte[16];

                    // Read pipe.
                    pipe.Read(buf);

                    if (buf[0] == 0) break;

                    _client.SetPresence(new RichPresence()
                    .WithDetails("Playing Shadow Legacy")
                    .WithState(GetStateString(buf[0]))
                    .WithAssets(a));
                    _client.Invoke();
                }
            }
            catch (ThreadAbortException)
            {
                _client.Dispose();
                pipe.Release();
            }
        }

        public DiscordRichPresence()
        {
            new Thread(RpcThread).Start();
        }
    }
}
