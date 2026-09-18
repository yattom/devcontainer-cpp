cmake_minimum_required(VERSION 3.28)

project(bad_trainroute LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# compile_commands.json を出力しておくと、C/C++ 拡張の補完が安定する
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------
# プロダクションコード
# ------------------------------------------------------------
add_library(trainroute
    src/bad/trainroute/domain/data_store.cpp
    src/bad/trainroute/domain/route_searcher.cpp
    src/bad/trainroute/domain/train_system.cpp
    src/bad/trainroute/domain/transfer_handler.cpp
)
target_include_directories(trainroute PUBLIC src)

add_library(hello_world src/hello_world.cpp)
target_include_directories(hello_world PUBLIC src)

add_executable(trainroute_app src/bad/trainroute/domain/main.cpp)
target_link_libraries(trainroute_app PRIVATE trainroute)

# ------------------------------------------------------------
# テスト（GoogleTest + GoogleMock）
#
# バージョンはコミットハッシュで固定する。タグは付け替えられる可能性が
# あるため、再現性が必要なピン留めにはハッシュを使う。
# 更新するときは README のバージョン表も合わせて変更すること。
# ------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 063de7e9578f82b369302001269680b4b1553359  # v1.18.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests
    tests/hello_world_test.cpp
    tests/gmock_examples_test.cpp
)
target_link_libraries(unit_tests PRIVATE hello_world trainroute GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
