<p align="center">
  <img src="https://github.com/ArtemisDevGroup/Artemis-Resources/blob/main/Logos/Logo256x.png" width="256" height="256"/>
</p>

![Artemis](https://github.com/ArtemisDevGroup/Artemis-Resources/blob/main/Text/Artemis.png)
An internal cheat for Rainbow Six: Siege - Shadow Legacy. Developed by Sigma, Monke and Tavreth.

# Projects
## Artemis
The main module to be injected into the game. Contains every interaction with the game client itself. Written in C++ as a dynamic link library (dll).
## Artemis.Loader
The loader for the cheat. This process will inject the module into the game. Written in C# as a WPF application.
## Artemis.Loader.Helper
A helper project for the loader. It bridges the gap between managed C# code and unmanaged C++ code. Written in C++ as a .NET CLR dll to extend the main loader.
## Artemis.SoftwareDevelopmentKit
The SDK used by the main module. Written in C++.
