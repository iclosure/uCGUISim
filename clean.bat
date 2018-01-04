@ECHO OFF

ECHO CleanUp.BAT:         Deleting output

REM ****************************************
REM   Delete working folders
REM ****************************************

IF "%OS%" == "Windows_NT" GOTO WinNT
FOR %%i IN (Output, Source, Debug, Release) DO DELTREE %%i %%i
GOTO CONT2
:WinNT
FOR %%i IN (Output, Source, Debug, Release, ipch,_UpgradeReport_Files) DO IF EXIST %%i RD %%i /S/Q
:CONT2

REM ****************************************
REM   Delete files
REM ****************************************

FOR %%i IN (plg, aps, ncb, log, opt, user,xml,sdf,sln,suo) DO IF EXIST *.%%i DEL *.%%i