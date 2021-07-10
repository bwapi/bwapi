LatencyChanger by MasterofChaos
Version 0.2.3
For patch 1.16.1

Features:
Reduces latency on bnet. It is enabled for games which end on
#LL or #L1 ... #L5. All players in such a game have to have the plugin enabled, or a disconnect occurs direcly on beginning of the game.
Example:
MyGame#LL => Latency=LowLatency=2 like on LAN
MyGame#L3 => Latency=3

Normal Latencies:
1 Singleplayer
2 LAN
5 Battle.net

For LAN-games there are two different settings:
"Reduce latency to 1 on LAN" this has the same effect as #L1 on bnet
All players have to have the same settings, or a disconnect occurs.

"Reduce userlatency on LAN" this works as if the user has chosen one setting lower in the Network-Settings. So Extra High becomes High, High becomes Low, and Low becomes a new Very Low setting. This setting can be chosen independently by the users, but for fairness reasons, I recommend using the same setting.

These settings are only relevent when playing on LAN. Real LAN, not LAN-Latency on bnet!

This LatencyChanger is not compatible with the one integrated in the ICCup launcher. It works on all gateways(and LAN), whereas the ICCup LAN-Latency only works on ICCup-Server. But it is compatible with LatencyChanger 0.1 on bnet.

Press Ctrl-L to bring up the lag screen earlier which shows who your Starcraft thinks is responsable for lag. If you are the one who lags it shows wrong players. So this is only usefull for games with more than two players. To find out which observer is lagging etc.

Changes Starcraftcode in memory, which is detectable by Antihacktools like Warden, so there is a risk, that blizzard decides to invalidate the users Accounts/CD-Keys.
USE AT YOUR OWN RISK!

Use with a BWL4 Launcher like Chaoslauncher
http://winner.cspsx.de/Starcraft