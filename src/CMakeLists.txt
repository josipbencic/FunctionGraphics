add_executable(graphics Main.cpp)

target_compile_features(graphics PUBLIC cxx_constexpr)
target_include_directories(graphics PRIVATE ../glm)

if(MSVC)
  add_compile_options(/W3 /WX)
else()
  add_compile_options(-W -Wall -Werror)
endif()

