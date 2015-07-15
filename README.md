# The Brood War API {#BWAPI}
[TOC]
# Project Information {#project}

* [Project website] (http://bwapi.github.io)
* [Go to the build pipeline](http://ec2-54-68-169-224.us-west-2.compute.amazonaws.com:8080/)
* [Go to the releases](https://github.com/bwapi/bwapi/releases)

@htmlinclude twitter_widget

## Overview {#overview}

The Brood War Application Programming Interface (BWAPI) is a free and open source C++ framework that is
used to interact with the popular Real Time Strategy (RTS) game Starcraft: Broodwar. Using BWAPI,
students, researchers, and hobbyists can create Artificial Intelligence (AI) agents that play the game.

BWAPI only reveals the visible parts of the game state to AI modules by default. Information on units
that have gone back into the fog of war is denied to the AI. This enables programmers to write competitive
non-cheating AIs that must plan and operate under partial information conditions. BWAPI also denies
user input by default, ensuring the user cannot take control of game units while the AI is playing.
These defaults can be changed for flexibility, unless enforced by a Tournament Module (game referee for
AI tournaments). Changed defaults will be advertised when the match begins.


## Capabilities {#capability}

 - Write competitive AIs for Starcraft: Broodwar by controlling individual units.
 - Read all relevant aspects of the game state.
 - Analyze replays frame-by-frame, and extract trends, build orders, and common strategies.
 - Get comprehensive information on the unit types, upgrades, technologies, weapons, and more.
 - Study and research real-time AI algorithms in a robust commercial RTS environment.

 
## Getting Started {#getting-started}
 - Developing a bot in C++? Then download the [latest release](https://github.com/bwapi/bwapi/releases).
 - If you would like to develop a bot in Java, please follow [this tutorial](http://sscaitournament.com/index.php?action=tutorial).
 - Want to help improve BWAPI? Fork the [`develop` branch](https://github.com/bwapi/bwapi/tree/develop).

 
## Important Links & Contact Information {#contact}
* **Documentation:**         http://bwapi.github.io/
* **Repository:**            https://github.com/bwapi/bwapi
* **Issue Tracker:**         https://github.com/bwapi/bwapi/issues
* **IRC Channel:**           http://webchat.freenode.net/?channels=BWAPI
* **Facebook:**              https://www.facebook.com/groups/bwapi/
* **Links to competitions, bots, etc. :**    https://github.com/bwapi/bwapi/wiki/Useful-Links


## Competitions {#competition}
Various venues host remote competitive AI competitions that allow developers from around the world to participate. These venues are often held annually and will sometimes offer prizes to the winners. It is also a great way to test your bot's capabilities.
* [AAAI Conference on Artificial Intelligence and Interactive Digital Entertainment (AIIDE)](http://www.starcraftaicompetition.com)
* [IEEE Conference on Computational Intelligence and Games (CIG)](http://cilab.sejong.ac.kr/sc_competition/)
* [Student StarCraft AI (SSCAI) Tournament](http://sscaitournament.com/)
* [BWAPI Bots Ladder](http://bots-stats.krasi0.com)


## Issues {#issues}
You may experience issues when working with BWAPI. Here are some steps you may want to follow in order to resolve them.
1. Check the log files found in `Starcraft/Errors/`.
2. Ask in the IRC channel if anyone has experienced your issue before.
3. Check the Issue Tracker to see if your issue has already been reported.
4. Submit an issue to the Issue Tracker. Some pieces of information to consider submitting are
  * Log files
  * Screenshots
  * Version or revision number
  * Operating System
  * **Steps to reproduce the problem**

[Go to the Issue tracker](https://github.com/bwapi/bwapi/issues)


## Legal {#legal}
[Starcraft](http://www.blizzard.com/games/sc/) and [Starcraft: Broodwar](http://www.blizzard.com/games/sc/) are trademarks of
[Blizzard Entertainment](http://www.blizzard.com). BWAPI is a third party "hack" that violates the End User License Agreement (EULA).
It is strongly recommended to purchase a legitimate copy of Starcraft: Broodwar from Blizzard Entertainment before using BWAPI.
