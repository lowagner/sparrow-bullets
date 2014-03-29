sparrow-bullets
===============

Getting sparrow3d talking to bullet physics. The
project is in its infant stages, but you can compile the code
and see a box fall from the sky.  Whoo!

sparrow3d is a C library, and bullet physics is mostly C++.  So
what you will find here is a C++ game framework which wraps
the sparrow3d library in an extern C bracket.


Compiling
---------

I have compiled this on a Linux distribution, and good luck
trying to do it on something else.  If you are on Linux, first
download sparrow3d from https://github.com/theZiz/sparrow3d.

Then compile sparrow3d.  You may need to install extra libraries such as
`libsdl-ttf2.0-dev`, `libsdl-mixer1.2-dev`, `libsdl-net1.2-dev`,
`libsdl-gfx1.2-dev`, and what not.  For sparrow-bullets, you'll 
also need to install `libbullet-dev` so grab that while you're at it.

After compiling the sparrow3d library, head back to sparrow-bullets,
copy options.mk.sample to options.mk, edit options.mk and put
in the correct sparrow3d location.  Then you 
should be able to compile it the code with `make` and run the program.

Eventually:  to compile for open source hardware such as the OpenPandora,
you will need to follow the roadmap in this thread:
[CrossCompiler ToolChain](http://boards.openpandora.org/topic/7147-crosscompiler-toolchain-based-on-openpandoraorg-ipks/).  You
would also need to compile the Bullet physics library natively for the 
Pandora (to get the libBullet\*.so) , and that is not something I 
have tried to do, and I am not sure if it is even possible.   If so, it
should only need to happen once (I think), and then I can include the .so
files on this github repository.  Let me know if you can provide them, and
I can work some fancy magic in the Makefile.  
