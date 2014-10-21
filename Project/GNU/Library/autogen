#libtoolize 
if test "$(uname)" = "Darwin" ; then
  #Darwin based Systems like Mac OS X: libtoolize is called glibtoolize.
  glibtoolize --automake
else
  libtoolize --automake
fi

#aclocal
if test -e /usr/bin/aclocal-1.11 ; then
  #OpenSolaris: no aclocal
  aclocal-1.11
elif test -e /usr/bin/aclocal-1.10 ; then
  aclocal-1.10
else
  aclocal
fi

#automake
if test -e /usr/bin/automake-1.11 ; then
  #OpenSolaris: no automake
  automake-1.11 -a
elif test -e /usr/bin/automake-1.10 ; then
  automake-1.10 -a
else
  automake -a
fi

autoreconf -fi
