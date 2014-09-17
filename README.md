sparrow-bullets
===============

Getting Sparrow3d talking to Bullet physics, and awesomeness results.

Sparrow3d is a C library for rendering and audio, and Bullet is C++.  So
what you will find here is a C++ game framework which wraps
the Sparrow3d library in an extern C bracket.


Compiling
---------

I have compiled this on a Linux distribution, and good luck
trying to do it on something else.  If you are on Linux, first
download sparrow3d from https://github.com/theZiz/sparrow3d.

Then compile sparrow3d.  To do so, you will need to install extra libraries.  On
Ubuntu, you can grab these:
`libsdl-ttf2.0-dev`, `libsdl-mixer1.2-dev`, `libsdl-net1.2-dev`,
`libsdl-gfx1.2-dev`, and `libsdl-image1.2-dev`.  (Let me know if I've missed any.)
For sparrow-bullets, you'll 
also need to install `libbullet-dev` so grab that while you're at it.

After compiling the Sparrow3d library, head back to sparrow-bullets,
copy options.mk.sample to options.mk, edit options.mk and put
in the correct Sparrow3d location, as well as modifying Bullet directories,
if necessary.  Then go into the `example` directory
and you should be able to compile the code with `make`.  Then run the program
with `./sparrow-bullet`.  By the way, check the LICENSE in that directory.  It's 
a completely free-to-use/copy/learn-from example.  You're welcome.


Roadmap
-------

I will be making a few games using sparrow-bullets, and they'll end up as
sub-directories here.  I hope to get them on open source hardware such as
the OpenPandora and the GCW-Zero.

Eventually:  to compile for open source hardware such as the OpenPandora,
you will need to follow the roadmap in this thread:
[CrossCompiler ToolChain](http://boards.openpandora.org/topic/7147-crosscompiler-toolchain-based-on-openpandoraorg-ipks/).  You
would also need to compile the Bullet physics library natively for the 
Pandora (to get the libBullet\*.so) , and that is not something I 
have tried to do, and I am not sure if it is even possible.   If so, it
should only need to happen once (I think), and then I can include the .so
files on this github repository.  Let me know if you can provide them, and
I can work some fancy magic in the Makefile.  


Licensing
---------

Check out the LICENSE files in each folder.  The examples (in the `example` directory)
are free to copy and learn from, you don't have to give credit to me or anything.  It 
would be nice, of course.  Full games (as they are being developed) are under the MIT license.

Note: Bullet Physics and Sparrow3D have their own licenses.

 Bullet is a 3D Collision Detection and Rigid Body Dynamics Library for games and animation.
 Free for commercial use, including Playstation 3, open source under the ZLib License.

 Sparrow3D is licensed under the (L)GPL.

