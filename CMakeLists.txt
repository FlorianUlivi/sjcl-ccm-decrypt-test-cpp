cmake_minimum_required(VERSION 3.8)
project(sjcl-ccm-decrypt-test-cpp)

find_package(OpenSSL REQUIRED)

add_executable(decrypt main.cpp)
target_compile_features(decrypt PUBLIC cxx_std_14)
target_link_libraries(decrypt OpenSSL::Crypto OpenSSL::SSL)