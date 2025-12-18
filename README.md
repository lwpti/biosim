# SimBio

---

## Overview

### Flecs
 - Flecs is an entity component system (ECS)
 - It manages entities, which in our case includes organisms and plants
 - It allows assignment of components to entities, which in our case are organs and statuses
 - It allows systems which are automatically applied to all entities with specified components. For example, our movement systems apply to all organisms with legs, and modifies the location status.
 - It allows queries which are like systems that are manually called, which we use to render the simulation.

### Percepts
 - Vision: Limited data of all flecs entities in a cone ahead of the perceiver.
 - Hearing: A combination of all of the "sounds" produced nearby. For example, movement produces a sound. If one organism is moving nearby, the perceiver will hear that, and if two are moving nearby, it will hear a combination of those two.

### Organs
 - Legs: Allows organisms to move and rotate.
 - Mouth: Allows organisms to eat plants and damage/eat other organisms.
 - Body: Determines maximum sizes for other organs and affects movement speed and maximum health and energy.
 - Eyes: Enable the vision percept.
 - Ears: Enable the hearing percept.

### Major Issues
 - We need to implement collision between all flecs entities. This will make everything more realistic and allow us to make the environment more interesting with walls and whatnot.
 - We need to either speed up the simulation a lot or make it continuous (or slow down movement a lot). Right now there are a lot of bugs because organisms can move up to 50 pixels in a single tick, so it is very difficult for one organism chasing another to align with its precise location to bite it.
 - The way movement works is probably unintuitive for machine learning. It should be reevaluated and simplified.

---

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

