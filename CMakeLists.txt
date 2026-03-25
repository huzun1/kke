cmake_minimum_required(VERSION 3.10.0)
project(kke VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_subdirectory(tests)

set(KKE_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/kke")

configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/kkeConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/kkeConfig.cmake"
	INSTALL_DESTINATION "${KKE_INSTALL_CMAKEDIR}")

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/kkeConfigVersion.cmake"
	VERSION "${PROJECT_VERSION}"
	COMPATIBILITY SameMajorVersion)

install(
	EXPORT kkeTargets
	FILE kkeTargets.cmake
	NAMESPACE kke::
	DESTINATION "${KKE_INSTALL_CMAKEDIR}")

install(
	FILES
		"${CMAKE_CURRENT_BINARY_DIR}/kkeConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/kkeConfigVersion.cmake"
	DESTINATION "${KKE_INSTALL_CMAKEDIR}")
