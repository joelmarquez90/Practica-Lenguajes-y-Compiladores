tasm  /la  /zi  %1
IF ERRORLEVEL 1 GOTO EXIT
tlink /v %1
IF ERRORLEVEL 1 GOTO EXIT
TD -cspecial %1
:EXIT
