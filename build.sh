set -e
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --target=DummyEditor --config=Release -j $(nproc --all)
cd ../Sandbox
./DummyEditor