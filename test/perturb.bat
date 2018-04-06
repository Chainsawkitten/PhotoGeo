@Echo Off
SETLOCAL ENABLEDELAYEDEXPANSION

SET src=source
SET dst=perturb
SET src_ext=.png
SET dst_ext=.png

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
    START /WAIT perturb.exe -i !input! -o !output!
)
