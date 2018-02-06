# ARK Server API: Improved Commands (Plugin)

## Introduction

Adds additional commands to ARK Survival Evolved servers using ARK Server API.

### Chat
* **/MyDinoStats**: Prints the base levels, and optionally base stats, of a owned and tamed dino in-front of the player.
* **/DestroyMyDino**: Deletes the owned and tamed dino in-front of the player from the ARK.
* **/Suicide**: Kill your own character (to get unstuck etc.)

### Rcon and Console
* **DoRespec `<steam id>`**: Force respec (mindwipe) of a character.
Example: `DoRespec 12345678912345678`

* **DoRespecDino `[<dino_id1> <dino_id2>]`**: Force respec of a tamed dino.
  * **`DoRespecDino  123456789 123456789`**: Respec the given tamed dino.

  * **`DoRespecDino`**: Respec the targeted tamed dino (console only).

* **DestroyAllStructuresForTeamId `<team id>`**: Destroys all structures owned by a player or team.
Example: `DestroyAllStructuresForTeamId 123456789`

* **DestroyStructuresForTeamIdAtPosition `<team id>` `<x>` `<y>` `<radius>` `<rafts>`**: Destroys all structures owned by a player or team within a given range from a position (optionally including rafts).
Example: `DestroyStructuresForTeamIdAtPosition 1234567890 85000.0 275000.0 1000 1`

* **DestroyDinosForTeamId `<team id>`**: Destroys all tamed dinos owned by a player or team.
Example: `DestroyDinosForTeamId 123456789`

* **DroppedEggs `<action>`**: List or destroy fertilized eggs on the map.
  * **`DroppedEggs  list`**: List eggs.

  * **`DroppedEggs  destroy_spoiled`**: Destroy spoiled eggs.

  * **`DroppedEggs  destroy_spoiled_including_dropped_by_player`**: Destroy spoiled eggs, even ones that have been dropped by a player.

  * **`DroppedEggs  destroy_all`**: Destroy all eggs.

  * **`DroppedEggs  destroy_all_including_dropped_by_player`**: Destroy all eggs, even ones that have been dropped by a player.

* **FeedDinosForTeamId `<team id>`**: Feed all dinos owned by a player or team.
Example: `FeedDinosForTeamId 123456789`

* **ImprintOn `[<steam id>]`**: Imprint yourself or another player on the targeted tamed dino.
  * **`ImprintOn`**: Imprint on yourself.

  * **`ImprintOn 12345678912345678`**: Imprint on another player.

* **NextImprint `[reroll [pet, food, walk]]`**: Immediately unlock the next imprint on the targeted tamed baby dino.
  * **`NextImprint`**: Unlock next imprint.

  * **`NextImprint reroll`**: Reroll the current imprint and unlock it immediately.
  
  * **`NextImprint reroll food`**: Reroll the current imprint as feed kibble and unlock it immediately.


##  Installation

1. Install ARK Server API (http://arkserverapi.com).
2. Download and extract ImprovedCommands into `ShooterGame\Binaries\Win64\ArkApi\Plugins`.
3. Optionally configure the plugin in `config.json`.

##  Configuration

In the configuration file `config.json` you may do the following
* Enable/disable individual commands.
* Change the name of individual commands.
* Enable/disable the ARK Server API permission system for individual chat commands.

#### DroppedEggs configuration
* ClassNames: Type(s) of dropped eggs the command will consider.

#### DestroyMyDino configuration
* Method: The method used when destroying a creature ("destroy" or "suicide").

#### MyDinoStats configuration
* BaseStats
  * Show: Enable/disable calculation and output of base stats (in addition to base levels).
  * ImprintCalc: Imprinting quality used in calculation ("none" => 0%, "current" => from dino, "best" => 100%)
  * TamingEfficiencyCalc: Taming Efficiency used in calculation ("current" => from dino, "best" => 100%)


## Acknowledgements

This plugin is based on Michidu's work on Ark-Server-Plugins and ARK Server API. The basic plumbing code is copied directly from those plugins.

## Links

ARK Server API (http://arkserverapi.com)

ARK Server API [GitHub] (https://github.com/Michidu/ARK-Server-Beyond-API)

Ark-Server-Plugins [GitHub] (https://github.com/Michidu/Ark-Server-Plugins)