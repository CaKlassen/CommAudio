==========================================================
|        Data Communications Windows Assignment 4        |
==========================================================
|                                                        |
|                    Christofer Klassen                  |
|                        Melvin Loho                     |
|                       Jonathan Chu                     |
|                                                        |
==========================================================

------------
- Overview -
------------

This assignment is a complex audio streaming program designed to send music over a network to be played aloud on the receiving end.


----------------
- File Listing -
----------------

The complete list of files included in this project are:

>> Client <<
	CMessage.h
	CommAudio.pro
	CommAudio.pro.user
	ControlChannel.cpp
	ControlChannel.h
	main.cpp
	mainwindow.cpp
	mainwindow.h
	mainwindow.ui
	Mic.cpp
	Mic.h
	micoutput.cpp
	micoutput.h
	MusicBuffer.cpp
	MusicBuffer.h
	Network.cpp
	Network.h

>> Server <<
	music (folder)
	plugins (folder)
	vlc (folder)
	Application.cpp
	Application.h
	CMessage.h
	ControlChannel.cpp
	ControlChannel.h
	dirent.h
	MusicBuffer.cpp
	MusicBuffer.h
	Network.cpp
	Network.h


------------------
- Final Comments -
------------------

The server must be compiled with the following additional include directories:
	$(ProjectDir)

The server must be compiled with the following libraries:
	WS2_32.lib
	winmm.lib
	libvlc.lib
	libvlccore.lib

If there are any issues compiling, executing, or using this program,
please let me know!