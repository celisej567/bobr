## Windows

In Visual Studio Powershell:

- ``mkdir build``
- ``cd build``
- ``cmake .. -G "Visual Studio 18 2026"`` (no other VS and MSVC support are guaranteed because i havent tested them)
- Rebuild whole solution
- Do INSTALL
- Files will be in (project root folder)/_BUILD/


## Linux (tested on arch btw)

- Get sdl3 package somewhere
- ``mkdir build; cd build``
- ``cmake ..``
- ``cmake --build .``
- ``cmake --install .``
- Files will be in (project root folder)/_BUILD/
