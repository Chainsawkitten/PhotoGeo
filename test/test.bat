@Echo Off
SETLOCAL ENABLEDELAYEDEXPANSION

:: Run test.
CALL photogeo_profile.bat %*
CALL rasterize.bat
CALL compare.bat

:: Store results.
MKDIR "results/%*"
MOVE compare "results/%*/compare"
MOVE photogeo_profile "results/%*/photogeo_profile"
MOVE vertex_count "results/%*/vertex_count"
