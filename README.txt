		 __     __   _  _  __ _   ___  _  _  ____  ____ 
		(  )   / _\ / )( \(  ( \ / __)/ )( \(  __)(  _ \
		/ (_/\/    \) \/ (/    /( (__ ) __ ( ) _)  )   /
		\____/\_/\_/\____/\_)__) \___)\_)(_/(____)(__\_)


### Requirements

- libjson, for configuration file $HOME/.launcher
- scons, for building libjson
- python, for running scons
- ncurses, used by launcher code

Python should be easy to install, probably a similar named package on your
linux dist.  For ncurses you need version 5. In debian I think the correct
package is: libncurses5-dev. On my SUSE machine I noticed libncurses5 and
ncurses-devel didn't install the required include files.  You may have to build
it yourself (it's in lib/, you can use ./configure, make, make install)

### Building

sh configure.sh prepares libjson and tries to compile it
sh build.sh does that and tries to compile launcher.
sh build-ncurses.sh builds ncurses from the lib (optional)


The manual approach would be:

- follow build instructions for libjson .tar.gz
    - install python..
    - install scons..
- make sure the libs are properly named libjson.a and libjson.so and available
  in lib/jsoncpp-src-0.5.0/libs/. The Makefile for launcher expects them to be
  there.
- make, make install, make clean

Now add a configuratio in your $HOME directory: $HOME/.launcher. Something like
the following. (I know it is currently very verbose, editing wasn't supposed to
be done manually but from within the utility. But currently that isn't
supported.).

{
   "0" : {
      "items" : {
         "a" : {
            "name" : "/var/chroot/lispbot/home/trigen",
            "type" : "Dir"
         },
         "b" : {
            "name" : "/var/chroot/lispbot/home/trigen/bot.lisp",
            "type" : "File"
         }
      },
      "name" : "IRC bot workdir",
      "type" : "Menu"
   },
   "1" : {
      "items" : {
         "a" : {
            "name" : "/usr/local/src/launcher",
            "type" : "Dir"
         },
         "b" : {
            "name" : "/usr/local/src/launcher/src",
            "type" : "Dir"
         },
         "c" : {
            "name" : "/usr/local/src/launcher/src/parser.cpp",
            "type" : "File"
         }
      },
      "name" : "C++ projects",
      "type" : "Menu"
   }
}

### GNU screen interaction

bindkey ^x exec /usr/local/bin/launch_scr.sh

### Known issue on SUSE

In case the compilation of libjson complains with error message regarding "- W
a l l", just remove that flag from the SConstruct file.  I couldn't fix it:
it's probably a bug in this old scons distribution, maybe it has something to
with character sets, as it inserted spaces after each character.

--- jsoncpp-src-0.5.0/SConstruct        2010-03-12 08:31:04.000000000 +0100
+++ jsoncpp-src-0.5.0/SConstruct     2011-01-15 14:19:57.000000000 +0100
@@ -106,7 +106,7 @@
     env.Append( CPPDEFINES=[ "WIN32", "NDEBUG", "_MT" ] )
 elif platform.startswith('linux-gcc'):
     env.Tool( 'default' )
-    env.Append( LIBS = ['pthread'], CCFLAGS = "-Wall" )
+    env.Append( LIBS = ['pthread'], CCFLAGS = '' )
     env['SHARED_LIB_ENABLED'] = True
 else:
     print "UNSUPPORTED PLATFORM."

