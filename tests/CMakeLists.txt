cmake_minimum_required(VERSION 3.10.0)

add_executable(kke_unit_tests
	Main.cc)

target_link_libraries(kke_unit_tests PRIVATE kke)

add_test(NAME kke_unit_tests COMMAND kke_unit_tests)
