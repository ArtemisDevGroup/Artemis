using Artemis.Loader.Core;
using System;

namespace Artemis.Loader.MVVM.ViewModel
{
    class AboutViewModel : ObservableObject
    {
        private static string GetRandomCat()
        {
            Random rand = new Random((int)DateTime.Now.Ticks);

            string uri = "pack://application:,,,/Imagez/Cats/";

            switch (rand.Next(1, 22))
            {
                case 1:
                    uri += "Bandit.png";
                    break;
                case 2:
                    uri += "Blitz.png";
                    break;
                case 3:
                    uri += "Caveria.png";
                    break;
                case 4:
                    uri += "Dokkaebi.png";
                    break;
                case 5:
                    uri += "Echo.png";
                    break;
                case 6:
                    uri += "Ela.png";
                    break;
                case 7:
                    uri += "Frost.png";
                    break;
                case 8:
                    uri += "Fuze.png";
                    break;
                case 9:
                    uri += "Hibana.png";
                    break;
                case 10:
                    uri += "IQ.png";
                    break;
                case 11:
                    uri += "Jager.png";
                    break;
                case 12:
                    uri += "Kapkan.png";
                    break;
                case 13:
                    uri += "Lesion.png";
                    break;
                case 14:
                    uri += "Rook.png";
                    break;
                case 15:
                    uri += "Tachanka.png";
                    break;
                case 16:
                    uri += "Thatcher.png";
                    break;
                case 17:
                    uri += "Twitch.png";
                    break;
                case 18:
                    uri += "Valkryie.png";
                    break;
                case 19:
                    uri += "Vigil.png";
                    break;
                case 20:
                    uri += "Ying.png";
                    break;
                case 21:
                    uri += "Zofia.png";
                    break;
            }

            return uri;
        }

        private string _currentCat;

        public string CurrentCat
        {
            get { return _currentCat; }
            set
            {
                _currentCat = value;
                OnPropertyChanged();
            }
        }

        public AboutViewModel()
        {
            CurrentCat = GetRandomCat();
        }
    }
}
