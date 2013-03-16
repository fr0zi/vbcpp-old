# Virtual Bus Core++

Repository has all code and libraries needed to compile executable file. When you clone repository, you can run:

- make.bat - compiling on Windows using Mingw32 compiler
- make.sh - compiling on Linux using GNU GCC

After that you will see file load3ds or load3ds.exe in current directory.

### Running the code

Running is simple:

     vbcpp [3ds_file] [texture_directory]

So, running:

     vbcpp model.3ds

or:

     vbcpp model.3ds ./

will couse loading model from file 'model.3ds' from current directory and use needed textures from current directory.

When you type:

     vbcpp model.3ds textures/

will start application and load model from file 'model.3ds' but textures will be loaded from sub-directory 'textures/'.
