lib_LTLIBRARIES = libace.la

libace_la_SOURCES = \
  ace_errcode.c         ace_errcode.h \
  ace_string.c          ace_string.h \
  ace_fs.c              ace_fs.h \
  ace_index_filesize.c  ace_index_filesize.h

bin_PROGRAMS = $(top_builddir)/bin/ace
__top_builddir__bin_ace_SOURCES = main.c
__top_builddir__bin_ace_LDADD = libace.la
