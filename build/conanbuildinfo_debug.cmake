
#################
###  LIBPQXX
#################
set(CONAN_LIBPQXX_ROOT_DEBUG "C:/Users/trafe_ccwwt/.conan/data/libpqxx/7.7.4/_/_/package/e0064ca83ef08ab777f660e93ce9416dd8009f96")
set(CONAN_INCLUDE_DIRS_LIBPQXX_DEBUG "C:/Users/trafe_ccwwt/.conan/data/libpqxx/7.7.4/_/_/package/e0064ca83ef08ab777f660e93ce9416dd8009f96/include")
set(CONAN_LIB_DIRS_LIBPQXX_DEBUG "C:/Users/trafe_ccwwt/.conan/data/libpqxx/7.7.4/_/_/package/e0064ca83ef08ab777f660e93ce9416dd8009f96/lib")
set(CONAN_BIN_DIRS_LIBPQXX_DEBUG )
set(CONAN_RES_DIRS_LIBPQXX_DEBUG )
set(CONAN_SRC_DIRS_LIBPQXX_DEBUG )
set(CONAN_BUILD_DIRS_LIBPQXX_DEBUG )
set(CONAN_FRAMEWORK_DIRS_LIBPQXX_DEBUG )
set(CONAN_LIBS_LIBPQXX_DEBUG pqxx)
set(CONAN_PKG_LIBS_LIBPQXX_DEBUG pqxx)
set(CONAN_SYSTEM_LIBS_LIBPQXX_DEBUG wsock32 ws2_32)
set(CONAN_FRAMEWORKS_LIBPQXX_DEBUG )
set(CONAN_FRAMEWORKS_FOUND_LIBPQXX_DEBUG "")  # Will be filled later
set(CONAN_DEFINES_LIBPQXX_DEBUG )
set(CONAN_BUILD_MODULES_PATHS_LIBPQXX_DEBUG )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_LIBPQXX_DEBUG )

set(CONAN_C_FLAGS_LIBPQXX_DEBUG "")
set(CONAN_CXX_FLAGS_LIBPQXX_DEBUG "")
set(CONAN_SHARED_LINKER_FLAGS_LIBPQXX_DEBUG "")
set(CONAN_EXE_LINKER_FLAGS_LIBPQXX_DEBUG "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_LIBPQXX_DEBUG_LIST "")
set(CONAN_CXX_FLAGS_LIBPQXX_DEBUG_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_LIBPQXX_DEBUG_LIST "")
set(CONAN_EXE_LINKER_FLAGS_LIBPQXX_DEBUG_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_LIBPQXX_DEBUG "${CONAN_FRAMEWORKS_LIBPQXX_DEBUG}" "_LIBPQXX" "_DEBUG")
# Append to aggregated values variable
set(CONAN_LIBS_LIBPQXX_DEBUG ${CONAN_PKG_LIBS_LIBPQXX_DEBUG} ${CONAN_SYSTEM_LIBS_LIBPQXX_DEBUG} ${CONAN_FRAMEWORKS_FOUND_LIBPQXX_DEBUG})


#################
###  BOOST
#################
set(CONAN_BOOST_ROOT_DEBUG "C:/.conan/695e9b/1")
set(CONAN_INCLUDE_DIRS_BOOST_DEBUG "C:/.conan/695e9b/1/include")
set(CONAN_LIB_DIRS_BOOST_DEBUG "C:/.conan/695e9b/1/lib")
set(CONAN_BIN_DIRS_BOOST_DEBUG )
set(CONAN_RES_DIRS_BOOST_DEBUG )
set(CONAN_SRC_DIRS_BOOST_DEBUG )
set(CONAN_BUILD_DIRS_BOOST_DEBUG )
set(CONAN_FRAMEWORK_DIRS_BOOST_DEBUG )
set(CONAN_LIBS_BOOST_DEBUG libboost_contract libboost_coroutine libboost_fiber_numa libboost_fiber libboost_context libboost_graph libboost_iostreams libboost_json libboost_locale libboost_log_setup libboost_log libboost_math_c99 libboost_math_c99f libboost_math_c99l libboost_math_tr1 libboost_math_tr1f libboost_math_tr1l libboost_nowide libboost_program_options libboost_random libboost_regex libboost_stacktrace_noop libboost_stacktrace_windbg libboost_stacktrace_windbg_cached libboost_timer libboost_type_erasure libboost_thread libboost_chrono libboost_container libboost_date_time libboost_unit_test_framework libboost_prg_exec_monitor libboost_test_exec_monitor libboost_exception libboost_wave libboost_filesystem libboost_atomic libboost_wserialization libboost_serialization)
set(CONAN_PKG_LIBS_BOOST_DEBUG libboost_contract libboost_coroutine libboost_fiber_numa libboost_fiber libboost_context libboost_graph libboost_iostreams libboost_json libboost_locale libboost_log_setup libboost_log libboost_math_c99 libboost_math_c99f libboost_math_c99l libboost_math_tr1 libboost_math_tr1f libboost_math_tr1l libboost_nowide libboost_program_options libboost_random libboost_regex libboost_stacktrace_noop libboost_stacktrace_windbg libboost_stacktrace_windbg_cached libboost_timer libboost_type_erasure libboost_thread libboost_chrono libboost_container libboost_date_time libboost_unit_test_framework libboost_prg_exec_monitor libboost_test_exec_monitor libboost_exception libboost_wave libboost_filesystem libboost_atomic libboost_wserialization libboost_serialization)
set(CONAN_SYSTEM_LIBS_BOOST_DEBUG ole32 dbgeng bcrypt ntdll shell32 Advapi32 user32)
set(CONAN_FRAMEWORKS_BOOST_DEBUG )
set(CONAN_FRAMEWORKS_FOUND_BOOST_DEBUG "")  # Will be filled later
set(CONAN_DEFINES_BOOST_DEBUG "-DBOOST_STACKTRACE_USE_NOOP"
			"-DBOOST_STACKTRACE_USE_WINDBG"
			"-DBOOST_STACKTRACE_USE_WINDBG_CACHED"
			"-DBOOST_ALL_NO_LIB")
set(CONAN_BUILD_MODULES_PATHS_BOOST_DEBUG )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_BOOST_DEBUG "BOOST_STACKTRACE_USE_NOOP"
			"BOOST_STACKTRACE_USE_WINDBG"
			"BOOST_STACKTRACE_USE_WINDBG_CACHED"
			"BOOST_ALL_NO_LIB")

