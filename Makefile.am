ACLOCAL_AMFLAGS = -I m4

LIBGRAPH_CURRENT=0
LIBGRAPH_REVISION=0
LIBGRAPH_AGE=0

AM_CXXFLAGS = -Wall -Wextra -Wno-unused-but-set-variable -Wno-unused-parameter --include config.hh -O3 -march=native
AM_LDFLAGS = -Wl,--as-needed -Wl,-O3
#	-I $(top_srcdir)/include/ \
#	-I $(top_builddir)/include/

bin_PROGRAMS = oriented-graph-test
pkglib_LTLIBRARIES = libgraph.la

CLEANFILES =

libgraph_la_SOURCES = \
	graph/graph.cc \
	graph/array-graph.cc \
	graph/list-graph.cc

libgraph_la_LDFLAGS = $(AM_LDFLAGS) -shared -version-info $(LIBGRAPH_CURRENT):$(LIBGRAPH_REVISION):$(LIBGRAPH_AGE)

oriented_graph_test_SOURCES = src/oriented-graph-test.cc

oriented_graph_test_LDADD = libgraph.la

install-data-hook: la-files-removal

la-files-removal:
	cd $(DESTDIR)/$(pkglibdir) && \
		rm $(pkglib_LTLIBRARIES)
