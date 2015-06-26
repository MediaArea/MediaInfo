#! /bin/bash

##########################################################################
Parallel_Build () {
	local numprocs=1
	echo $numprocs
	if [ "$OS" == "mac" ]; then
		if type sysctl &> /dev/null; then
			numprocs=`sysctl -n hw.ncpu`
			if [ "$numprocs" = "" ] || [ "$numprocs" = "0" ]; then
				numprocs=1
			fi
		fi
	elif [ "$OS" == "linux" ]; then
		if test -e /proc/stat; then
			numprocs=`grep -c ^processor /proc/cpuinfo 2>/dev/null`
			if [ "$numprocs" = "" ] || [ "$numprocs" = "0" ]; then
				numprocs=1
			fi
		fi
	fi
	#elif [ "$OS" == "solaris" ]; then
		# on Solaris you need to use psrinfo -p instead
	#fi
	make -s -j$numprocs
}

##########################################################################
ZenLib () {
	if test -e ZenLib/Project/GNU/Library/configure; then
		cd ZenLib/Project/GNU/Library/
		test -e Makefile && rm Makefile
		chmod u+x configure
		./configure $ZenLib_Options $*
		if test -e Makefile; then
			make clean
			Parallel_Build
			if test -e libzen.la; then
				echo ZenLib compiled
			else
				echo Problem while compiling ZenLib
				exit
			fi
		else
			echo Problem while configuring ZenLib
			exit
		fi
	else
		echo ZenLib directory is not found
		exit
	fi
	cd $Home
}

##########################################################################
MediaInfoLib () {
	if test -e MediaInfoLib/Project/GNU/Library/configure; then
		cd MediaInfoLib/Project/GNU/Library/
		test -e Makefile && rm Makefile
		chmod u+x configure
		./configure $*
		if test -e Makefile; then
			make clean
			Parallel_Build
			if test -e libmediainfo.la; then
				echo MediaInfoLib compiled
			else
				echo Problem while compiling MediaInfoLib
				exit
			fi
		else
			echo Problem while configuring MediaInfoLib
			exit
		fi
	else
		echo MediaInfoLib directory is not found
		exit
	fi
	cd $Home
}

##########################################################################
# MediaInfo
MediaInfo () {
	if test -e MediaInfo/Project/GNU/GUI/configure; then
		cd MediaInfo/Project/GNU/GUI/
		test -e Makefile && rm Makefile
		chmod u+x configure
		./configure $*
		if test -e Makefile; then
			make clean
			Parallel_Build
			if test -e mediainfo-gui; then
				echo MediaInfo compiled
			else
				echo Problem while compiling MediaInfo
				exit
			fi
		else
			echo Problem while configuring MediaInfo
			exit
		fi
	else
		echo MediaInfo directory is not found
		exit
	fi
	cd $Home
}

#########################################################################

Home=`pwd`
ZenLib_Options=" --without-subdirs --enable-gui"

uname=$(uname -s)
# expr isn’t available on mac
if [ "$uname" == "Darwin" ]; then
	OS="mac"
# if the 5 first caracters of $OS equal "Linux"
elif [ "$(expr substr $uname 1 5)" == "Linux" ]; then
	OS="linux"
#elif [ "$(expr substr $uname 1 7)" == "Solaris" ]; then
	#OS="solaris"
fi

ZenLib
MediaInfoLib
MediaInfo

echo `pwd`
echo $Home
cd $Home
echo "MediaInfo executable is in MediaInfo/Project/GNU/GUI"

unset -v Home ZenLib_Options OS
