IF NOT EXIST build ( mkdir build )
cd build
cmake -G "Visual Studio 15 2017 Win64" ..\src
cd ..
IF NOT EXIST saves ( mkdir saves )

