#!/bin/sh

set -e

basedir=`cd $(dirname $0); pwd`
prefix=$basedir/install
builddir=$basedir/build
MAKEOPTS="-j4"

GCCVER=4.8.2 # version for gcc
GCCMD5="a3d7d63b9cb6b6ea049469a0c4a43c9d" # md5sum for gcc
BINVER=2.24 # version for binutils
BINMD5="e0f71a7b2ddab0f8612336ac81d9636b" # md5sum for binutils

# some utility functions
info() {
	echo -e ' * Info: \033[32;1m'"$@"'\033[m'
}
warn() {
	echo -e ' + Warning: \033[33;1m'"$@"'\033[m'
}
err() {
	echo -e ' ! Error: \033[31;1m'"$@"'\033[m'
}
md5cksum () {
	md5sum "$1" 2>/dev/null | cut -d" " -f1
}
download() { # args: URL filename md5
	while [ "`md5cksum $2`" != $3 ]; do
		warn bad md5 checksum: $2, will re-download.
		rm -f "$1"
		wget -q -O "$2" "$1"
	done
	info "Downloaded $2."
}

mkdir -p $builddir $prefix

cd $builddir

gccbase="gcc-${GCCVER}"
gccfile="${gccbase}.tar.bz2"
download "http://ftp.gnu.org/gnu/gcc/gcc-$GCCVER/$gccfile" "$gccfile" $GCCMD5
binbase="binutils-${BINVER}"
binfile="${binbase}.tar.bz2"
download "http://ftp.gnu.org/gnu/binutils/$binfile" "$binfile" $BINMD5

rm -rf $gccbase/ $binbase/
tar xf $gccfile
tar xf $binfile

cd ${binbase}
mkdir -p build
cd build
info Building binutils.
../configure --target=c6x-none-elf --prefix=$prefix
make $MAKEOPTS
make install
cd ../..

cd ${gccbase}
./contrib/download_prerequisites
mkdir -p build
cd build
info Building gcc.
../configure --target=c6x-none-elf --prefix=$prefix \
   --enable-languages=c --disable-shared --disable-libssp
make $MAKEOPTS
make install
cd ../..

info Done.
