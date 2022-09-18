using System;
using System.Windows;
using Artemis.Loader.Core;

namespace Artemis.Loader.MVVM.ViewModel
{
    class MainViewModel : ObservableObject
    {
        public RelayCommand HomeViewCommand { get; set; }
        public RelayCommand SettingsViewCommand { get; set; }
        public RelayCommand FeaturesViewCommand { get; set; }
        public RelayCommand ChangeLogViewCommand { get; set; }
        public RelayCommand AboutViewCommand { get; set; }
        public RelayCommand CreditsViewCommand { get; set; }

        public HomeViewModel HomeVM { get; set; }
        public SettingsViewModel SettingsVM { get; set; }
        public FeaturesViewModel FeaturesVM { get; set; }
        public ChangeLogViewModel ChangeLogVM { get; set; }
        public AboutViewModel AboutVM { get; set; }
        public CreditsViewModel CreditsVM { get; set; }

        private object _currentView;

        public object CurrentView
        {
            get { return _currentView; }
            set {
                _currentView = value;
                OnPropertyChanged();
            }
        }

        public MainViewModel()
        {
            HomeVM = new HomeViewModel();
            SettingsVM = new SettingsViewModel();
            FeaturesVM = new FeaturesViewModel();
            ChangeLogVM = new ChangeLogViewModel();
            AboutVM = new AboutViewModel();
            CreditsVM = new CreditsViewModel();

            _currentView = HomeVM;

            HomeViewCommand = new RelayCommand(o =>
            {
                CurrentView = HomeVM;
            });

            SettingsViewCommand = new RelayCommand(o =>
            {
                CurrentView = SettingsVM;
            });

            FeaturesViewCommand = new RelayCommand(o =>
            {
                CurrentView = FeaturesVM;
            });

            ChangeLogViewCommand = new RelayCommand(o =>
            {
                CurrentView = ChangeLogVM;
            });

            AboutViewCommand = new RelayCommand(o =>
            {
                CurrentView = AboutVM;
            });

            CreditsViewCommand = new RelayCommand(o =>
            {
                CurrentView = CreditsVM;
            });
        }
    }
}
