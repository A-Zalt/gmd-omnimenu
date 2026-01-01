# OMNImenu / OmniMenu

Mod Menu for older versions of Geometry Dash (pre-1.9).

Currently works for versions 1.02 - 1.71.

## Downloads
Downloads are available in the Discord server: https://discord.gg/4AC23yn4yF

## Libraries Used
- DobbyHook
- fmtlib
- miniaudio
- rapidjson

## Special Thanks
- [**akqanile (Adelfa)**](https://github.com/cierra-kb) - 100 KB fix, 16k fix, lots of help with issues. He is regarded as the god of tradmodding for a reason.
- Eclipse Mod Menu team - many hack implementations
- Hris69 - betatested on Android 4.4. Also got me started with tradmodding in general. Without him, this mod menu wouldn't exist.
- Nikolyas, Caster, Tabbi - betatesters
- HJFod - BetterEdit code (particularly the RGB Color Inputs)
- iAndy_HD3 - Helping out with fmtlib
- Pololak - GD 1.7 (1.9?) Cocos 2.2.3 headers

## Build Instructions
Note: the minimum supported Android version is currently KitKat (4.4). Android 14+ devices won't work without solutions like Shizuku or Virtual Master.

0. NOTE: You should remove the build folder reguarly.
1. [Download and run APK Editor Studio](<https://qwertycube.com/apk-editor-studio/download/>)
2. Open Settings > Options or press Ctrl+P, go to Apktool and check "Decompile source code (smali)" if it isn't checked already
3. In the bottom left corner of APK Editor Studio, click on File System, find the `lib` folder. Then, if you're building for pre-1.5, open the only folder: `armeabi`, otherwise open `armeabi-v7a` (and delete the other folders). Right-click the existing `.so` file and press "Open Resource Directory...", then copy the `libgame.so` or `libcocos2dcpp.so` file and paste it to `src/version/{GD version}`.
4. Find the package name smali uses by locating the GeometryJump.smali file. The default directory is `com/robtopx/geometryjump/GeometryJump.smali`. If this directory is different for you, open `version/constants.hpp` in OMNImenu's source code and change the `JAVA_PATH_MAIN` and `JAVA_PATH_MAIN_JNI_HOOK` constants to use the correct path instead of `com/robtopx/geometryjump` and `com_robtopx_geometryjump` respectively. Also, if you plan to compile for the Neopointfour GDPS (which adds extra features exclusive to the GDPS), uncomment the `#define NP4` line
5. Run build script (TODO instructions for Windows). The libmenu.so file will be in the `build` folder, which you need to paste to the same location you got `libgame.so`/`libcocos2dcpp.so` from.
6. Copy all of the resources from the `resources` folder to the APK's `assets` folder. If compiling for 1.0, copy the resources from `resources-1.0` as well. If compiling for Neopointfour, copy the resources from `resources-np4`.
7. In the `smali` folder of the APK, open the `GeometryJump.smali` file. Find where it says something like:

```
const-string v0, "game"

invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
```

Copy and paste these lines right below the original lines and replace `"game"` (or `"cocos2dcpp"`) with `"menu"`, so that it says something along the lines of:

```
.method static constructor <clinit>()V
    .locals 1

    const-string v0, "game"

    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

    const-string v0, "menu"

    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

    return-void
.end method
```

(Note: this is slightly different for newer versions)

After that, find the onCreate method, where it should be something like:

```
.method protected onCreate(Landroid/os/Bundle;)V
    .locals 5

    const v4, 0x80001

    const/4 v3, -0x1

    invoke-super {p0, p1}, Lorg/cocos2dx/lib/Cocos2dxActivity;->onCreate(Landroid/os/Bundle;)V

    ...
```

Below the `invoke-super` line, put this line: 

```
sput-object p0, Lcom/robtopx/geometryjump/GeometryJump;->instance:Lcom/robtopx/geometryjump/GeometryJump;
```

and then these three lines if building with miniaudio:
```
invoke-virtual {p0}, Landroid/content/Context;->getAssets()Landroid/content/res/AssetManager;

move-result-object v0

invoke-virtual {p0, v0}, Lcom/robtopx/geometryjump/GeometryJump;->nativeSetAssetManager(Landroid/content/res/AssetManager;)V
```

And, at the top of the file, after the static field definitions, add this line: `.field private static instance:Lcom/robtopx/geometryjump/GeometryJump;`

(replace `com/robtopx/geometryjump` in all of these if necessary)

8. In the folder with the mod source, open `smali/GeometryJump.smali`, copy everything from it and paste it at the bottom of the APK's `GeometryJump.smali`. This is required for GDShare to work. If the APK's `GeometryJump.smali` is not located at `com/robtopx/geometryjump`, you should replace the 2 occurences of `com/robtopx/geometryjump/GeometryJump` in the smali code you're pasting in to `new/path/to/GeometryJump`. Also copy `smali/ClipboardHelper.smali` and `smali/ClipboardHelper$1.smali` to the smali folder of the APK. This is required for Copy UDID to work on Bluestacks.
9. (Optional, but recommended) Switch back to Resources at the bottom left of APK Editor Studio. Click on values, then strings, then the entry without a flag. Find `<string name="app_name">Geometry Dash</string>` and replace `Geometry Dash` with your desired app name. (Note: the recommended capitalizations are OMNImenu or OmniMenu)
10. (Optional, but recommended) Change the package name in apktool.yml: the line where it says `renameManifestPackage: null`, change `null` to your desired package name. Note that it **must have a length of 24 characters** (to match `com.robtopx.geometryjump`), no more, no less. Allowed characters are `[a-z0-9.]`. Open `libgame.so` (for pre-1.7) or `libcocos2dcpp.so` (for 1.7-1.8) in `lib/armeabi` (for pre-1.5) or `lib/armeabi-v7a` (for 1.5+) in a hex editor, search for an occurence of `com.robtopx.geometryjump` (or the previous package name if it's different) and replace it with your new package name.
11. Click File > Save APK... or press Ctrl+Alt+S and save the APK.
12. You're good to go!