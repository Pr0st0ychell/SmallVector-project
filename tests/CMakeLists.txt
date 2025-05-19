# Подключаем Google Test
find_package(GTest REQUIRED)

# Тестовое приложение
add_executable(smallvector_tests smallvector_tests.cpp ../include/SmallVector.h)
target_link_libraries(smallvector_tests PRIVATE GTest::GTest GTest::Main)