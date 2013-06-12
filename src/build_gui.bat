echo off

REM Set this variable to point to the java installation 
SET JAVA_HOME=C:\j2sdk1.4.2_04

cd gui/FTAGUI

REM compile the java
%JAVA_HOME%\bin\javac *.java

FOR %%A IN (*.java) DO (
  %JAVA_HOME%\bin\javah -classpath .. FTAGUI.%%~nA
)

cd ..

%JAVA_HOME%\bin\jar -cvf FTAGUI.jar FTAGUI\*.class

cd images

%JAVA_HOME%\bin\jar -cvf ..\images.jar *.gif

REM pause
