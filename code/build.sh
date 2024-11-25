#!/bin/bash

CC=g++
CPPFLAGS=`pkg-config --cflags glew ImageMagick freetype2 glfw3 fontconfig assimp`
CPPFLAGS="$CPPFLAGS -I../Include -I../Common/FreetypeGL -ggdb3"
LDFLAGS=`pkg-config --libs glew ImageMagick freetype2 glfw3 fontconfig assimp`
LDFLAGS="$LDFLAGS -lglut ../Lib/libAntTweakBar.a -lX11  "

$CC -I/usr/include/freetype2 hw4.cpp skinning_technique.cpp ../Common/ogldev_skinned_mesh.cpp ../Common/ogldev_basic_mesh.cpp ../Common/ogldev_util.cpp ../Common/pipeline.cpp ../Common/math_3d.cpp ../Common/camera.cpp ../Common/ogldev_atb.cpp ../Common/glut_backend.cpp ../Common/ogldev_texture.cpp ../Common/ogldev_basic_lighting.cpp ../Common/technique.cpp ../Common/ogldev_app.cpp ../Common/FreetypeGL/freetypeGL.cpp ../Common/3rdparty/stb_image.cpp $CPPFLAGS $LDFLAGS -o hw4
