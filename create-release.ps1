$path = "x64\Release\ImprovedCommands"

New-Item -ItemType Directory -Force -Path $path
Copy-Item x64\Release\ImprovedCommands.dll $path
Copy-Item x64\Release\ImprovedCommands.pdb $path
Copy-Item Configs\Config.json $path
Copy-Item Configs\PluginInfo.json $path

Compress-Archive -Path $path -CompressionLevel Optimal -DestinationPath ImprovedCommands-2.00.zip

Remove-Item $path -Force -Recurse -ErrorAction SilentlyContinue