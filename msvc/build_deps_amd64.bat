call "C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\VC\\bin\\amd64\\vcvars64.bat"
msbuild /m /v:n /p:Configuration=ReleaseDLL .\zeromq-4.2.2\builds\msvc\vs2015\libzmq\libzmq.vcxproj

