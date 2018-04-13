@Echo Off
SETLOCAL ENABLEDELAYEDEXPANSION

CALL photogeo_profile.bat %*
CALL rasterize.bat
CALL compare.bat
