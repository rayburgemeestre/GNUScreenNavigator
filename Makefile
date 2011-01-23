CC		= g++
CFLAGS	= -O2 -g -Wall -funsigned-char -I lib/jsoncpp-src-0.5.0/include/ 
LFLAGS	= -lm -L lib/jsoncpp-src-0.5.0/libs/ -ljson -lncurses -lmenu
OBJ		= widgets.o \
			launcher.o\
			menurenderer.o\
			abstractengine.o\
			menuengine.o\
			dispatchengine.o\
			menu.o\
			file.o\
			dir.o
BIN		= launcher

all:		launcher

install:
			chmod +x scripts/* launcher
			cp scripts/*.sh /usr/local/bin/
			cp launcher /usr/local/bin/
			chmod +x /usr/local/bin/launcher /usr/local/bin/launch_scr.sh /usr/local/bin/launch_scr2.sh
			@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
			@echo "add this to your .screenrc: bindkey ^x exec /usr/local/bin/launch_scr.sh"
			@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

clean:
			rm -f *.o

abstractengine.o:		src/abstractengine.cpp
	$(CC) $(CFLAGS) -c src/abstractengine.cpp

menuengine.o:		src/menuengine.cpp
	$(CC) $(CFLAGS) -c src/menuengine.cpp

dispatchengine.o:		src/dispatchengine.cpp
	$(CC) $(CFLAGS) -c src/dispatchengine.cpp

dir.o:		src/widgets/dir.cpp
	$(CC) $(CFLAGS) -c src/widgets/dir.cpp

file.o:		src/widgets/file.cpp
	$(CC) $(CFLAGS) -c src/widgets/file.cpp

menu.o:		src/widgets/menu.cpp
	$(CC) $(CFLAGS) -c src/widgets/menu.cpp

widgets.o:		src/widgets.cpp
	$(CC) $(CFLAGS) -c src/widgets.cpp

launcher.o:		src/launcher.cpp
	$(CC) $(CFLAGS) -c src/launcher.cpp

menurenderer.o:		src/menurenderer.cpp
	$(CC) $(CFLAGS) -c src/menurenderer.cpp
	
launcher:	$(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LFLAGS)
