# SimBio

## Setup Instructions for Visual Studio (2022+)

0) Make sure you have Development tools with C++ installed in Visual Studio
   - Open Visual Studio Installer, click Modify on VS 2022, make sure the box is checked for it; if not check it and click Modify
1) Open Visual Studio
2) "Click Clone a repository"
3) Click "GitHub" under "Browse a Repository"
   - You may need to sign in
4) Click "lwpti/SimBio"
5) Click "Clone"
   - This may take few minutes
6) Drop down src and double click "main.cpp" on the right side of the screen (in the "Solution Explorer - Folder View" window)
   - You should see the code for the main function
7) Select the drop down arrow next to the solid green play button in the top center of the screen and select "Simbio.exe (src\SimBio.exe)"
8) Press the solid green play button

Extra step if it does not work and your "flecs" or "raylib" folders are empty:
Pull all submodules (`flecs` and `raylib`)
```bash
git submodule update --init --recursive
```
The only runnable target is the SimBio executable. Visual Studio shows this as "SimBio.exe".

