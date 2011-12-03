ACLOCAL_AMFLAGS = -I m4

LIBDIGRAPH_CURRENT=0
LIBDIGRAPH_REVISION=0
LIBDIGRAPH_AGE=0

AM_CXXFLAGS = -Wall -Wextra -Wno-unused-but-set-variable -Wno-unused-parameter --include config.hh -O3 -march=native
AM_LDFLAGS = -Wl,--as-needed -Wl,-O3

bin_PROGRAMS = digraph-test gps

pkglib_LTLIBRARIES = libdigraph.la
pkginclude_HEADERS = digraph/digraph.hh digraph/array-digraph.hh digraph/list-digraph.hh

CLEANFILES =
EXTRA_DIST = $(plginclude_HEADERS)

libdigraph_la_SOURCES = \
	digraph/digraph.cc \
	digraph/array-digraph.cc \
	digraph/list-digraph.cc

libdigraph_la_LDFLAGS = $(AM_LDFLAGS) -shared -version-info $(LIBDIGRAPH_CURRENT):$(LIBDIGRAPH_REVISION):$(LIBDIGRAPH_AGE)

digraph_test_SOURCES = src/digraph-test.cc
digraph_test_LDADD = libdigraph.la

gps_SOURCES = src/gps-main.cc src/gps.cc src/map-parser.cc
gps_LDADD = libdigraph.la

