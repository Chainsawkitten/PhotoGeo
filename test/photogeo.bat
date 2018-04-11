@Echo Off
SETLOCAL ENABLEDELAYEDEXPANSION

SET src=perturb
SET dst=photogeo
SET src_ext=.png
SET dst_ext=.svg

IF NOT EXIST %src% (
    ECHO %src% could not be found.
    PAUSE
    EXIT
)

IF NOT EXIST %dst% (
    mkdir %dst%
)

FOR /R %%i IN (%src%/*%src_ext%) DO (
    SET name=%%~ni
    SET input=%src%/!name!%src_ext%
    SET output=%dst%/!name!%dst_ext%
    START /WAIT PhotoGeoCmd.exe -i !input! -o !output! -b 162:152:155 -f 35:29:32 -f 167:34:44
)
