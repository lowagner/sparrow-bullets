sparrow-bullets
===============

attempting to get sparrow3d working with bullet physics.

sparrow3d is a C library, and bullet physics is mostly C++.  So
what you will find here is a C++ game framework which wraps
the sparrow3d library in an extern C bracket.




Requirements
------------

I have compiled this on a Linux distribution, but if someone
with another distro has a roadmap, send it my way.


Download sparrow3d from https://github.com/theZiz/sparrow3d.

Compile it.  You may need to install extra libraries such as
`libsdl-ttf2.0-dev`, `libsdl-mixer1.2-dev`, `libsdl-net1.2-dev`,
`libsdl-gfx1.2-dev`, and what not.

After compiling the sparrow3d library, head back to sparrow-bullets
and change the Makefile:  put in the correct sparrow3d location. 


Eventually for bullet (not needed presently).  Install `libbullet-dev`
and change `LIB_BULLET_SRC` to /usr/local/include/bullet.  That
maybe will work!  


Eventually:  to compile for open source hardware such as the OpenPandora,
you will need to follow the roadmap in this thread:
http://boards.openpandora.org/topic/7147-crosscompiler-toolchain-based-on-openpandoraorg-ipks/

