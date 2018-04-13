@Echo Off
SETLOCAL ENABLEDELAYEDEXPANSION

SET src_ext=.svg
SET dst_ext=.png

:: Vertex count folder.
SET vc_dir=vertex_count
IF NOT EXIST %vc_dir% (
    MKDIR %vc_dir%
)

::PhotoGeo
SET src=photogeo
SET dst=photogeo_rasterized
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
    START /WAIT rasterize.exe -i !input! -o !output!  -s 2 -v %vc_dir%\!name!_photogeo.txt
)

::Manual
SET src=manual
SET dst=manual_rasterized
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
    START /WAIT rasterize.exe -i !input! -o !output! -s 4 -v %vc_dir%\!name!_manual.txt
)
