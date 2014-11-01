Unedited(but may be missing some formatting) [Short Autotools tutorial](https://www.gc3.uzh.ch/blog/Autotools_tutorial/) by [University of Zurich](https://www.uzh.ch/) licensed under [CC BY-NC-SA 3.0](http://creativecommons.org/licenses/by-nc-sa/3.0/)

What is autotools
=================

Autotools are a set of tools aimed to automatically determine the correct options for the compiler and produce Makfiles to compile your software. A better, more in deep explanation of what autotools is can be found on the [Autotools official website](http://www.gnu.org/savannah-checkouts/gnu/automake/manual/html_node/GNU-Build-System.html#GNU-Build-System), for our very short introduction the only things you need to know are:

* There are Makefile.am files you create with a specific syntax. These files essentially contain variable assignmenet, later interpreted by automake. You have one of this fore each Makefile you want to produce. This is where you define what has to be compiled starting from which source files.
* automake tool converts Makefile.am into Makefile.in files, using m4 macros and a lot of magic...
* Makefile.in files are parametric makefiles, makefiles with some placeholders like @prefix@ which are replaced by the configure script. They usually contain a lot of commonly used targets, like install, clean, distclean etc...
* configure.ac is a configuration file read by autoconf tool. It contains m4 macros. This is where you can define new options for the configure script, for instance.
* autoconf tool produces a configure script where the configure.ac file is.
* libtool is an even more obscure and magic tool which is automatically called by autoconf, depending on the macros you used in the configure.ac and Makefile.am. It is able to automatically produce static or dynamic libraries.
* autoreconf automatically calls automake, autoconf and libtool in order to produce the configure scripts starting only from Makefile.am files and the configure.ac file. This is the successor of the autogen.sh script you may have found in some GNU source code...

In principle, then, you should only create Makefile.am files on each directory where the make command will run and define in there which source files are needed to produce which binary executable, and onlye one configure.ac file to specify some basic behavior of the configure script. Then, running autoreconf will produce the configure script you will use to configure your software for compiling in your machine and all the customized Makefile.in needed by the configure script.

One of the things autotools is able to do, for instance, is to automatically produce static and/or dynamic libraries, to use the proper options to produce dynamic libraries for different architectures etc...

Autotool-ize your software
--------------------------

We will start with a almost-real-world complex Hello World program. This how it appears the source code at the beginning:

    src/
    ├── hw/
    │   ├── cli.c
    │   ├── cli.h
    │   └── hw.c
    └── libmath/
        ├── hwmath.c
        └── hwmath.h

The hw.c file contain the main routine, and it uses functions from cli.c and libath/hwmath.c.

We want to produce a static library from the source files in libmath and compile it together with the files in hw, and then produce a hw binary.

### src/Makefile.am
We need a makefile in src which will only call make inside the two directories hw and libmath. Therefore this Makefile.am will only contain one directive:

    # src/Makefile.am
    # recursively process `Makefile.am` in these subdirectories
    SUBDIRS = libmath hw

Please note that we put the hw directory at the end, because the binary in there will need that the library in libmath has been already produced to compile.

Next step is then produce a library in libmath. For this, we will need libtool as we already said, and some configuration in configure.ac, but for the time being let's just modify the

### src/libmath/Makefile.am
We only need to define two variables, one to specify the name of the library, and the other one to specify the source files used to produce it. Everything else is automatically managed by libtool:

    lib_LTLIBRARIES = libhwmath.la
    
    libhwmath_la_SOURCES = hwmath.c
    
    # In case we need to define include files, or other flags for the
    # C preprocessor, we can add them to the following variable
    # libhwmath_la_CPPFLAGS =
    
    # Headers file that are going to be installed in <prefix>/include
    # include_HEADERS =

You can produce multiple libraries by appending them to the lib_LTLIBRARIES variable. In this case, you will need to define multiple _SOURCES and maybe also multiple _CPPFLAGS variables, one for each of the libraries you need to define.

Please note the .la extension: this tells the autotools to produce a file which is then managed by libtool in order to produce either a static library or a dynamic one.

### src/hw/Makefile.am
This file will define the executable to be produced, where to look for other header files, which libraries we need to add in order to compile and other flags for the C preprocessor or the linker. Let's take a look at it:

    # Defines which program executables will be produced
    bin_PROGRAMS = hw
    
    # Defines which source files are used to produce the specificed executable
    hw_SOURCES = cli.c hw.c
    
    # Defines flags for the C preprocessor *specific to ``hw``*
    # hw_CPPLFAGS =
    
    # These are instead *default* flags, used by all the programs in ``bin_PROGRAMS``
    AM_CPPFLAGS = -I$(srcdir) -I$(top_srcdir)/libmath
    
    # Extra options for the linker
    # hw_LDFLAGS =
    
    # Libraries the ``hw`` binary will be linked to
    hw_LDADD = $(top_srcdir)/libmath/libhwmath.la

### src/configure.ac
This file defines how the configure script will be written. Some options are pretty standard, let's take a look at a minimal configure.ac script:

    ## Process this file to produce a configure script:
    ##   aclocal -I build-aux/m4 && autoheader && autoconf && automake
    ##
    
    ## Preamble - used to set up meta paths, meta-information, etc.
    #
    # require a minimum version of AutoConf
    AC_PREREQ([2.65])
    
    # software name, version, contact address
    AC_INIT([hw],[1.0.0],[foo.bar@example.com])
    
    # if this file does not exist, `configure` was invoked in the wrong directory
    AC_CONFIG_SRCDIR([hw/hw.c])
    
    # directories (relative to top-level) to look into for AutoConf/AutoMake files
    AC_CONFIG_AUX_DIR([build-aux])
    AC_CONFIG_MACRO_DIR([build-aux])
    # enable AutoMake
    AM_INIT_AUTOMAKE([1.10])
    # all defined C macros (HAVE_*) will be saved to this file
    AC_CONFIG_HEADERS([config.h])
    
    # Macros for the compilers. Check
    # http://www.gnu.org/savannah-checkouts/gnu/autoconf/manual/autoconf-2.69/html_node/Compilers-and-Preprocessors.html#Compilers-and-Preprocessors
    # for a full list
    #
    # This macro checks if you have a C compiler
    AC_PROG_CC
    AM_PROG_CC_C_O
    
    # Check if you have a C++ compiler
    # AC_PROG_CXX
    # AC_PROG_CXX_C_O
    
    # Check if you have an Objective-C compiler
    # AC_PROG_OBJC
    # AC_PROG_OBJC_C_O
    
    # Check for fortran compilers
    # AC_PROG_F77
    # AC_PROG_F77_C_)
    # AC_PROG_FC
    # AC_PROG_FC_C_O
    
    # Check if the `install` program is present
    AC_PROG_INSTALL
    
    ## Initialize GNU LibTool
    #
    # http://www.gnu.org/software/libtool/manual/html_node/LT_005fINIT.html
    #
    # GNU LibTool provides a portable way to build libraries.  AutoMake
    # knows how to use it; you just need to activate it.
    LT_INIT([static])
    
    # Checks for header files.
    AC_CHECK_HEADERS([stdio.h stdlib.h string.h])
    
    # Checks for typedefs, structures, and compiler characteristics.
    AC_TYPE_SIZE_T
    
    # Checks for library functions.
    AC_FUNC_MALLOC
    AC_FUNC_REALLOC
    AC_CHECK_FUNCS([floor pow sqrt])
    
    # Substitute all conditionals in these files; this is normally used to
    # create `Makefile`s but could also be used for scripts, include
    # files, etc.
    AC_CONFIG_FILES([Makefile
                     hw/Makefile
                     libmath/Makefile])
    AC_OUTPUT
    Autoreconf

Now that our source directory looks like:

    src/
    ├── configure.ac
    ├── hw/
    │   ├── cli.c
    │   ├── cli.h
    │   ├── hw.c
    │   └── Makefile.am
    ├── libmath/
    │   ├── hwmath.c
    │   ├── hwmath.h
    │   └── Makefile.am
    └── Makefile.am

Some more files required by autoreconf are still missing. These files contain information on the license, on how to install the program etc. They can be empty, but autoconf will complain if they are not there, so let's create them:

    antonio@kenny:~/src$ touch NEWS README AUTHORS ChangeLog
    Some other missing auxiliary files can be instead created automatically with automake:
    
    antonio@kenny:~/src$ automake --add-missing
    configure.ac:17: required directory ./build-aux does not exist
    configure.ac: no proper invocation of AM_INIT_AUTOMAKE was found.
    configure.ac: You should verify that configure.ac invokes AM_INIT_AUTOMAKE,
    configure.ac: that aclocal.m4 is present in the top-level directory,
    configure.ac: and that aclocal.m4 was recently regenerated (using aclocal).
    configure.ac:47: installing `build-aux/install-sh'; error while making link: No such file or directory
    hw/Makefile.am: installing `build-aux/depcomp'; error while making link: No such file or directory
    /usr/share/automake-1.11/am/depend2.am: am__fastdepCC does not appear in AM_CONDITIONAL
    /usr/share/automake-1.11/am/depend2.am:   The usual way to define `am__fastdepCC' is to add `AC_PROG_CC'
    /usr/share/automake-1.11/am/depend2.am:   to `configure.ac' and run `aclocal' and `autoconf' again.
    /usr/share/automake-1.11/am/depend2.am: AMDEP does not appear in AM_CONDITIONAL
    /usr/share/automake-1.11/am/depend2.am:   The usual way to define `AMDEP' is to add one of the compiler tests
    /usr/share/automake-1.11/am/depend2.am:     AC_PROG_CC, AC_PROG_CXX, AC_PROG_CXX, AC_PROG_OBJC,
    /usr/share/automake-1.11/am/depend2.am:     AM_PROG_AS, AM_PROG_GCJ, AM_PROG_UPC
    /usr/share/automake-1.11/am/depend2.am:   to `configure.ac' and run `aclocal' and `autoconf' again.
    libmath/Makefile.am:1: Libtool library used but `LIBTOOL' is undefined
    libmath/Makefile.am:1:   The usual way to define `LIBTOOL' is to add `LT_INIT'
    libmath/Makefile.am:1:   to `configure.ac' and run `aclocal' and `autoconf' again.
    libmath/Makefile.am:1:   If `LT_INIT' is in `configure.ac', make sure
    libmath/Makefile.am:1:   its definition is in aclocal's search path.
    Makefile.am: installing `./INSTALL'
    Makefile.am: installing `./COPYING' using GNU General Public License v3 file
    Makefile.am:     Consider adding the COPYING file to the version control system
    Makefile.am:     for your code, to avoid questions about which license your project uses.
    configure.ac:22: required file `config.h.in' not found

In order to produce all the Makefile and the configure script you only need to run autoreconf from within the src directory. However, we will get an error:

    antonio@kenny:~/src$ autoreconf
    libtoolize: putting auxiliary files in AC_CONFIG_AUX_DIR, `build-aux'.
    libtoolize: copying file `build-aux/ltmain.sh'
    libtoolize: putting macros in AC_CONFIG_MACRO_DIR, `build-aux'.
    libtoolize: copying file `build-aux/libtool.m4'
    libtoolize: copying file `build-aux/ltoptions.m4'
    libtoolize: copying file `build-aux/ltsugar.m4'
    libtoolize: copying file `build-aux/ltversion.m4'
    libtoolize: copying file `build-aux/lt~obsolete.m4'
    libtoolize: Consider adding `-I build-aux' to ACLOCAL_AMFLAGS in Makefile.am.
    configure.ac:30: required file `build-aux/compile' not found
    configure.ac:30:   `automake --add-missing' can install `compile'
    configure.ac:55: required file `build-aux/config.guess' not found
    configure.ac:55:   `automake --add-missing' can install `config.guess'
    configure.ac:55: required file `build-aux/config.sub' not found
    configure.ac:55:   `automake --add-missing' can install `config.sub'
    configure.ac:20: required file `build-aux/install-sh' not found
    configure.ac:20:   `automake --add-missing' can install `install-sh'
    configure.ac:20: required file `build-aux/missing' not found
    configure.ac:20:   `automake --add-missing' can install `missing'
    hw/Makefile.am: required file `build-aux/depcomp' not found
    hw/Makefile.am:   `automake --add-missing' can install `depcomp'
    autoreconf: automake failed with exit status: 1

Some more files are missing, but we are almost there. Re-run automake --add-missing as suggested:

    antonio@kenny:~/src$ automake --add-missing
    configure.ac:30: installing `build-aux/compile'
    configure.ac:55: installing `build-aux/config.guess'
    configure.ac:55: installing `build-aux/config.sub'
    configure.ac:20: installing `build-aux/install-sh'
    configure.ac:20: installing `build-aux/missing'
    hw/Makefile.am: installing `build-aux/depcomp'

And finally:

    antonio@kenny:~/src$ autoreconf
    libtoolize: Consider adding `-I build-aux' to ACLOCAL_AMFLAGS in Makefile.am.
    The source directory now has been populated by more and more files:
    
    src/
    ├── aclocal.m4
    ├── AUTHORS
    ├── autom4te.cache/
    │   ├── output.0
    │   ├── output.1
    │   ├── output.2
    │   ├── requests
    │   ├── traces.0
    │   ├── traces.1
    │   └── traces.2
    ├── build-aux/
    │   ├── compile -> /usr/share/automake-1.11/compile*
    │   ├── config.guess -> /usr/share/automake-1.11/config.guess*
    │   ├── config.sub -> /usr/share/automake-1.11/config.sub*
    │   ├── depcomp -> /usr/share/automake-1.11/depcomp*
    │   ├── install-sh -> /usr/share/automake-1.11/install-sh*
    │   ├── libtool.m4
    │   ├── ltmain.sh
    │   ├── lt~obsolete.m4
    │   ├── ltoptions.m4
    │   ├── ltsugar.m4
    │   ├── ltversion.m4
    │   └── missing -> /usr/share/automake-1.11/missing*
    ├── ChangeLog
    ├── config.h.in
    ├── configure*
    ├── configure.ac
    ├── COPYING -> /usr/share/automake-1.11/COPYING
    ├── hw/
    │   ├── cli.c
    │   ├── cli.h
    │   ├── hw.c
    │   ├── Makefile.am
    │   └── Makefile.in
    ├── INSTALL -> /usr/share/automake-1.11/INSTALL
    ├── libmath/
    │   ├── hwmath.c
    │   ├── hwmath.h
    │   ├── Makefile.am
    │   └── Makefile.in
    ├── Makefile.am
    ├── Makefile.in
    ├── NEWS
    └── README

Please note that wherever we created a Makefile.am file, a Makefile.in has been created.

The configure script should now be able to create all the needed makefiles:

    antonio@kenny:~/src$ ./configure
    checking for a BSD-compatible install... /usr/bin/install -c
    checking whether build environment is sane... yes
    checking for a thread-safe mkdir -p... /bin/mkdir -p
    checking for gawk... gawk
    checking whether make sets $(MAKE)... yes
    checking for gcc... gcc
    checking whether the C compiler works... yes
    checking for C compiler default output file name... a.out
    checking for suffix of executables...
    checking whether we are cross compiling... no
    checking for suffix of object files... o
    checking whether we are using the GNU C compiler... yes
    checking whether gcc accepts -g... yes
    checking for gcc option to accept ISO C89... none needed
    checking for style of include used by make... GNU
    checking dependency style of gcc... gcc3
    checking whether gcc and cc understand -c and -o together... yes
    checking build system type... x86_64-unknown-linux-gnu
    checking host system type... x86_64-unknown-linux-gnu
    checking how to print strings... printf
    checking for a sed that does not truncate output... /bin/sed
    checking for grep that handles long lines and -e... /bin/grep
    checking for egrep... /bin/grep -E
    checking for fgrep... /bin/grep -F
    checking for ld used by gcc... /usr/bin/ld
    checking if the linker (/usr/bin/ld) is GNU ld... yes
    checking for BSD- or MS-compatible name lister (nm)... /usr/bin/nm -B
    checking the name lister (/usr/bin/nm -B) interface... BSD nm
    checking whether ln -s works... yes
    checking the maximum length of command line arguments... 1572864
    checking whether the shell understands some XSI constructs... yes
    checking whether the shell understands "+="... yes
    checking how to convert x86_64-unknown-linux-gnu file names to x86_64-unknown-linux-gnu format... func_convert_file_noop
    checking how to convert x86_64-unknown-linux-gnu file names to toolchain format... func_convert_file_noop
    checking for /usr/bin/ld option to reload object files... -r
    checking for objdump... objdump
    checking how to recognize dependent libraries... pass_all
    checking for dlltool... no
    checking how to associate runtime and link libraries... printf %s\n
    checking for ar... ar
    checking for archiver @FILE support... @
    checking for strip... strip
    checking for ranlib... ranlib
    checking command to parse /usr/bin/nm -B output from gcc object... ok
    checking for sysroot... no
    checking for mt... mt
    checking if mt is a manifest tool... no
    checking how to run the C preprocessor... gcc -E
    checking for ANSI C header files... yes
    checking for sys/types.h... yes
    checking for sys/stat.h... yes
    checking for stdlib.h... yes
    checking for string.h... yes
    checking for memory.h... yes
    checking for strings.h... yes
    checking for inttypes.h... yes
    checking for stdint.h... yes
    checking for unistd.h... yes
    checking for dlfcn.h... yes
    checking for objdir... .libs
    checking if gcc supports -fno-rtti -fno-exceptions... no
    checking for gcc option to produce PIC... -fPIC -DPIC
    checking if gcc PIC flag -fPIC -DPIC works... yes
    checking if gcc static flag -static works... yes
    checking if gcc supports -c -o file.o... yes
    checking if gcc supports -c -o file.o... (cached) yes
    checking whether the gcc linker (/usr/bin/ld -m elf_x86_64) supports shared libraries... yes
    checking whether -lc should be explicitly linked in... no
    checking dynamic linker characteristics... GNU/Linux ld.so
    checking how to hardcode library paths into programs... immediate
    checking whether stripping libraries is possible... yes
    checking if libtool supports shared libraries... yes
    checking whether to build shared libraries... yes
    checking whether to build static libraries... yes
    checking stdio.h usability... yes
    checking stdio.h presence... yes
    checking for stdio.h... yes
    checking for stdlib.h... (cached) yes
    checking for string.h... (cached) yes
    checking for size_t... yes
    checking for stdlib.h... (cached) yes
    checking for GNU libc compatible malloc... yes
    checking for stdlib.h... (cached) yes
    checking for GNU libc compatible realloc... yes
    checking for floor... no
    checking for pow... no
    checking for sqrt... no
    configure: creating ./config.status
    config.status: creating Makefile
    config.status: creating hw/Makefile
    config.status: creating libmath/Makefile
    config.status: creating config.h
    config.status: executing depfiles commands
    config.status: executing libtool commands

and now compile:

    antonio@kenny:~/src$ make
    make  all-recursive
    make[1]: Entering directory `/home/antonio/zurich/apps.git/gc3wiki/blog/Autotools_tutorial/example/src'
    Making all in libmath
    make[2]: Entering directory `/home/antonio/zurich/apps.git/gc3wiki/blog/Autotools_tutorial/example/src/libmath'
    /bin/bash ../libtool --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..     -g -O2 -MT hwmath.lo -MD -MP -MF .deps/hwmath.Tpo -c -o hwmath.lo hwmath.c
    libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -g -O2 -MT hwmath.lo -MD -MP -MF .deps/hwmath.Tpo -c hwmath.c  -fPIC -DPIC -o .libs/hwmath.o
    libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -g -O2 -MT hwmath.lo -MD -MP -MF .deps/hwmath.Tpo -c hwmath.c -o hwmath.o >/dev/null 2>&1
    mv -f .deps/hwmath.Tpo .deps/hwmath.Plo
    /bin/bash ../libtool --tag=CC   --mode=link gcc  -g -O2   -o libhwmath.la -rpath /usr/local/lib hwmath.lo
    libtool: link: gcc -shared  -fPIC -DPIC  .libs/hwmath.o    -O2   -Wl,-soname -Wl,libhwmath.so.0 -o .libs/libhwmath.so.0.0.0
    libtool: link: (cd ".libs" && rm -f "libhwmath.so.0" && ln -s "libhwmath.so.0.0.0" "libhwmath.so.0")
    libtool: link: (cd ".libs" && rm -f "libhwmath.so" && ln -s "libhwmath.so.0.0.0" "libhwmath.so")
    libtool: link: ar cru .libs/libhwmath.a  hwmath.o
    libtool: link: ranlib .libs/libhwmath.a
    libtool: link: ( cd ".libs" && rm -f "libhwmath.la" && ln -s "../libhwmath.la" "libhwmath.la" )
    make[2]: Leaving directory `/home/antonio/zurich/apps.git/gc3wiki/blog/Autotools_tutorial/example/src/libmath'
    Making all in hw
    make[2]: Entering directory `/home/antonio/zurich/apps.git/gc3wiki/blog/Autotools_tutorial/example/src/hw'
    gcc -DHAVE_CONFIG_H -I. -I..  -I. -I../libmath   -g -O2 -MT cli.o -MD -MP -MF .deps/cli.Tpo -c -o cli.o cli.c
    mv -f .deps/cli.Tpo .deps/cli.Po
    gcc -DHAVE_CONFIG_H -I. -I..  -I. -I../libmath   -g -O2 -MT hw.o -MD -MP -MF .deps/hw.Tpo -c -o hw.o hw.c
    mv -f .deps/hw.Tpo .deps/hw.Po
    /bin/bash ../libtool --tag=CC   --mode=link gcc  -g -O2 -static  -o hw cli.o hw.o ../libmath/libhwmath.la
    libtool: link: gcc -g -O2 -o hw cli.o hw.o  ../libmath/.libs/libhwmath.a
    make[2]: Leaving directory `/home/antonio/zurich/apps.git/gc3wiki/blog/Autotools_tutorial/example/src/hw'
    make[2]: Entering directory `/home/antonio/zurich/apps.git/gc3wiki/blog/Autotools_tutorial/example/src'
    make[2]: Leaving directory `/home/antonio/zurich/apps.git/gc3wiki/blog/Autotools_tutorial/example/src'
    make[1]: Leaving directory `/home/antonio/zurich/apps.git/gc3wiki/blog/Autotools_tutorial/example/src'











