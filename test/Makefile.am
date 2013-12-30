TESTS = check_ace
check_PROGRAMS = check_ace

check_ace_SOURCES = \
  check_ace_testsuite.c \
  check_ace_string.c           check_ace_string.h \
  check_ace_fs.c               check_ace_fs.h \
  check_ace_index_filesize.c   check_ace_index_filesize.h \
  ace_testutil_fs.c            ace_testutil_fs.h \
  $(top_builddir)/src/ace_errcode.h \
  $(top_builddir)/src/ace_string.h \
  $(top_builddir)/src/ace_fs.h \
  $(top_builddir)/src/ace_index_filesize.h
  
check_ace_CFLAGS = @CHECK_CFLAGS@
check_ace_LDADD = $(top_builddir)/src/libace.la @CHECK_LIBS@
