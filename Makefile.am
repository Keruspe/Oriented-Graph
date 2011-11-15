ACLOCAL_AMFLAGS = -I m4

LIBDIGRAPH_CURRENT=0
LIBDIGRAPH_REVISION=0
LIBDIGRAPH_AGE=0

AM_CXXFLAGS = -Wall -Wextra -Wno-unused-but-set-variable -Wno-unused-parameter --include config.hh -O3 -march=native
AM_LDFLAGS = -Wl,--as-needed -Wl,-O3

bin_PROGRAMS = oriented-graph-test

digraph_libdir = $(libdir)/digraph
digraph_lib_LTLIBRARIES = libdigraph.la

digraph_includedir = $(includedir)/digraph
digraph_include_HEADERS = digraph/digraph.hh digraph/array-digraph.hh digraph/list-digraph.hh

CLEANFILES =
EXTRA_DIST = $(digraph_include_HEADERS)

libdigraph_la_SOURCES = \
	digraph/digraph.cc \
	digraph/array-digraph.cc \
	digraph/list-digraph.cc

libdigraph_la_LDFLAGS = $(AM_LDFLAGS) -shared -version-info $(LIBDIGRAPH_CURRENT):$(LIBDIGRAPH_REVISION):$(LIBDIGRAPH_AGE)

oriented_graph_test_SOURCES = src/oriented-graph-test.cc

oriented_graph_test_LDADD = libdigraph.la

install-data-hook: la-files-removal

la-files-removal:
	cd $(DESTDIR)/$(digraph_libdir) && \
		rm $(digraph_lib_LTLIBRARIES)
