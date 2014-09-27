sparrow-bullets
===============

Getting Sparrow3D talking to Bullet physics, and awesomeness 
results.  Sparrow3D is a C library for rendering and audio, and Bullet is C++.  So
what you will find here is a C++ game framework which wraps
the Sparrow3D library in an extern C bracket.


Compiling
---------

I have compiled this on a Linux distribution, and good luck
trying to do it on something else.  If you are on Linux, first
download Sparrow3D from https://github.com/theZiz/sparrow3d
and compile.  To do so, you may need to install extra libraries.  On
Ubuntu, you can grab these:
`libsdl-ttf2.0-dev`, `libsdl-mixer1.2-dev`, `libsdl-net1.2-dev`,
`libsdl-gfx1.2-dev`, and `libsdl-image1.2-dev`.  (Let me know if I've missed any.)
For sparrow-bullets, you'll 
also need to install `libbullet-dev` so grab that while you're at it.

After compiling the Sparrow3D library, head back to sparrow-bullets,
copy options.mk.sample to options.mk, edit options.mk and put
in the correct Sparrow3D location, as well as modify the Bullet directories
(if necessary).  Then go into the `example` directory
and you should be able to compile the code with `make`.  Then run the program
with `./sparrow-bullet`.  By the way, check the LICENSE in that directory.  It's 
a completely free-to-use/copy/learn-from example.  You're welcome.


dumps
-----

I will be making a few games using sparrow-bullets, and they'll end up as
sub-directories here.  They go by a different LICENSE, so please check their
respective directories.  The first contender is `dumps`, in which you play
as a red-checkered cube dumptruck trying to clean the world of all its
cubey messes!  Compile with `make` and run with `./dump`.

Keyboard controls:
- WASD control the camera
- E jumps
- Q resets play
- [Enter] opens gameplay menu 
- [Backspace] pauses/unpauses
- Arrow keys move the checkered dumptruck, RELATIVE motion!

If you fall on your face or side, the arrow keys may do something which is
unexpected (for you), but I contend that it makes sense (in a relative way),
if you know exactly how you landed.  Also you have control
of your orientation midair, but not your velocity.  This is important
for certain levels.  For instance, if you hold forward while jumping, you will probably
land on your face, whereas if you jump and then press backwards you may have a little
more control.

Note:  All levels are beatable, try to figure them out.


Roadmap
-------

I hope to get these games working on open source hardware such as
the OpenPandora and the GCW-Zero.  To compile for these,
you might need to follow the roadmap in this thread:
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

