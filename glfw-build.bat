if not exist glfw-build mkdir glfw-build
pushd glfw-build
cmake -DBUILD_SHARED_LIBS=ON ..\glfw-3.1.1
msbuild /v:m /p:Configuration="Release" src\glfw.vcxproj
popd
