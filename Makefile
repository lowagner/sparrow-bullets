PROGRAM=sparrow-bullet
SPARROW_FOLDER = /home/lowagner/code/sparrow3d
BULLET_SRC_DIR=. #/home/lowagner/code/bullet2/src
BULLET_INCLUDE=. #/usr/local/include -I$(BULLET_SRC_DIR)
BULLET_DYNAMIC_LIB= #-rdynamic $(BULLET_SRC_DIR)/BulletDynamics/libBulletDynamics.so $(BULLET_SRC_DIR)/BulletCollision/libBulletCollision.so $(BULLET_SRC_DIR)/LinearMath/libLinearMath.so -Wl,-rpath,$(BULLET_SRC_DIR)/BulletDynamics:$(BULLET_SRC_DIR)/BulletCollision:$(BULLET_SRC_DIR)/LinearMath

#==stuff linked to
DYNAMIC = -lSDL_ttf -lSDL_mixer -lSDL_image -lSDL -lm
#==global Flags. Even on the gp2x with 16 kb Cache, -O3 is much better then -Os
#CFLAGS = -O3 -fsingle-precision-constant -fPIC -Wno-write-strings
CFLAGS = -O3 -DNDEBUG -Wno-write-strings
CDEBUGFLAGS = -O1 -g -fsingle-precision-constant -fPIC
# Testtweaks: -fgcse-lm -fgcse-sm -fsched-spec-load -fmodulo-sched -funsafe-loop-optimizations -Wunsafe-loop-optimizations -fgcse-las -fgcse-after-reload -fvariable-expansion-in-unroller -ftracer -fbranch-target-load-optimize
GENERAL_TWEAKS =  -ffast-math
#==PC==
CPP = c++ -march=native -DX86CPU
SDL = `sdl-config --cflags`


ifdef TARGET
include $(SPARROW_FOLDER)/target-files/$(TARGET).mk
BUILD = ./build/$(TARGET)/$(PROGRAM)
SPARROW_LIB = $(SPARROW_FOLDER)/build/$(TARGET)/sparrow3d
else
TARGET = "Default (change with make TARGET=otherTarget. See All targets with make targets)"
BUILD = .
SPARROW_LIB = $(SPARROW_FOLDER)
endif
LIB += -L$(SPARROW_LIB) 
DYNAMIC += -lsparrow3d -lsparrowSound -lsparrowNet #$(BULLET_DYNAMIC_LIB)

all: main.cpp
	@echo "=== Built for Target "$(TARGET)" ==="

targets:
	@echo "The targets are the same like for sparrow3d. :P"

main.cpp: copySparrow play.o makeBuildDir meta.h
	$(CPP) $(CFLAGS) play.o main.cpp $(SDL) $(INCLUDE) -I$(BULLET_INCLUDE) -I$(SPARROW_FOLDER) $(LIB) $(SDL_LIB) $(STATIC) $(DYNAMIC) -o $(BUILD)/$(PROGRAM) $(BULLET_DYNAMIC_LIB)

play.o: play.cpp play.h gamestates.h meta.h
	$(CPP) $(CFLAGS) -c play.cpp $(SDL) $(INCLUDE) -I$(BULLET_INCLUDE) -I$(SPARROW_FOLDER) $(SDL_INCLUDE) $(SPARROW_INCLUDE)

makeBuildDir:
	 #@if [ ! -d $(BUILD:/$(PROGRAM)=/) ]; then mkdir $(BUILD:/$(PROGRAM)=/);fi
	 @if [ ! -d $(BUILD) ]; then mkdir $(BUILD);fi

copySparrow:
	cp $(SPARROW_LIB)/libsparrow3d.so $(BUILD)
	cp $(SPARROW_LIB)/libsparrowSound.so $(BUILD)
	cp $(SPARROW_LIB)/libsparrowNet.so $(BUILD)

clean:
	rm -f *.o libsparrow*
	rm -f $(PROGRAM) $(PROGRAM)-g

#
debug: copySparrow makeBuildDir gamestates.h play.h meta.h
	rm -f *.o
	$(CPP) $(CDEBUGFLAGS) -c play.cpp $(SDL) $(INCLUDE) -I$(BULLET_INCLUDE) -I$(SPARROW_FOLDER) $(SDL_INCLUDE) $(SPARROW_INCLUDE)
	$(CPP) $(CDEBUGFLAGS) play.o main.cpp $(SDL) $(INCLUDE) -I$(BULLET_INCLUDE) -I$(SPARROW_FOLDER) $(LIB) $(SDL_LIB) $(STATIC) $(DYNAMIC) -o $(BUILD)/$(PROGRAM)-g $(BULLET_DYNAMIC_LIB)
	rm -f *.o

