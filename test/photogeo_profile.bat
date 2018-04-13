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

SET it=5
SET log_dst=photogeo_profile
IF NOT EXIST %log_dst% (
    mkdir %log_dst%
)

::Profile time
FOR /R %%i IN (%src%/*%src_ext%) DO (
    SET name=%%~ni
    SET input=%src%/!name!%src_ext%
    SET output=%dst%/!name!%dst_ext%
    SET log=%log_dst%/!name!_time.txt
    START /WAIT PhotoGeoCmd.exe -i !input! -o !output! -b 162:152:155 -f 35:29:32 -f 167:34:44 -li %it% -lo !log! -pt %*
)

::Profile memory
FOR /R %%i IN (%src%/*%src_ext%) DO (
    SET name=%%~ni
    SET input=%src%/!name!%src_ext%
    SET output=%dst%/!name!%dst_ext%
    SET log=%log_dst%/!name!_memory.txt
    START /WAIT PhotoGeoCmd.exe -i !input! -o !output! -b 162:152:155 -f 35:29:32 -f 167:34:44 -li %it% -lo !log! -pm %*
)
