echo off

SET JAVA_HOME=C:\j2sdk1.4.2_04
SET CLASSPATH=FTAGUI.jar;images

SET PATH=%PATH%;%JAVA_HOME%\bin
REM SET PATH=%PATH%;.

%JAVA_HOME%\bin\java.exe -Xmx256m FTAGUI.OpenFTA

pause
