
if not exist "build" (
    mkdir build
) else (
    rmdir /s /q build
    mkdir build
)

cd build

cmake ..

cmake --build . --config Release