<h1>ZoneBuilder</h1>
Zonebuilder helps create custom assets for fourth-generation FPS titles developed by Infinity Ward.  

<h2>Setup</h2>
Setup the development folder by extracting the ZoneBuilder into the game folder.  Having the ZoneBuilder directory within the game's folders will help with the setup.  The process requires a full game installation for the program to work as ZoneBuild works with the game files.

<h3>Folder Structure</h3>
Extracting the program directly into the game folder will include the proper folder structure. The extracted files include an example asset (weapon model with animations) to test with.

+ /zone_source
    + *.csv files
+ /zonebuilder
    + /images
    + /materials
    + /sounds
    + /fx
    + build scripts
    + any resource files needed to build fastfiles
    
<h3>File Formats</h3>
<h4>CSV File</h4>
The project and game includes CSV files describing data with a simple format:
```
value1, value2, value3
value4, , value5
value6, value7, value8
```
<h4>zone_source CSV files</h4>
Rows describe assets in the fastfile to build and can be described 2 ways.  Any combination of the two will work in a csv file.

Custom Asset
```
type, asset name, asset filename
```

Repacking Asset *note: for here you need the asset loaded from a source (-s) zone*
```
type, asset name
```

<h4>Material CSV files</h4>
CSV files describe materials even though the file's extension is ".txt".  The format is as follows.
```
option, value
```
The options for materials are described in the table below.

| option | value description |
| ------ | ----------------- |
| basemat | The base material to use. |
| colorMap | color map image | 
| normalMap | normal map image | 
| specularMap | specular image | 
| detailMap | camo image | 

<h3>XME, XAE, and IWI</h3>
XME, XAE, and IWI files define models, animations, and images respectively. The zone's CSV file references the XME, XAR, and IWI files when creating a custom asset. The format for the files are in binary and not described here.

<h2>Building a custom Fastfile</h2>
Custom Fastfiles allows for loading custom assets into the game.  This process is simple if the setup process was done correctly.  The paths reference to the base folder of the game installation.

__Step 1:__
Create a .csv for the new Fastfile.  Use the format described above and create a CSV including all the assets to be included in the Fastfile.

__Step 2:__
Ensure all of the asset files are in the correct folders inside the /zonebuilder directory.  ZoneBuilder cannot find the files when not placed in the correct directory.

__Step 3:__
Run Zonebuilder.  A simple way to run ZoneBuilder is to call ```zonebuilder.exe fastfile``` where fastfile is the name of the CSV.  ZoneBuild will output a fastfile into the /zone/language folder.

<h2>Custom Sounds</h2>
Creating custom sounds requires a compatable sound file (mp3 or wav).
The definition of the sound alias is a text file and only contains the filename of the sound.
Place sounds in the /sound subdirectory.
An example of the corresponding CSV line for a sound alias called "explosion3" with a sound file of "sound/explosions/expl3.wav" would be:
```sound,explosion3,sound/expl3.txt ```
and the contents of sound/expl3.txt would be:
```sound/explosions/expl3.wav ```


<h2>Command Line Options</h2>
__-s__: Loads a Fastfile to be the source zone. There is no limit on the number of source zones and be cautious about overflowing asset pools as the issue is not handled by ZoneBuilder. *note: there is no space between the -s and the Fastfile name!*

__-v__: Verifies a created Fastfile and prints out every asset in the Fastfile.

__-d__: Dumps certain assets from the game to a format that ZoneBuilder can rebuild.

__-b__: Changes the value of fs_basegame and defaults to "zonebulder" where all data is placed.
