sparrow-bullets
===============

attempting to get sparrow3d working with bullet physics. The
project is in its infant stages, so no working examples (yet).  But
you can compile a code and see a box.

sparrow3d is a C library, and bullet physics is mostly C++.  So
what you will find here is a C++ game framework which wraps
the sparrow3d library in an extern C bracket.


Compiling
---------

I have compiled this on a Linux distribution, but if someone
with another distro has a roadmap, send it my way.


### Linux

Download sparrow3d from https://github.com/theZiz/sparrow3d.

Compile it.  You may need to install extra libraries such as
`libsdl-ttf2.0-dev`, `libsdl-mixer1.2-dev`, `libsdl-net1.2-dev`,
`libsdl-gfx1.2-dev`, and what not.  For sparrow-bullets, you'll 
also need to install `libbullet-dev` so grab that while you're at it.

After compiling the sparrow3d library, head back to sparrow-bullets
and change the Makefile:  put in the correct sparrow3d location.  You
should be able to compile it and run it.

Eventually:  to compile for open source hardware such as the OpenPandora,
you will need to follow the roadmap in this thread:
http://boards.openpandora.org/topic/7147-crosscompiler-toolchain-based-on-openpandoraorg-ipks/
You would also need to compile the Bullet physics library natively for the 
Pandora (to get the libBullet\*.so) , and that currently is not in my skill
set.   But that should only need to happen once, and then I can include the .so
files in this github (I think).  Let me know if you can provide them.
