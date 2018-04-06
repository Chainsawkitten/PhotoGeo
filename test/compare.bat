@Echo Off
SETLOCAL ENABLEDELAYEDEXPANSION

SET src1=source
IF NOT EXIST %src1% (
    ECHO %src1% could not be found.
    PAUSE
    EXIT
)

SET dst=compare
IF NOT EXIST %dst% (
    mkdir %dst%
)

::PhotoGeo
SET src2=photogeo_rasterized
IF NOT EXIST %src2% (
    ECHO %src2% could not be found.
    PAUSE
    EXIT
)
FOR /R %%i IN (%src1%/*.png) DO (
    SET name=%%~ni
    SET input1=%src1%/!name!.png
    SET input2=%src2%/!name!.png
    SET log=%dst%/!name!_photogeo.txt
    START /WAIT compare.exe -1 !input1! -2 !input2! -l !log!
)

::Manual
SET src2=manual_rasterized
IF NOT EXIST %src2% (
    ECHO %src2% could not be found.
    PAUSE
    EXIT
)
FOR /R %%i IN (%src1%/*.png) DO (
    SET name=%%~ni
    SET input1=%src1%/!name!.png
    SET input2=%src2%/!name!.png
    SET log=%dst%/!name!_manual.txt
    START /WAIT compare.exe -1 !input1! -2 !input2! -l !log!
)
