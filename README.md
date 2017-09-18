# ARK Beyond API: Improved Commands (Plugin)

## Introduction

Adds additional commands to ARK Survival Evolved servers using ARK Beyond API.

Chat
* **/MyDinoStats**: Prints the base stats (levels) of a owned and tamed dino in-front of the player.
* **/DestroyMyDino**: Deletes the owned and tamed dino in-front of the player from the ARK.

Rcon
* **DoRespec `<steam id>`**: Force respec (mindwipe) of a character.
Example: `DoRespec 12345678912345678`

* **DestroyAllStructuresForTeamId `<team id>`**: Destroys all structures owned by a player or team.
Example: `DestroyAllStructuresForTeamId 123456789`

* **DestroyStructuresForTeamIdAtPosition `<team id>` `<x>` `<y>` `<radius>` `<rafts>`**: Destroys all structures owned by a player or team within a given range from a position (optionally including rafts).
Example: `DestroyStructuresForTeamIdAtPosition 1234567890 85000.0 275000.0 1000 1`

* **DestroyDinosForTeamId `<team id>`**: Destroys all tamed dinos owned by a player or team.
Example: `DestroyDinosForTeamId 123456789`

## Acknowledgements

This plugin is based on Michidu's work on Ark-Server-Plugins and ARK Beyond API. The basic plumbing code is copied directly from those plugins.

## Links

My ARK Beyond API Fork (https://github.com/tsebring/ARK-Server-Beyond-API)

ARK Beyond API by Michidu (https://github.com/Michidu/ARK-Server-Beyond-API)

Ark-Server-Plugins (https://github.com/Michidu/Ark-Server-Plugins)