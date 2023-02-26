# inspired by git@github.com:chungphb/grpc-cpp.git

cmake_minimum_required(VERSION 3.0)

project(grpc_demo)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
else()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

get_filename_component(my_service_proto "${CMAKE_SOURCE_DIR}/protos/my_service.proto" ABSOLUTE)
get_filename_component(my_service_proto_path "${my_service_proto}" PATH)

set(my_service_proto_sources "${CMAKE_CURRENT_BINARY_DIR}/my_service.pb.cc")
set(my_service_proto_headers "${CMAKE_CURRENT_BINARY_DIR}/my_service.pb.h")
set(my_service_grpc_sources "${CMAKE_CURRENT_BINARY_DIR}/my_service.grpc.pb.cc")
set(my_service_grpc_headers "${CMAKE_CURRENT_BINARY_DIR}/my_service.grpc.pb.h")

add_custom_command(
        OUTPUT "${my_service_proto_sources}" "${my_service_proto_headers}" "${my_service_grpc_sources}" "${my_service_grpc_headers}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}" --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${my_service_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${my_service_proto}"
        DEPENDS "${my_service_proto}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_library(my_service_grpc_proto
        ${my_service_proto_sources}
        ${my_service_proto_headers}
        ${my_service_grpc_sources}
        ${my_service_grpc_headers})
target_link_libraries(my_service_grpc_proto
        ${_PROTOBUF_LIBPROTOBUF}
        ${_REFLECTION}
        ${_GRPC_GRPCPP})

foreach(_target   client server)
    add_executable(${_target} "${_target}.cc")
    target_link_libraries(${_target}
            my_service_grpc_proto
            ${_PROTOBUF_LIBPROTOBUF}
            ${_REFLECTION}
            ${_GRPC_GRPCPP})
endforeach()

