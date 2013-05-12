# Virtual Bus Core++

### Libraries

This project uses following libraries for running:

- OpenGL 3.3 Core (graphics library for scene rendering, working on Linux and Windows)
- GLFW (creating OpenGL window, handling input / output events)
- SOIL (reading textures from graphics files)
- Bullet Physics (physics and collisions)
- OpenAL or Audiere (sounds and audio effects)

### Compiling

Repository has all code and libraries needed to compile executable file. All necessary libraries are placed in
'3rdparty' subdirectory in project's directory. Compilation is based on GNU Make mechanism and requires only
text editor for code editing (Vim, Nano, gEdit, Notepad, Notepad++ or so...) and GNU C++ Compiler (Linux) 
or Mingw32 Compiler (Win). This guide assumes that you have correct path to your compiler set in system environment.

This project is provided with two simple scripts to simplify compilation. When you clone repository, all you have to do is run:

- make.bat - for compiling on Windows using Mingw32 compiler, or
- make.sh - for compiling on Linux using GNU GCC

After that you will see file vbcpp or vbcpp.exe in current directory.

### Running the code

Running an application is simple:

    vbcpp [3ds_file] [texture_directory]

So, running:

    vbcpp model.3ds

or:

    vbcpp model.3ds ./

will couse loading model from file 'model.3ds' from current directory and use needed textures from current directory.

When you type:

    vbcpp model.3ds textures/

will start application and load model from file 'model.3ds' but textures will be loaded from sub-directory 'textures/'.
