
AC_INIT([msrp],[0.9.4],[msrp-tool@googlegroups.com], [msrp], [http://msrp.github.com])
AC_CONFIG_SRCDIR(src/msrp.cc)
AM_INIT_AUTOMAKE
#AM_INIT_AUTOMAKE([-Wall -Werror foreign -Wextra -pedantic -fdiagnostics-show-option])
AC_CANONICAL_HOST

echo "relevant environment variable values, if any:"
for x in CC FC CXX CPPFLAGS CFLAGS FCFLAGS CXXFLAGS LDFLAGS LIBS ISSUE DISTRIBUTION
do eval v=\$$x
   test "$v" && echo "   $x=$v"
done


for i in $CFLAGS
do case $i in
   -I*) AC_MSG_ERROR("preprocessor flag $i found in CFLAGS instead of in CPPFLAGS") ;;
   -L*) AC_MSG_ERROR("link flag $i found in CFLAGS instead of in LDFLAGS");;
   esac
done

for i in $CXXFLAGS
do case $i in
   -I*) AC_MSG_ERROR("preprocessor flag $i found in CXXFLAGS instead of in CPPFLAGS");;
   -L*) AC_MSG_ERROR("link flag $i found in CXXFLAGS instead of in LDFLAGS");;
   esac
done

for i in $CPPFLAGS
do case $i in
   -L*) AC_MSG_ERROR("link flag $i found in CPPFLAGS instead of in LDFLAGS");;
   esac
done

for i in $LDFLAGS
do case $i in
   -I*) AC_MSG_ERROR("preprocessor flag $i found in LDFLAGS instead of in CPPFLAGS");;
   esac
done

test "$CXX" = gcc && AC_MSG_ERROR("oops, CXX=gcc, should use CXX=g++")

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
PCRE_LIBS=`$PCRE_CONFIG --libs`

AC_MSG_NOTICE([CFLAGS: $PCRE_CFLAGS])
AC_MSG_NOTICE([LIBS: $PCRE_LIBS])
         
AC_SUBST([PCRE_CFLAGS])
AC_SUBST([PCRE_LIBS])


AC_PROG_CXX

AM_MAINTAINER_MODE

AC_CONFIG_FILES([Makefile src/Makefile src/autover.h])
AC_OUTPUT
