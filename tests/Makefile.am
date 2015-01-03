#
# Automake file for building unit tests.
#

DEFS = @DEFS@ -DVERSION=\"@VERSION@\" -DDICTIONARY_DIR=\"$(pkgdatadir)\"

# $(top_builddir) to pick up autogened link-grammar/link-features.h
AM_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir) $(CXX_FLAGS) \
    $(WARN_CXXFLAGS)

# -----------------------------------------------------------
# Directives to build the threading unit test.
bin_PROGRAMS=multi-thread
multi_thread_SOURCES = multi-thread.cc

multi_thread_LDADD = $(top_builddir)/link-grammar/liblink-grammar.la
if HAVE_SQLITE
multi_thread_LDADD += $(SQLITE3_LIBS)
endif

if WITH_SAT_SOLVER
multi_thread_LDADD  += $(top_builddir)/link-grammar/sat-solver/libsat-solver.la
multi_thread_LDADD  += $(top_builddir)/link-grammar/minisat/libminisat.la
endif

# -----------------------------------------------------------
# Directives to build the threading unit test.
bin_PROGRAMS += dict-reopen
dict_reopen_SOURCES = dict-reopen.cc

dict_reopen_LDADD = $(top_builddir)/link-grammar/liblink-grammar.la
if HAVE_SQLITE
dict_reopen_LDADD += $(SQLITE3_LIBS)
endif

if WITH_SAT_SOLVER
dict_reopen_LDADD  += $(top_builddir)/link-grammar/sat-solver/libsat-solver.la
dict_reopen_LDADD  += $(top_builddir)/link-grammar/minisat/libminisat.la
endif
