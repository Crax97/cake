# HELLO HELLO HELLO

This is the repo where i'm going to put my small game engine, developed with a modular approach in mind (at least i hope).
Right now all i have is a sloppy 3D renderer, but we all start somewhere

Also it's called cake because my s.o likes cake

# How do i build it?
```bash
mkdir build 
cd build
cmake ..
make
```

Some of the headers of this program (specifically the ones in the `generated_headers` folder) are generated using the 
[Katsu](https://github.com/Crax97/katsu) generator

Be wary that this project requires c++20 (no modules, only concepts/constraints + some headers are used)

# How do i develop games with it?
Wait until i put the game framework in an usable state lol