set(CONAN_C_FLAGS_BOOST_DEBUG "")
set(CONAN_CXX_FLAGS_BOOST_DEBUG "")
set(CONAN_SHARED_LINKER_FLAGS_BOOST_DEBUG "")
set(CONAN_EXE_LINKER_FLAGS_BOOST_DEBUG "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_BOOST_DEBUG_LIST "")
set(CONAN_CXX_FLAGS_BOOST_DEBUG_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_BOOST_DEBUG_LIST "")
set(CONAN_EXE_LINKER_FLAGS_BOOST_DEBUG_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_BOOST_DEBUG "${CONAN_FRAMEWORKS_BOOST_DEBUG}" "_BOOST" "_DEBUG")
# Append to aggregated values variable
set(CONAN_LIBS_BOOST_DEBUG ${CONAN_PKG_LIBS_BOOST_DEBUG} ${CONAN_SYSTEM_LIBS_BOOST_DEBUG} ${CONAN_FRAMEWORKS_FOUND_BOOST_DEBUG})


#################
###  CATCH2
#################
set(CONAN_CATCH2_ROOT_DEBUG "C:/Users/trafe_ccwwt/.conan/data/catch2/3.1.0/_/_/package/8af5e418a748613c82c33773ac1c90f147094571")
set(CONAN_INCLUDE_DIRS_CATCH2_DEBUG "C:/Users/trafe_ccwwt/.conan/data/catch2/3.1.0/_/_/package/8af5e418a748613c82c33773ac1c90f147094571/include")
set(CONAN_LIB_DIRS_CATCH2_DEBUG "C:/Users/trafe_ccwwt/.conan/data/catch2/3.1.0/_/_/package/8af5e418a748613c82c33773ac1c90f147094571/lib")
set(CONAN_BIN_DIRS_CATCH2_DEBUG )
set(CONAN_RES_DIRS_CATCH2_DEBUG )
set(CONAN_SRC_DIRS_CATCH2_DEBUG )
set(CONAN_BUILD_DIRS_CATCH2_DEBUG "C:/Users/trafe_ccwwt/.conan/data/catch2/3.1.0/_/_/package/8af5e418a748613c82c33773ac1c90f147094571/lib/cmake/Catch2")
set(CONAN_FRAMEWORK_DIRS_CATCH2_DEBUG )
set(CONAN_LIBS_CATCH2_DEBUG Catch2Maind Catch2d)
set(CONAN_PKG_LIBS_CATCH2_DEBUG Catch2Maind Catch2d)
set(CONAN_SYSTEM_LIBS_CATCH2_DEBUG )
set(CONAN_FRAMEWORKS_CATCH2_DEBUG )
set(CONAN_FRAMEWORKS_FOUND_CATCH2_DEBUG "")  # Will be filled later
set(CONAN_DEFINES_CATCH2_DEBUG )
set(CONAN_BUILD_MODULES_PATHS_CATCH2_DEBUG )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_CATCH2_DEBUG )

