#! /bin/sh

JAVAHOME=/usr/lib/jvm/java-6-openjdk-amd64/
export JAVAHOME

LD_LIBRARY_PATH=.:$JAVAHOME/jre/lib/amd64::$JAVAHOME/jre/lib/amd64/client:../src/lib/JNI:$JAVAHOME/jre/lib/amd64/xawt:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH

$JAVAHOME/bin/java -cp /home/mark/Downloads/OpenFTA-git/src/gui/FTAGUI.jar:$JAVAHOME/lib/tools.jar:$JAVAHOME/lib/dt.jar FTAGUI.OpenFTA
