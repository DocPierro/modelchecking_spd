
noinst_LTLIBRARIES = libeval.la
libeval_la_SOURCES = \
	Eval.cpp Eval.hpp \
	Eval-scanner.ll Eval-parser.yy

BUILT_SOURCES = Eval-parser.cc

EXTRA_DIST = Eval-parser.hh location.hh position.hh stack.hh

AM_YFLAGS = -y -p eval
AM_LFLAGS = -o$(LEX_OUTPUT_ROOT).c --prefix=eval
AM_CPPFLAGS =  -Wall -Wextra -Wno-unused -Wno-deprecated -Wno-write-strings -Wno-unneeded-internal-declaration
