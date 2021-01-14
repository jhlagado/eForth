rem copy this platform project to an arduino sketch at ~/arduino

mkdir .\arduino
copy -a .\include\*.* .\arduino
copy -a .\src\*.* .\arduino
rename .\arduino\main.cpp .\arduino\arduino.ino