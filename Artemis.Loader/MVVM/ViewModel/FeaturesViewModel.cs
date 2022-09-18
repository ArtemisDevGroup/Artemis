using System;
using Artemis.Loader.Core;
using Artemis.Loader.MVVM.ViewModel.FeatuesSubViewModel;

namespace Artemis.Loader.MVVM.ViewModel
{
    class FeaturesViewModel : ObservableObject
    {
        public RelayCommand TerroristViewCommand { get; set; }

        public TerroristViewModel TerroristVM { get; set; }

        private object _currentFeatuesView;

        public object CurrentFeaturesView
        {
            get { return _currentFeatuesView; }
            set {
                _currentFeatuesView = value;
                OnPropertyChanged();
            }
        }

        public FeaturesViewModel()
        {
            TerroristVM = new TerroristViewModel();

            TerroristViewCommand = new RelayCommand(o =>
            {
                CurrentFeaturesView = TerroristVM;
            });

            CurrentFeaturesView = TerroristVM;
        }
    }
}