set(CONAN_C_FLAGS_CATCH2_DEBUG "")
set(CONAN_CXX_FLAGS_CATCH2_DEBUG "")
set(CONAN_SHARED_LINKER_FLAGS_CATCH2_DEBUG "")
set(CONAN_EXE_LINKER_FLAGS_CATCH2_DEBUG "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_CATCH2_DEBUG_LIST "")
set(CONAN_CXX_FLAGS_CATCH2_DEBUG_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_CATCH2_DEBUG_LIST "")
set(CONAN_EXE_LINKER_FLAGS_CATCH2_DEBUG_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_CATCH2_DEBUG "${CONAN_FRAMEWORKS_CATCH2_DEBUG}" "_CATCH2" "_DEBUG")
# Append to aggregated values variable
set(CONAN_LIBS_CATCH2_DEBUG ${CONAN_PKG_LIBS_CATCH2_DEBUG} ${CONAN_SYSTEM_LIBS_CATCH2_DEBUG} ${CONAN_FRAMEWORKS_FOUND_CATCH2_DEBUG})


#################
###  LIBPQ
#################
set(CONAN_LIBPQ_ROOT_DEBUG "C:/Users/trafe_ccwwt/.conan/data/libpq/14.5/_/_/package/f9454b6359001e12129d09e21becd65fa0fb452c")
set(CONAN_INCLUDE_DIRS_LIBPQ_DEBUG "C:/Users/trafe_ccwwt/.conan/data/libpq/14.5/_/_/package/f9454b6359001e12129d09e21becd65fa0fb452c/include")
set(CONAN_LIB_DIRS_LIBPQ_DEBUG "C:/Users/trafe_ccwwt/.conan/data/libpq/14.5/_/_/package/f9454b6359001e12129d09e21becd65fa0fb452c/lib")
set(CONAN_BIN_DIRS_LIBPQ_DEBUG )
set(CONAN_RES_DIRS_LIBPQ_DEBUG )
set(CONAN_SRC_DIRS_LIBPQ_DEBUG )
set(CONAN_BUILD_DIRS_LIBPQ_DEBUG )
set(CONAN_FRAMEWORK_DIRS_LIBPQ_DEBUG )
set(CONAN_LIBS_LIBPQ_DEBUG libpq libpgport libpgcommon)
set(CONAN_PKG_LIBS_LIBPQ_DEBUG libpq libpgport libpgcommon)
set(CONAN_SYSTEM_LIBS_LIBPQ_DEBUG ws2_32 secur32 advapi32 shell32 crypt32 wldap32)
set(CONAN_FRAMEWORKS_LIBPQ_DEBUG )
set(CONAN_FRAMEWORKS_FOUND_LIBPQ_DEBUG "")  # Will be filled later
set(CONAN_DEFINES_LIBPQ_DEBUG )
set(CONAN_BUILD_MODULES_PATHS_LIBPQ_DEBUG )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_LIBPQ_DEBUG )

