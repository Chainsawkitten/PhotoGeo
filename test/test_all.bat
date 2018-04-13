@Echo Off
SETLOCAL ENABLEDELAYEDEXPANSION

:: Perturb source files.
CALL perturb.bat

:: Run all configurations.
CALL test.bat -q1 -t0 -v0
CALL test.bat -q1 -t0 -v1
CALL test.bat -q1 -t0 -v2

CALL test.bat -q4 -t0 -v0
CALL test.bat -q4 -t0 -v1
CALL test.bat -q4 -t0 -v2

CALL test.bat -p2 -p1 -q1 -t0 -v0
CALL test.bat -p2 -p1 -q1 -t0 -v1
CALL test.bat -p2 -p1 -q1 -t0 -v2

CALL test.bat -p2 -p1 -q4 -t0 -v0
CALL test.bat -p2 -p1 -q4 -t0 -v1
CALL test.bat -p2 -p1 -q4 -t0 -v2

CALL test.bat -p2 -p3 -q1 -t0 -v0
CALL test.bat -p2 -p3 -q1 -t0 -v1
CALL test.bat -p2 -p3 -q1 -t0 -v2

CALL test.bat -p2 -p3 -q4 -t0 -v0
CALL test.bat -p2 -p3 -q4 -t0 -v1
CALL test.bat -p2 -p3 -q4 -t0 -v2
