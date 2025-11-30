# SimBio

---

## Setup Instructions

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

---

## Part A

Students will collaborate to design and create a simulated environment which includes the following components:

- A toroidal grid with an appropriately large size  
- At least one species of plant with the ability to grow and reproduce  
- Initially randomly distributed throughout the grid  
- Various organs that can be used to build organisms, including at least the following:

---

### Eyes

- Receive visual data from the environment  
- Consumes a small amount of energy  
- Adds a small amount of weight  
- Variable resolution; increasing resolution increases energy usage  

---

### Ears

- Receive auditory data from the environment  
- Consumes a small amount of energy  
- Adds a small amount of weight  
- Variable sensitivity; increasing sensitivity increases energy usage  

---

### Legs

- Allow organism to move through grid  
- Consumes a large amount of energy when in use  
- Adds a moderate amount of weight  
- Variable strength; determines speed along with total mass, and ability to escape holds  

---

### Arms

- Allows organism to grab and damage other organisms  
- Consumes a large amount of energy when in use  
- Adds a moderate amount of weight  
- Variable strength; determines damage and ability to hold  

---

### Mouths

- Allows organisms to consume plants, other organisms, or both  
- Adds a large amount of weight  
- Specialized diet increases efficiency of energy absorption  
- Size increases total that can be eaten  

---

### Brains

- Connects input organs (Eyes, Ears, Arms, Mouths) to output organs (Legs, Arms, Mouths)  
- Black box; can use any internal AI  
- Consumes a moderate amount of energy based on compute time