set(CONAN_C_FLAGS_LIBPQ_DEBUG "")
set(CONAN_CXX_FLAGS_LIBPQ_DEBUG "")
set(CONAN_SHARED_LINKER_FLAGS_LIBPQ_DEBUG "")
set(CONAN_EXE_LINKER_FLAGS_LIBPQ_DEBUG "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_LIBPQ_DEBUG_LIST "")
set(CONAN_CXX_FLAGS_LIBPQ_DEBUG_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_LIBPQ_DEBUG_LIST "")
set(CONAN_EXE_LINKER_FLAGS_LIBPQ_DEBUG_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_LIBPQ_DEBUG "${CONAN_FRAMEWORKS_LIBPQ_DEBUG}" "_LIBPQ" "_DEBUG")
# Append to aggregated values variable
set(CONAN_LIBS_LIBPQ_DEBUG ${CONAN_PKG_LIBS_LIBPQ_DEBUG} ${CONAN_SYSTEM_LIBS_LIBPQ_DEBUG} ${CONAN_FRAMEWORKS_FOUND_LIBPQ_DEBUG})


#################
###  ZLIB
#################
set(CONAN_ZLIB_ROOT_DEBUG "C:/Users/trafe_ccwwt/.conan/data/zlib/1.3.1/_/_/package/f9454b6359001e12129d09e21becd65fa0fb452c")
set(CONAN_INCLUDE_DIRS_ZLIB_DEBUG "C:/Users/trafe_ccwwt/.conan/data/zlib/1.3.1/_/_/package/f9454b6359001e12129d09e21becd65fa0fb452c/include")
set(CONAN_LIB_DIRS_ZLIB_DEBUG "C:/Users/trafe_ccwwt/.conan/data/zlib/1.3.1/_/_/package/f9454b6359001e12129d09e21becd65fa0fb452c/lib")
set(CONAN_BIN_DIRS_ZLIB_DEBUG )
set(CONAN_RES_DIRS_ZLIB_DEBUG )
set(CONAN_SRC_DIRS_ZLIB_DEBUG )
set(CONAN_BUILD_DIRS_ZLIB_DEBUG "C:/Users/trafe_ccwwt/.conan/data/zlib/1.3.1/_/_/package/f9454b6359001e12129d09e21becd65fa0fb452c/")
set(CONAN_FRAMEWORK_DIRS_ZLIB_DEBUG )
set(CONAN_LIBS_ZLIB_DEBUG zlib)
set(CONAN_PKG_LIBS_ZLIB_DEBUG zlib)
set(CONAN_SYSTEM_LIBS_ZLIB_DEBUG )
set(CONAN_FRAMEWORKS_ZLIB_DEBUG )
set(CONAN_FRAMEWORKS_FOUND_ZLIB_DEBUG "")  # Will be filled later
set(CONAN_DEFINES_ZLIB_DEBUG )
set(CONAN_BUILD_MODULES_PATHS_ZLIB_DEBUG )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_ZLIB_DEBUG )

set(CONAN_C_FLAGS_ZLIB_DEBUG "")
set(CONAN_CXX_FLAGS_ZLIB_DEBUG "")
set(CONAN_SHARED_LINKER_FLAGS_ZLIB_DEBUG "")
set(CONAN_EXE_LINKER_FLAGS_ZLIB_DEBUG "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_ZLIB_DEBUG_LIST "")
set(CONAN_CXX_FLAGS_ZLIB_DEBUG_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_ZLIB_DEBUG_LIST "")
set(CONAN_EXE_LINKER_FLAGS_ZLIB_DEBUG_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_ZLIB_DEBUG "${CONAN_FRAMEWORKS_ZLIB_DEBUG}" "_ZLIB" "_DEBUG")
# Append to aggregated values variable
set(CONAN_LIBS_ZLIB_DEBUG ${CONAN_PKG_LIBS_ZLIB_DEBUG} ${CONAN_SYSTEM_LIBS_ZLIB_DEBUG} ${CONAN_FRAMEWORKS_FOUND_ZLIB_DEBUG})


#################
###  BZIP2
#################
set(CONAN_BZIP2_ROOT_DEBUG "C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/6d1d248972e84f7b73288d825f88efec45347894")
set(CONAN_INCLUDE_DIRS_BZIP2_DEBUG "C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/6d1d248972e84f7b73288d825f88efec45347894/include")
set(CONAN_LIB_DIRS_BZIP2_DEBUG "C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/6d1d248972e84f7b73288d825f88efec45347894/lib")
set(CONAN_BIN_DIRS_BZIP2_DEBUG "C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/6d1d248972e84f7b73288d825f88efec45347894/bin")
set(CONAN_RES_DIRS_BZIP2_DEBUG )
set(CONAN_SRC_DIRS_BZIP2_DEBUG )
set(CONAN_BUILD_DIRS_BZIP2_DEBUG "C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/6d1d248972e84f7b73288d825f88efec45347894/")
set(CONAN_FRAMEWORK_DIRS_BZIP2_DEBUG )
set(CONAN_LIBS_BZIP2_DEBUG bz2)
set(CONAN_PKG_LIBS_BZIP2_DEBUG bz2)
set(CONAN_SYSTEM_LIBS_BZIP2_DEBUG )
set(CONAN_FRAMEWORKS_BZIP2_DEBUG )
set(CONAN_FRAMEWORKS_FOUND_BZIP2_DEBUG "")  # Will be filled later
set(CONAN_DEFINES_BZIP2_DEBUG )
set(CONAN_BUILD_MODULES_PATHS_BZIP2_DEBUG )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_BZIP2_DEBUG )

