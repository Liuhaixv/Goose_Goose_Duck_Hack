echo
reg delete "HKEY_CURRENT_USER\Software\Gaggle Studios INC"  /f
reg delete "HKEY_CURRENT_USER\Software\Classes\gaggleggd"  /f

del /f /s /q "%AppData%\..\Roaming\MsSqlLib" 1>nul
rmdir /s /q "%AppData%\..\Roaming\MsSqlLib"

del /f /s /q "%AppData%\..\LocalLow\Gaggle Studios INC" 1>nul
rmdir /s /q "%AppData%\..\LocalLow\Gaggle Studios INC"

pause