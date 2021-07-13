[Documentation](http://bwapi.github.io/) | [Github Repository](https://github.com/bwapi/bwapi) | [Issues](https://github.com/bwapi/bwapi/issues) | [Q&A](https://github.com/bwapi/bwapi/discussions/categories/q-a) | [More Resources](https://github.com/bwapi/bwapi/wiki/Useful-Links)

[![Build](https://github.com/bwapi/bwapi/actions/workflows/build.yml/badge.svg)](https://github.com/bwapi/bwapi/actions/workflows/build.yml) [![Tests](https://github.com/bwapi/bwapi/actions/workflows/test.yml/badge.svg)](https://github.com/bwapi/bwapi/actions/workflows/test.yml)

## Project Information

### Overview

The Brood War [Application Programming Interface](https://en.wikipedia.org/wiki/Application_programming_interface) (BWAPI)
is a free and open source C++ framework that is used to interact with the popular
[Real Time Strategy](https://en.wikipedia.org/wiki/Real-time_strategy) (RTS) game
[Starcraft: Broodwar](http://www.blizzard.com/games/sc/). Using BWAPI, students, researchers, and hobbyists can
create [Artificial Intelligence](https://en.wikipedia.org/wiki/Artificial_intelligence) (AI) agents that play the game.

BWAPI only reveals the visible parts of the game state to AI modules by default. Information on units
that have gone back into the fog of war is denied to the AI. This enables programmers to write competitive
non-cheating AIs that must plan and operate under partial information conditions. BWAPI also denies
user input by default, ensuring the user cannot take control of game units while the AI is playing.
These defaults can be changed for flexibility, unless enforced by a Tournament Module (game referee for
AI tournaments). Changed defaults will be advertised when the match begins.


### Capabilities

 - Write competitive AIs for Starcraft: Broodwar by controlling individual units.
 - Read all relevant aspects of the game state.
 - Analyze replays frame-by-frame, and extract trends, build orders, and common strategies.
 - Get comprehensive information on the unit types, upgrades, technologies, weapons, and more.
 - Study and research real-time AI algorithms in a robust commercial RTS environment.

 
## Users Getting Started
Developing a bot in C++? Then download the [latest release](https://github.com/bwapi/bwapi/releases). If you would like to develop a bot in Java, please follow [this tutorial](http://sscaitournament.com/index.php?action=tutorial).

1. Find and install one of the supported flavours of **StarCraft: Brood War**:
   - StarCraft: Brood War 1.16.1
2. Download and extract a [BWAPI 5+ release](https://github.com/bwapi/bwapi/releases)
3. Follow one of the *Quick Start* sections below to build a bot.
4. Follow *Starting the Game Client* to start Starcraft.
5. Run your bot executable. It will connect to the Starcraft instance and wait for a game to start.

*Note: At this time, OpenBW and StarCraft: Remastered are unsupported (but being worked on).*

### Quick Start With Visual Studio (Windows)

1. Install [Visual Studio Community 2019](https://visualstudio.microsoft.com/vs/)
2. Open the sample `.sln` from the BWAPI release.
3. Build the solution.

### Quick Start With CMake (Windows, Mac, Linux)

1. Install [CMake](https://cmake.org/)
2. Open a command line in the BWAPI directory you just extracted.
3. Run the following commands to configure and build the project:
    ```sh
    cmake -B Debug -DCMAKE_BUILD_TYPE=Debug
    cmake -B Debug --build . --parallel
    ```

For a release target, replace `Debug` with `Release`.

### Starting the Game Client

#### Starcraft 1.16.1 (Windows)
1. Run `Chaoslauncher.exe` from the provided Chaoslauncher directory (it comes with the download)
2. Check *BWAPI Injector* and any other plugins you want to include
3. Click **Start**

To run a game against Blizzard's AI:
  1. Go to **Single Player** -> **Expansion**
  2. Select any user and click **OK**
  3. Click **Play Custom**, select a map, and start a game

To Run a game against yourself:
  1. To use the *Local PC* and *Direct IP* multiplayer providers, copy `SNP_DirectIP.snp` to your Starcraft directory.
  2. Run `Chaoslauncher - MultiInstance.exe`
  3. Click **Start**
      1. Go to **Multiplayer** -> **Expansion** -> **Local PC**
      2. Select a user and click **OK**
      3. Click **Create Game**, select a map, and click **OK**
  4. Click **Start** in Chaoslauncher again
      1. Go to **Multiplayer** -> **Expansion** -> **Local PC**
      2. Select a user and click **OK**
      3. Join the existing game created by the other client

Each bot instance will connect to only one Starcraft instance.

### Issues
You may experience issues when working with BWAPI. Here are some steps you may want to follow in order to resolve them.
1. Check the log files found in `Starcraft/Errors/`.
2. Ask around if anyone has experienced your issue before.
3. Check the [Issue Tracker](https://github.com/bwapi/bwapi/issues) to see if your issue has already been reported.
4. Submit an issue to the [Issue Tracker](https://github.com/bwapi/bwapi/issues). Some pieces of information to consider submitting are:
   - Log files
   - Screenshots
   - Version (or git commit hash)
   - Operating System
   - **Steps to reproduce the problem**

### Getting Help
- [Github Q&A Board](https://github.com/bwapi/bwapi/discussions/categories/q-a)
- [SSCAIT Discord](https://discordapp.com/invite/w9wRRrF)

## BWAPI Developers Getting Started
Want to help improve BWAPI? Fork the [develop branch](https://github.com/bwapi/bwapi/tree/develop) and read the [contributing guide](CONTRIBUTING.md).

