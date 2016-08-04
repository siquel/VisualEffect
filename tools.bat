call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64

msbuild .build\projects\vs2015\shaderc.vcxproj /target:Build /property:Configuration=Release;Platform=x64

echo f | xcopy .build\win64_vs2015\bin\shadercRelease.exe tools\bin\shaderc.exe /f /y
