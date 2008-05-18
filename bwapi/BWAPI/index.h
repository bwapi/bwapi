/** @mainpage Welcome to Broodwar api documentaiton

@section intro Introduction

Broodwar API is library allowing simple access to broodwar relevant data and 
unit control.
The library is not inteded to make hacks or bw tools, but to make broodwar ai 
project possible, we are still considering how to avoid using this library to 
use some hack tools.

@section tutorial Tutorial

@section install Installation

The library target is Windows Xp + Visual studio 2005 only for now. 
To use the library:

-# Download latest sources.
-# Get Chaos launcher
-# Set Environment "ChaosDir" variable in windows (ControllPanels->System->Advanced->Envirenment variables) to your chaos launcher directory.
-# Compile (U must compile on "Release", you can also use debug later, but the ".bwl" has to be the release version)
-# move the bwapi.ini file from the project to the starcraft directory
-# move the "data" directory in the project to the starcraft directory as directory "bwapi"- it is annoying - if you can read the bw address from registry in batch file, we could automate it
-# Run Chaos launcher and click to use the BWAPI addon

@section parametrization Parametrization of BW API

@section design Overall Design

*/

/** @page webpages Related webpages
  http://code.google.com/p/bwapi - home of the bwapi library.
  http://www.broodwarai.com/forums/index.php?showtopic=295 - Thread where the project started.
*/