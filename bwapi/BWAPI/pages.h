/** @page localData Latency and local data issues

  Bw must distribute every command to all players before it can run it and
  it can't wait every game frame until all commands are distributed (would be laggy),
  so it has to run every coommand with deley.
  This deley is called latency, where \#L  is count of frames before commands take effect. 
  As it would be annoying to workaround the latency in the code, as you don't see any result of order
  until the \#L frames pass, we decided to make workaround tool for that directly 
  in our bw api.

  We will have 3 versions of bw data.
  -# Data that starcraft uses directly 
  -# Our copy (that is made every frame), so some multithread version could
     work with that without problems.
  -# Partialy precomputed version of data

  Let's explain it on example, let's assume you ordered your command center to start 
  building scv, but \#L the command center will still not start the build, so code 
  like if (cc->hasEmptyQueue()) -> cc->train(SCV), will order the command to build 
  the scv \#L + 1 times.

  So when you do builSCV command, the bwapi library will call that %command in bw, 
  but also call our function, that modifies the precomuted version of data 
  immediatly, store it (the command) in buffer, and keep it \#L frames, then every 
  frame, we will make fresh copy of bw data (version 3), and apply all commands 
  stored in the buffer manualy.

  This include for now (will be extended)
  -# Training : We will just place the unit to be built in the building queue, 
     decrease mineral count, and increase supply used (all in Local data version ofcourse).
     Note that we will probably implement negative % progress of the unit in construction)
  -# Right click order (Change of unit orderID and target(location/unit)).

  so the code will be (commandCenter->hasEmptyQueueLocal()) -> commandCenter->train(BWAPI::Prototypes::SCV) and 
  the preComputed (with Local postfix) state will change immedeatly.

  Everywhere the "Local" prefix is used, the function/data involves this kind of precomputed data.

  This issue concerns
  <ul>
    <li>BWAPI::Unit#getRawDataLocal</li>
    <li>BWAPI::Unit#getBuildQueueLocal</li>
    <li>BWAPI::Unit#getBuildQueueSlotLocal</li>
    <li>BWAPI::Unit#hasEmptyBuildQueueLocal</li>
    <li>BWAPI::Unit#getOrderIDLocal</li>
    <li>BWAPI::Player#getMineralsLocal</li>
    <li>BWAPI::Player#getGasLocal</li>
    <li>BWAPI::Player#getSuppliesAvailableProtossLocal</li>
    <li>BWAPI::Player#getSuppliesUsedProtossLocal</li>
    <li>BWAPI::Player#freeSuppliesProtossLocal</li>
    <li>BWAPI::Player#getSuppliesAvailableTerranLocal</li>
    <li>BWAPI::Player#getSuppliesUsedTerranLocal</li>
    <li>BWAPI::Player#freeSuppliesTerranLocal</li>
    <li>BWAPI::Player#getSuppliesAvailableProtossLocal</li>
    <li>BWAPI::Player#getSuppliesUsedProtossLocal</li>
    <li>BWAPI::Player#freeSuppliesProtossLocal</li>
  </ul>


  The user of the library must be aware, that we can't guarantee that the preComputed 
  state will really occur, for example if you order to build scv, and just before 
  the command gets executed you lose minerals, the command will fail.
*/

/** @page functionality List of library functionality
The library provides easy
-# Access to in-game units
-# Access players minerals/gas/supplies/names
-# Select/RightClick/Train command execution
-# Slot State/Race change command execution
-# Game start command execution (form chat lobby)
-# Catch start/end events
-# Catch nextFrame event (virtually)
-# Precompute game state according to commands immediatly (@ref localData)

We are just going to integrate soon
-# Easy access to selected units (shared/current player)
-# Build building command execution
-# Morph larva command execution

We are searchnig a way how to do 


*/