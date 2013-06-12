#! /bin/sh

JAVAHOME=/usr/local/j2sdk1.4.2_04
export JAVAHOME

LD_LIBRARY_PATH=.:$JAVAHOME/jre/lib/i386::$JAVAHOME/jre/lib/i386/client:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH

$JAVAHOME/bin/java -cp ./FTAGUI.jar:$JAVAHOME/lib/tools.jar:$JAVAHOME/lib/dt.jar FTAGUI.OpenFTA
