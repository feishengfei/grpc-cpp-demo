# Introduction

This is a grpc c++ demo which depends on grpc already compiled and installed(don't care about protobuf | grpc installed by yum or apt).

CMakeLists.txt is inspired by [grpc-cpp](https://github.com/chungphb/grpc-cpp).



# Clone grpc and compile/install to ${HOME}/grpc

```shell
git clone --recurse-submodules -b v1.52.1 https://github.com/grpc/grpc grpc_v1.52.1
pushd grpc_v1.52.1
git submodule update --init
# please make sure source code dir is clear, no non-staged log file either.

# compile grpc(will compile/install third-party zlib|protobuf into CMAKE_INSTALL_PREFIX too)
cmake -DgRPC_INSTALL=ON -DCMAKE_INSTALL_PREFIX=${HOME}/grpc -DgRPC_BUILD_TESTS=OFF -DgRPC_BUILD_GRPC_CPP_PLUGIN=ON -B build && cmake --build build -- install -j `nproc`
popd

# BTW, be care to install above output into ~/.local/, where a lot of system config/cache 
# was stored there, such as Zeal & gnome-chrome-extensions

# protoc installed into CMAKE_INSTALL_PREFIX, so you may need to export PATH with prefix location too.
```



# Compile this project

```shell
# make sure installed protoc is in your PATH by
export PATH=${HOME}/grpc/bin:$PATH

# compile this project(source code inspired by chat GPT)
cmake -B build && cmake --build build -- -j `nproc`
```

