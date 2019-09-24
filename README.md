## Unown Report

![](/bpre-unown-report.gif)

The Johto-based games introduced an item called the [Unown Report](https://bulbapedia.bulbagarden.net/wiki/Unown_Report), which logs the Unown forms you have captured, and logs the player's research into the Ruins of Alph as they progress through the quest.

This ports that item to FR v1.0 ROMs.

### Build Instructions

See [here](https://gist.github.com/Zeturic/db1611cc7b17c3140f9b9af32e1b596b) for the prerequisites and help in installing them.

Not included in the above link is the dependency of [grit](https://www.coranac.com/man/grit/html/grit.htm), which you should download and add to your PATH.

#### Cloning the repo and building the tools

Open your terminal to whatever folder you want to download this repo into. Then, do the following to download the repo and build the included tools:

```shell
$ git clone https://github.com/sphericalice/bpre-unown-report.git
$ cd bpre-unown-report
$ ./build_tools.sh
```

#### Adding your ROM

Copy your ROM to this directory and rename it `rom.gba`.

#### Configuration

##### Compile Time Constants

Open `config.mk` in a text editor to set some compile-time configuration.

The build system is smart enough to find enough free space on its own, but if you want it to be inserted at a particular address, you can specify it by updating the definition of `START_AT`.

If the given address is acceptable (i.e. is word-aligned and has enough bytes of free space), it will be inserted there. Otherwise, it will just use `START_AT` to determine where in the ROM it should start looking for free space.

The code automatically sets the Field Script for the Unown Report to point to the correct function. You must therefore configure the item's index by simply setting the `ITEM_UnownReport` field in `config.mk` to the desired item ID. This repository lists the item ID used for Pokémon Gaia, which is `0x182`, so you will likely have to change this as it may not be a valid item ID for your ROM.

See the Usage section below for information on how the item should be configured.

##### Flags and Variables

This item requires two free variables and six flags, which are defined in the header files `include/constants/vars.h` and `include/constants/flags.h`, respectively.

The two variables keep track of which Unown forms the player has captured, and the flags, when set, unlock additional pages of the Unown Report.

#### Strings

If you want to edit the strings involved in this feature - for example, to edit the Gaia-specific story text for the report pages - they can be found and modified in `src/strings.c`.

#### Building the project itself

Once you're ready, run:

```shell
make
```

This won't actually modify `rom.gba`, instead your output will be in `test.gba`. Naturally, test it in an emulator.

### Usage

#### How should I configure the Unown Report item?

As explained in the Compile Time Constants section above, running `make` will automatically change the Field Script of the specified item index in `config.mk`.

The item requires some additional configuration to work as intended, however.

The `Pocket` should be `02 Key Items`. The `Type` should be `01 Out of battle`.

Of course, you can change the item's sprite and description at your leisure.

#### How can I change the background image?

Just edit the `unown.png` image directly in `include/graphics/`, save it, close it, and run `make`.

Of course, the usual restrictions for these types of tilemaps apply; this only uses one palette, so there is a limit of 15 colours plus transparency. I recommend keeping the size of 256x160, too, but bear in mind that only the leftmost 240 pixels are visible.

#### How can I add extra report pages to the Unown Report?

New report pages can be added by adding new strings to the `src/strings.c`, adding their declaration in `include/strings.h`, and adding them to the `ReportPages[]` array in `include/strings.h`, along with the flag needed for the page to be unlocked. You can add new flags in `include/constants/flags.h`. As the page counter is an unsigned byte, the maximum number of report pages is 250 (with the other 5 pages being the first page and the 4 allocated for the list of Unown forms).

### Caveats

* This code alters the move command `0xF1`, which is called by the battle system upon a Pokémon's capture to try and set the Pokédex flags. If your ROM hack has already changed this function, you may need to add your changes from vanilla to the `atkF1_TrySetCaughtMonDexFlags` function in `src/battle_script_commands.c`.

* This code also alters the function which registers the Pokédex flags of traded Pokémon, so if your hack has edited this function already, you will need to add your changes to the function `SetTradedMonPokedexFlags` in `src/trade.c`.

* At the moment, this code does not register new Unown forms obtained through `givepokemon`, Mystery Gift, or hatching from Eggs -- only those captured in battle or obtained via trades. I will most likely update this with support for these methods of obtaining Unown eventually, though.

* Save files from before this was added will not have Unown forms registered even if the player has already captured them, but will register the player if they catch them again.

### Credits

Project structure, `preproc`, and `scaninc` are all from [pokeemerald](https://github.com/pret/pokeemerald).

This repository is also heavily based on work done by [Zeturic](https://github.com/Zeturic).
