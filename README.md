# SimBio

---

## Pull all submodules (`flecs` and `raylib`)

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