set(CONAN_C_FLAGS_BZIP2_DEBUG "")
set(CONAN_CXX_FLAGS_BZIP2_DEBUG "")
set(CONAN_SHARED_LINKER_FLAGS_BZIP2_DEBUG "")
set(CONAN_EXE_LINKER_FLAGS_BZIP2_DEBUG "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_BZIP2_DEBUG_LIST "")
set(CONAN_CXX_FLAGS_BZIP2_DEBUG_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_BZIP2_DEBUG_LIST "")
set(CONAN_EXE_LINKER_FLAGS_BZIP2_DEBUG_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_BZIP2_DEBUG "${CONAN_FRAMEWORKS_BZIP2_DEBUG}" "_BZIP2" "_DEBUG")
# Append to aggregated values variable
set(CONAN_LIBS_BZIP2_DEBUG ${CONAN_PKG_LIBS_BZIP2_DEBUG} ${CONAN_SYSTEM_LIBS_BZIP2_DEBUG} ${CONAN_FRAMEWORKS_FOUND_BZIP2_DEBUG})


### Definition of global aggregated variables ###

set(CONAN_DEPENDENCIES_DEBUG libpqxx boost catch2 libpq zlib bzip2)

set(CONAN_INCLUDE_DIRS_DEBUG "C:/Users/trafe_ccwwt/.conan/data/libpqxx/7.7.4/_/_/package/e0064ca83ef08ab777f660e93ce9416dd8009f96/include"
			"C:/.conan/695e9b/1/include"
			"C:/Users/trafe_ccwwt/.conan/data/catch2/3.1.0/_/_/package/8af5e418a748613c82c33773ac1c90f147094571/include"
			"C:/Users/trafe_ccwwt/.conan/data/libpq/14.5/_/_/package/f9454b6359001e12129d09e21becd65fa0fb452c/include"
			"C:/Users/trafe_ccwwt/.conan/data/zlib/1.3.1/_/_/package/f9454b6359001e12129d09e21becd65fa0fb452c/include"
			"C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/6d1d248972e84f7b73288d825f88efec45347894/include" ${CONAN_INCLUDE_DIRS_DEBUG})
set(CONAN_LIB_DIRS_DEBUG "C:/Users/trafe_ccwwt/.conan/data/libpqxx/7.7.4/_/_/package/e0064ca83ef08ab777f660e93ce9416dd8009f96/lib"
			"C:/.conan/695e9b/1/lib"
			"C:/Users/trafe_ccwwt/.conan/data/catch2/3.1.0/_/_/package/8af5e418a748613c82c33773ac1c90f147094571/lib"
			"C:/Users/trafe_ccwwt/.conan/data/libpq/14.5/_/_/package/f9454b6359001e12129d09e21becd65fa0fb452c/lib"
			"C:/Users/trafe_ccwwt/.conan/data/zlib/1.3.1/_/_/package/f9454b6359001e12129d09e21becd65fa0fb452c/lib"
			"C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/6d1d248972e84f7b73288d825f88efec45347894/lib" ${CONAN_LIB_DIRS_DEBUG})
set(CONAN_BIN_DIRS_DEBUG "C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/6d1d248972e84f7b73288d825f88efec45347894/bin" ${CONAN_BIN_DIRS_DEBUG})
set(CONAN_RES_DIRS_DEBUG  ${CONAN_RES_DIRS_DEBUG})
set(CONAN_FRAMEWORK_DIRS_DEBUG  ${CONAN_FRAMEWORK_DIRS_DEBUG})
set(CONAN_LIBS_DEBUG pqxx libboost_contract libboost_coroutine libboost_fiber_numa libboost_fiber libboost_context libboost_graph libboost_iostreams libboost_json libboost_locale libboost_log_setup libboost_log libboost_math_c99 libboost_math_c99f libboost_math_c99l libboost_math_tr1 libboost_math_tr1f libboost_math_tr1l libboost_nowide libboost_program_options libboost_random libboost_regex libboost_stacktrace_noop libboost_stacktrace_windbg libboost_stacktrace_windbg_cached libboost_timer libboost_type_erasure libboost_thread libboost_chrono libboost_container libboost_date_time libboost_unit_test_framework libboost_prg_exec_monitor libboost_test_exec_monitor libboost_exception libboost_wave libboost_filesystem libboost_atomic libboost_wserialization libboost_serialization Catch2Maind Catch2d libpq libpgport libpgcommon zlib bz2 ${CONAN_LIBS_DEBUG})
set(CONAN_PKG_LIBS_DEBUG pqxx libboost_contract libboost_coroutine libboost_fiber_numa libboost_fiber libboost_context libboost_graph libboost_iostreams libboost_json libboost_locale libboost_log_setup libboost_log libboost_math_c99 libboost_math_c99f libboost_math_c99l libboost_math_tr1 libboost_math_tr1f libboost_math_tr1l libboost_nowide libboost_program_options libboost_random libboost_regex libboost_stacktrace_noop libboost_stacktrace_windbg libboost_stacktrace_windbg_cached libboost_timer libboost_type_erasure libboost_thread libboost_chrono libboost_container libboost_date_time libboost_unit_test_framework libboost_prg_exec_monitor libboost_test_exec_monitor libboost_exception libboost_wave libboost_filesystem libboost_atomic libboost_wserialization libboost_serialization Catch2Maind Catch2d libpq libpgport libpgcommon zlib bz2 ${CONAN_PKG_LIBS_DEBUG})
set(CONAN_SYSTEM_LIBS_DEBUG wsock32 ole32 dbgeng bcrypt ntdll Advapi32 user32 ws2_32 secur32 advapi32 shell32 crypt32 wldap32 ${CONAN_SYSTEM_LIBS_DEBUG})
set(CONAN_FRAMEWORKS_DEBUG  ${CONAN_FRAMEWORKS_DEBUG})
set(CONAN_FRAMEWORKS_FOUND_DEBUG "")  # Will be filled later
set(CONAN_DEFINES_DEBUG "-DBOOST_STACKTRACE_USE_NOOP"
			"-DBOOST_STACKTRACE_USE_WINDBG"
			"-DBOOST_STACKTRACE_USE_WINDBG_CACHED"
			"-DBOOST_ALL_NO_LIB" ${CONAN_DEFINES_DEBUG})
set(CONAN_BUILD_MODULES_PATHS_DEBUG  ${CONAN_BUILD_MODULES_PATHS_DEBUG})
set(CONAN_CMAKE_MODULE_PATH_DEBUG "C:/Users/trafe_ccwwt/.conan/data/catch2/3.1.0/_/_/package/8af5e418a748613c82c33773ac1c90f147094571/lib/cmake/Catch2"
			"C:/Users/trafe_ccwwt/.conan/data/zlib/1.3.1/_/_/package/f9454b6359001e12129d09e21becd65fa0fb452c/"
			"C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/6d1d248972e84f7b73288d825f88efec45347894/" ${CONAN_CMAKE_MODULE_PATH_DEBUG})

set(CONAN_CXX_FLAGS_DEBUG " ${CONAN_CXX_FLAGS_DEBUG}")
set(CONAN_SHARED_LINKER_FLAGS_DEBUG " ${CONAN_SHARED_LINKER_FLAGS_DEBUG}")
set(CONAN_EXE_LINKER_FLAGS_DEBUG " ${CONAN_EXE_LINKER_FLAGS_DEBUG}")
set(CONAN_C_FLAGS_DEBUG " ${CONAN_C_FLAGS_DEBUG}")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_DEBUG "${CONAN_FRAMEWORKS_DEBUG}" "" "_DEBUG")
# Append to aggregated values variable: Use CONAN_LIBS instead of CONAN_PKG_LIBS to include user appended vars
set(CONAN_LIBS_DEBUG ${CONAN_LIBS_DEBUG} ${CONAN_SYSTEM_LIBS_DEBUG} ${CONAN_FRAMEWORKS_FOUND_DEBUG})
