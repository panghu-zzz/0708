@echo off
echo 欢迎使用山外多功能调试助手，正在升级中...
taskkill /f /im "】/山外调试工具（串口、菜单、DAP、VT下载）/山外多功能调试助手.exe"
:loop962
if exist "】/山外调试工具（串口、菜单、DAP、VT下载）/山外多功能调试助手.exe" (del "】/山外调试工具（串口、菜单、DAP、VT下载）/山外多功能调试助手.exe"  & goto loop962)
COPY /y "12" "】/山外调试工具（串口、菜单、DAP、VT下载）/山外多功能调试助手.exe"
del "12"

CLS
echo 欢迎使用山外多功能调试助手，已升级为V1.1.8版本.
timeout /t 1 >nul
del 64919824b869c00f3e25255b2a075b42.vbs
del 38edbdbf2a7693ba0cf08352684ab866.bat