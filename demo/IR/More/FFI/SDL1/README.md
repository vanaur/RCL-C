## SDL1
This is a very basic example of FFI (calling foreign functions from dynamic libraries).

The SDL2 library is required.

You can compile the `SDL.c` file into a dynamic library with:

```
gcc -c SDL.c -lSDL2
gcc -shared -o SDL.dll SDL.o
```
If you are running on Linux/Mac, you may want to change the file extensions.

This will show a blank window:

![Result](\img0.PNG)
