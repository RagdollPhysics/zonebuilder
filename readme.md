<h1>Introduction</h1>
Zonebuilder is a tool that was designed to aid in the creation of custom assets for the Fourth Infity Ward Iteration of a certain popular FPS series.  

<h1>Setup</h1>
The first step to making your custom assets is to setup your development forlder correctly.  I recommend simply extracting the program directly into your game folder.  This will help with most of the setup.  You need a full game installation for the program to work as it requires many game files to work.

<h2>Folder Structure</h2>
This is the basic folder structure that needs to exist in your game folder.  If you extracted the program directly into your game folder then this should already be set up for you. It also includes an example asset (weapon model with anims) for you to test with.

+ /zone_source
    + *.csv files
+ /zonebuilder
    + /images
    + /materials
    + /sounds
    + /fx
    + build scripts
    + any resource files needed to build fastfiles
    
<h1>File Formats</h1>
<h2>CSV File</h2>
CSV files are included throughout the project (and the game) and describe data through use of a very simple format shown below.
```
value1, value2, value3
value4, , value5
value6, value7, value8
```
<h3>zone_source CSV files</h3>
Each row of this file describes an asset in the fastfile that will be built. The rows can be described 2 different ways.  You can use any kind of combination of these two in a csv file.

Custom Asset
```
type, asset name, asset filename
```

Repacking Asset *note: for here you need the asset loaded from a source (-s) zone*
```
type, asset name
```

<h3>Material CSV files</h3>
Materials are described by CSV files even though their extension is ".txt".  Their format is as follows.
```
option, value
```
These options are described in the table below.

| option | value description |
| ------ | ----------------- |
| basemat | The base material to use. |
| colorMap | color map image | 
| normalMap | normal map image | 
| specularMap | specular image | 
| detailMap | camo image | 

<h2>XME, XAE, and IWI</h2>
These are files that define models, animations, and images respectively. They are the files referenced in the zone's csv file when creating a custom asset. They are binary formats and not described here.

<h1>Building a custom Fastfile</h1>
Custom Fastfiles are one of the ways that you can load custom assets into the game.  This process is fairly simple if you followed the setup process correctly.  I will refer to paths in reference to the base folder of your game installation.

__Step 1:__
First step is to create a .csv for your new fastfile.  Using the format described above, create a csv that includes all the assets that you wish to include in your fastfile.

__Step 2:__
Make sure all of your asset files are in their correct folders inside the /zonebuilder directory.  If files aren't in here then the tool will be unable to find them.

__Step 3:__
Run Zonebuilder.  The simpelest way to do this is to simply call ```zonebuilder.exe fastfile``` where fastfile is the name of your csv.  This will output a fastfile into the /zone/language folder.

<h2>Custom Models</h2>
Coming Soon...
<h2>Custom Anims</h2>
Coming Soon...
<h2>Custom Materials</h2>
Coming Soon...
<h2>Custom Sounds</h2>
Coming Soon...

<h1>Command Line Options</h1>
__-s__: Load a fastfile to be used as a source zone. You can have as many source zones as you want. but be careful about overflowing asset pools. This isn't checked by the tool. *note: there is no space between the -s and the fastfile name!*

__-v__: This option allows you to verify a created fastfile. Prints out every asset in the fastfile.

__-d__: Dumps certain assets from the game to a format that can be rebuilt by this tool.

__-b__: Change the value of fs_basegame. Defaults to "zonebulder".  Where you place all data the tool uses.