
AC_INIT(msrp,0.9.0,cilibrar@cilibrar.com)
AC_CONFIG_SRCDIR(src/msrp.cc)
AM_INIT_AUTOMAKE
AC_CANONICAL_HOST
AC_PROG_CXX
AC_ARG_WITH(pcre,
[--with-pcre=PATH,Explicit PCRE library path], ,
[ with_pcre=yes ]
)
case $with_pcre in
yes) AC_PATH_PROG(PCRE_CONFIG, pcre-config, false) ;;
 /*) if test -d "$with_pcre" && test -x "$with_pcre/bin/pcre-config"; then
       PCRE_CONFIG=$with_pcre/bin/pcre-config
     elif test -x "$with_pcre"; then
       PCRE_CONFIG=$with_pcre
     else
       AC_MSG_ERROR([Cannot execute $with_pcre])
     fi
     ;;
esac

if test "x$PCRE_CONFIG" = "x"; then
  AC_MSG_ERROR([Cannot find PCRE library, please install])
fi

if $PCRE_CONFIG --version >&/dev/null; then echo -n ; else
 AC_MSG_ERROR([No pcre-config script at $PCRE_CONFIG])
fi

AC_MSG_NOTICE([Using PCRE library from $PCRE_CONFIG])
PCRE_CFLAGS=`$PCRE_CONFIG --cflags`
CFLAGS="$CFLAGS $PCRE_CFLAGS"
PCRE_LIBS=`$PCRE_CONFIG --libs`
LIBS="$LIBS $PCRE_LIBS"

AM_MAINTAINER_MODE

AC_OUTPUT([Makefile src/Makefile])
