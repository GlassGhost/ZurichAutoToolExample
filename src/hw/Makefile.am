# Defines which program executables will be produced.
bin_PROGRAMS = hw

# Defines which source files are used to produce the specificed
# executable.
hw_SOURCES = cli.c hw.c

# Defines flags for the C preprocessor *specific to ``hw``*.
# hw_CPPLFAGS = 

# These are instead *default* flags, used by all the programs in
# ``bin_PROGRAMS``.
AM_CPPFLAGS = -I$(srcdir) -I$(top_srcdir)/libmath

# Extra options for the linker.
# hw_LDFLAGS = 

# Default options for the linker OR for libtool (depending on what you
# put in ``configure.ac``), used by all the programs in
# ``bin_PROGRAMS``.
#
# In this case, we want to statically link the libraries in
# ``hw_LDADD`.
AM_LDFLAGS = -static

# Libraries the ``hw`` binary will be linked to.
hw_LDADD = $(top_srcdir)/libmath/libhwmath.la
