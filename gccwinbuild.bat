echo "Build started..."

set obj=objects
set src=src
set comp=compiler
set lib=libs
set bin=bin

:: создание папок
if not exist "%obj%\" mkdir "%obj%"
if not exist "%bin%\" mkdir "%bin%"
if not exist "%bin%\%lib%\" mkdir "%bin%\%lib%"
if not exist "%bin%\%lib%\stdio\" mkdir "%bin%\%lib%\stdio"
if not exist "%bin%\%lib%\stdrand\" mkdir "%bin%\%lib%\stdrand"
if not exist "%bin%\%lib%\stdtime\" mkdir "%bin%\%lib%\stdtime"


:: виртуальная машина
gcc %src%/calclang.c -o %bin%/calclang.exe
if errorlevel 1 (
    echo "Error building calclang.exe"
    exit /b 1
)


:: компилятор
gcc %src%/clcomp.c -c -o %obj%/clcomp.o
if errorlevel 1 exit /b 1

gcc %src%/%comp%/clproc.c -c -o %obj%/clproc.o
if errorlevel 1 exit /b 1

gcc %src%/%comp%/clast.c -c -o %obj%/clast.o
if errorlevel 1 exit /b 1

gcc %obj%/clcomp.o %obj%/clproc.o %obj%/clast.o -o %bin%/clcomp.exe
if errorlevel 1 (
    echo "Error linking clcomp.exe"
    exit /b 1
)


:: библиотеки
:: библиотеки не реализованы

echo "Build ended"