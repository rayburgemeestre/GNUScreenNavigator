CC		= g++
CFLAGS	= -O2 -g -Wall -funsigned-char -I lib/jsoncpp-src-0.5.0/include/ 
LFLAGS	= -lm -L lib/jsoncpp-src-0.5.0/libs/ -ljson -lncurses -lmenu
OBJ		= widgets.o parser.o menu_renderer.o
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

widgets.o:		src/widgets.cpp
	$(CC) $(CFLAGS) -c src/widgets.cpp

parser.o:		src/parser.cpp
	$(CC) $(CFLAGS) -c src/parser.cpp

menu_renderer.o:		src/menu_renderer.cpp
	$(CC) $(CFLAGS) -c src/menu_renderer.cpp
	
launcher:	$(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LFLAGS)
	make clean
