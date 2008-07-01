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
-# move the content of the "starcraft-folder" into your starcraft folder (note that it is good to just move the bwapi.ini there and map the location of the bwapi-data to your project, co your data won't be doubled)
-# Run Chaos launcher and click to use the BWAPI addon
-# If you want to test the functionality, take in mind that your nick must be the same as the one specified in bwapi.ini in starcraft folder, and only terran supported now.

@section parametrization Parametrization of BW API

@section threadDesign Thread design

BWAPI gets loaded as Chaos plugin, and its DLLMain is called. DLLMain just initialises several hooks like 
onFrame, onSendCommand, onGameStart, onGameEnd etc. All the code than runs when bw calls some of these 
functions, our code takes the control over bw for a while, and gives it back to bw when it is done.

@section overallDesign Overall design

The whole project is divided in several layers.

-# Bw itself, can be regarded as lowest layer.
-# BW namespace, it maps all the bw relevant data, so we can access it, this namespace shouldn't contain any 
   data, it only contains access to bw data.
-# BWAPI uses BW namespace to provide easy to use interface to access bw data and call it's functions, BWAPI 
   already contains it's own data and structures that helps to access bw data effectively.
-# BWAI uses BWAPI namespace (and shoul use as least of BW directly as possible, prefferably just enumerations)
   to do the real ai stuff.
-# BuildOrder can be regarded as the highest layer, as it tells the BWAI what to do


*/

/** @page webpages Related webpages
  http://code.google.com/p/bwapi - home of the bwapi library.
  http://www.broodwarai.com/forums/index.php?showtopic=295 - Thread where the project started.
*/