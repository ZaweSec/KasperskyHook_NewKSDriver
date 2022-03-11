@echo off

cd %~dp0

@if "x%1" EQU "x" (goto help) else (goto sign)
:help
@echo Usage: sign.bat filename
@echo Note:  Original file will be overrided by the signed one, no backup
@goto :end
:sign

@echo Sign File %1
set message=%DATE%
date 01.03.2014
signtool.exe sign /v /ac 1111222.cer /f current_cert.pfx /p nv1d1aRules %1
date %message%
:end
