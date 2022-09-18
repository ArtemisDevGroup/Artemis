using System;
using System.Threading;

using Artemis.Loader.Helper;

namespace Artemis.Loader.Core
{
    interface ITrayIcon : IBaseTrayIcon
    {
        public IntPtr InstanceHandle { get; set; }
        public IntPtr HelperWindowHandle { get; set; }
        public IntPtr NotifyIconDataPtr { get; set; }
        public IntPtr WndClassExPtr { get; set; }
        public bool IsValidInstance { get; set; }

        public abstract void TrayIcon_OnNotifyIcon();

        public void InvokeThread(object? obj)
        {
            while (IsValidInstance)
            {
                if (IsInterrupt())
                {
                    TrayIcon_OnNotifyIcon();
                    ResetInterrupt();
                }
            }
        }

        public void Initialize()
        {
            InstanceHandle = TrayIconStatic.GetModuleInstanceHandle();
            WndClassExPtr = TrayIconStatic.RegisterClass(InstanceHandle);
            HelperWindowHandle = TrayIconStatic.CreateWindow(InstanceHandle, WndClassExPtr);
            NotifyIconDataPtr = TrayIconStatic.RegisterNotifyIconData(InstanceHandle, HelperWindowHandle);

            IsValidInstance = true;
            new Thread(InvokeThread).Start();
        }

        public bool ShowIcon() { return TrayIconStatic.NotifyIcon(TrayIconStatic.NotifyIconFlags.Add, NotifyIconDataPtr); }
        public bool HideIcon() { return TrayIconStatic.NotifyIcon(TrayIconStatic.NotifyIconFlags.Remove, NotifyIconDataPtr); }

        public void Release()
        {
            IsValidInstance = false;
            TrayIconStatic.Release(HelperWindowHandle, WndClassExPtr, NotifyIconDataPtr);
        }
    }
}
