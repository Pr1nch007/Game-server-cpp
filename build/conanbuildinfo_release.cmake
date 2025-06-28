
#################
###  LIBPQXX
#################
set(CONAN_LIBPQXX_ROOT_RELEASE "C:/Users/trafe_ccwwt/.conan/data/libpqxx/7.7.4/_/_/package/ae18969a5607dfe18b8e981dbe7318ab669b9409")
set(CONAN_INCLUDE_DIRS_LIBPQXX_RELEASE "C:/Users/trafe_ccwwt/.conan/data/libpqxx/7.7.4/_/_/package/ae18969a5607dfe18b8e981dbe7318ab669b9409/include")
set(CONAN_LIB_DIRS_LIBPQXX_RELEASE "C:/Users/trafe_ccwwt/.conan/data/libpqxx/7.7.4/_/_/package/ae18969a5607dfe18b8e981dbe7318ab669b9409/lib")
set(CONAN_BIN_DIRS_LIBPQXX_RELEASE )
set(CONAN_RES_DIRS_LIBPQXX_RELEASE )
set(CONAN_SRC_DIRS_LIBPQXX_RELEASE )
set(CONAN_BUILD_DIRS_LIBPQXX_RELEASE )
set(CONAN_FRAMEWORK_DIRS_LIBPQXX_RELEASE )
set(CONAN_LIBS_LIBPQXX_RELEASE pqxx)
set(CONAN_PKG_LIBS_LIBPQXX_RELEASE pqxx)
set(CONAN_SYSTEM_LIBS_LIBPQXX_RELEASE wsock32 ws2_32)
set(CONAN_FRAMEWORKS_LIBPQXX_RELEASE )
set(CONAN_FRAMEWORKS_FOUND_LIBPQXX_RELEASE "")  # Will be filled later
set(CONAN_DEFINES_LIBPQXX_RELEASE )
set(CONAN_BUILD_MODULES_PATHS_LIBPQXX_RELEASE )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_LIBPQXX_RELEASE )

set(CONAN_C_FLAGS_LIBPQXX_RELEASE "")
set(CONAN_CXX_FLAGS_LIBPQXX_RELEASE "")
set(CONAN_SHARED_LINKER_FLAGS_LIBPQXX_RELEASE "")
set(CONAN_EXE_LINKER_FLAGS_LIBPQXX_RELEASE "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_LIBPQXX_RELEASE_LIST "")
set(CONAN_CXX_FLAGS_LIBPQXX_RELEASE_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_LIBPQXX_RELEASE_LIST "")
set(CONAN_EXE_LINKER_FLAGS_LIBPQXX_RELEASE_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_LIBPQXX_RELEASE "${CONAN_FRAMEWORKS_LIBPQXX_RELEASE}" "_LIBPQXX" "_RELEASE")
# Append to aggregated values variable
set(CONAN_LIBS_LIBPQXX_RELEASE ${CONAN_PKG_LIBS_LIBPQXX_RELEASE} ${CONAN_SYSTEM_LIBS_LIBPQXX_RELEASE} ${CONAN_FRAMEWORKS_FOUND_LIBPQXX_RELEASE})


#################
###  BOOST
#################
set(CONAN_BOOST_ROOT_RELEASE "C:/.conan/81d457/1")
set(CONAN_INCLUDE_DIRS_BOOST_RELEASE "C:/.conan/81d457/1/include")
set(CONAN_LIB_DIRS_BOOST_RELEASE "C:/.conan/81d457/1/lib")
set(CONAN_BIN_DIRS_BOOST_RELEASE )
set(CONAN_RES_DIRS_BOOST_RELEASE )
set(CONAN_SRC_DIRS_BOOST_RELEASE )
set(CONAN_BUILD_DIRS_BOOST_RELEASE )
set(CONAN_FRAMEWORK_DIRS_BOOST_RELEASE )
set(CONAN_LIBS_BOOST_RELEASE libboost_contract libboost_coroutine libboost_fiber_numa libboost_fiber libboost_context libboost_graph libboost_iostreams libboost_json libboost_locale libboost_log_setup libboost_log libboost_math_c99 libboost_math_c99f libboost_math_c99l libboost_math_tr1 libboost_math_tr1f libboost_math_tr1l libboost_nowide libboost_program_options libboost_random libboost_regex libboost_stacktrace_noop libboost_stacktrace_windbg libboost_stacktrace_windbg_cached libboost_timer libboost_type_erasure libboost_thread libboost_chrono libboost_container libboost_date_time libboost_unit_test_framework libboost_prg_exec_monitor libboost_test_exec_monitor libboost_exception libboost_wave libboost_filesystem libboost_atomic libboost_wserialization libboost_serialization)
set(CONAN_PKG_LIBS_BOOST_RELEASE libboost_contract libboost_coroutine libboost_fiber_numa libboost_fiber libboost_context libboost_graph libboost_iostreams libboost_json libboost_locale libboost_log_setup libboost_log libboost_math_c99 libboost_math_c99f libboost_math_c99l libboost_math_tr1 libboost_math_tr1f libboost_math_tr1l libboost_nowide libboost_program_options libboost_random libboost_regex libboost_stacktrace_noop libboost_stacktrace_windbg libboost_stacktrace_windbg_cached libboost_timer libboost_type_erasure libboost_thread libboost_chrono libboost_container libboost_date_time libboost_unit_test_framework libboost_prg_exec_monitor libboost_test_exec_monitor libboost_exception libboost_wave libboost_filesystem libboost_atomic libboost_wserialization libboost_serialization)
set(CONAN_SYSTEM_LIBS_BOOST_RELEASE ole32 dbgeng bcrypt ntdll shell32 Advapi32 user32)
set(CONAN_FRAMEWORKS_BOOST_RELEASE )
set(CONAN_FRAMEWORKS_FOUND_BOOST_RELEASE "")  # Will be filled later
set(CONAN_DEFINES_BOOST_RELEASE "-DBOOST_STACKTRACE_USE_NOOP"
			"-DBOOST_STACKTRACE_USE_WINDBG"
			"-DBOOST_STACKTRACE_USE_WINDBG_CACHED"
			"-DBOOST_ALL_NO_LIB")
set(CONAN_BUILD_MODULES_PATHS_BOOST_RELEASE )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_BOOST_RELEASE "BOOST_STACKTRACE_USE_NOOP"
			"BOOST_STACKTRACE_USE_WINDBG"
			"BOOST_STACKTRACE_USE_WINDBG_CACHED"
			"BOOST_ALL_NO_LIB")

set(CONAN_C_FLAGS_BOOST_RELEASE "")
set(CONAN_CXX_FLAGS_BOOST_RELEASE "")
set(CONAN_SHARED_LINKER_FLAGS_BOOST_RELEASE "")
set(CONAN_EXE_LINKER_FLAGS_BOOST_RELEASE "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_BOOST_RELEASE_LIST "")
set(CONAN_CXX_FLAGS_BOOST_RELEASE_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_BOOST_RELEASE_LIST "")
set(CONAN_EXE_LINKER_FLAGS_BOOST_RELEASE_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_BOOST_RELEASE "${CONAN_FRAMEWORKS_BOOST_RELEASE}" "_BOOST" "_RELEASE")
# Append to aggregated values variable
set(CONAN_LIBS_BOOST_RELEASE ${CONAN_PKG_LIBS_BOOST_RELEASE} ${CONAN_SYSTEM_LIBS_BOOST_RELEASE} ${CONAN_FRAMEWORKS_FOUND_BOOST_RELEASE})


#################
###  CATCH2
#################
set(CONAN_CATCH2_ROOT_RELEASE "C:/Users/trafe_ccwwt/.conan/data/catch2/3.1.0/_/_/package/8d0a8dbc116f331a5e1021bc6c34e6e08e9aebc0")
set(CONAN_INCLUDE_DIRS_CATCH2_RELEASE "C:/Users/trafe_ccwwt/.conan/data/catch2/3.1.0/_/_/package/8d0a8dbc116f331a5e1021bc6c34e6e08e9aebc0/include")
set(CONAN_LIB_DIRS_CATCH2_RELEASE "C:/Users/trafe_ccwwt/.conan/data/catch2/3.1.0/_/_/package/8d0a8dbc116f331a5e1021bc6c34e6e08e9aebc0/lib")
set(CONAN_BIN_DIRS_CATCH2_RELEASE )
set(CONAN_RES_DIRS_CATCH2_RELEASE )
set(CONAN_SRC_DIRS_CATCH2_RELEASE )
set(CONAN_BUILD_DIRS_CATCH2_RELEASE "C:/Users/trafe_ccwwt/.conan/data/catch2/3.1.0/_/_/package/8d0a8dbc116f331a5e1021bc6c34e6e08e9aebc0/lib/cmake/Catch2")
set(CONAN_FRAMEWORK_DIRS_CATCH2_RELEASE )
set(CONAN_LIBS_CATCH2_RELEASE Catch2Main Catch2)
set(CONAN_PKG_LIBS_CATCH2_RELEASE Catch2Main Catch2)
set(CONAN_SYSTEM_LIBS_CATCH2_RELEASE )
set(CONAN_FRAMEWORKS_CATCH2_RELEASE )
set(CONAN_FRAMEWORKS_FOUND_CATCH2_RELEASE "")  # Will be filled later
set(CONAN_DEFINES_CATCH2_RELEASE )
set(CONAN_BUILD_MODULES_PATHS_CATCH2_RELEASE )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_CATCH2_RELEASE )

set(CONAN_C_FLAGS_CATCH2_RELEASE "")
set(CONAN_CXX_FLAGS_CATCH2_RELEASE "")
set(CONAN_SHARED_LINKER_FLAGS_CATCH2_RELEASE "")
set(CONAN_EXE_LINKER_FLAGS_CATCH2_RELEASE "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_CATCH2_RELEASE_LIST "")
set(CONAN_CXX_FLAGS_CATCH2_RELEASE_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_CATCH2_RELEASE_LIST "")
set(CONAN_EXE_LINKER_FLAGS_CATCH2_RELEASE_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_CATCH2_RELEASE "${CONAN_FRAMEWORKS_CATCH2_RELEASE}" "_CATCH2" "_RELEASE")
# Append to aggregated values variable
set(CONAN_LIBS_CATCH2_RELEASE ${CONAN_PKG_LIBS_CATCH2_RELEASE} ${CONAN_SYSTEM_LIBS_CATCH2_RELEASE} ${CONAN_FRAMEWORKS_FOUND_CATCH2_RELEASE})


#################
###  LIBPQ
#################
set(CONAN_LIBPQ_ROOT_RELEASE "C:/Users/trafe_ccwwt/.conan/data/libpq/14.5/_/_/package/871c6303207aa1b539e682559d0b5180e15f0c30")
set(CONAN_INCLUDE_DIRS_LIBPQ_RELEASE "C:/Users/trafe_ccwwt/.conan/data/libpq/14.5/_/_/package/871c6303207aa1b539e682559d0b5180e15f0c30/include")
set(CONAN_LIB_DIRS_LIBPQ_RELEASE "C:/Users/trafe_ccwwt/.conan/data/libpq/14.5/_/_/package/871c6303207aa1b539e682559d0b5180e15f0c30/lib")
set(CONAN_BIN_DIRS_LIBPQ_RELEASE )
set(CONAN_RES_DIRS_LIBPQ_RELEASE )
set(CONAN_SRC_DIRS_LIBPQ_RELEASE )
set(CONAN_BUILD_DIRS_LIBPQ_RELEASE )
set(CONAN_FRAMEWORK_DIRS_LIBPQ_RELEASE )
set(CONAN_LIBS_LIBPQ_RELEASE libpq libpgport libpgcommon)
set(CONAN_PKG_LIBS_LIBPQ_RELEASE libpq libpgport libpgcommon)
set(CONAN_SYSTEM_LIBS_LIBPQ_RELEASE ws2_32 secur32 advapi32 shell32 crypt32 wldap32)
set(CONAN_FRAMEWORKS_LIBPQ_RELEASE )
set(CONAN_FRAMEWORKS_FOUND_LIBPQ_RELEASE "")  # Will be filled later
set(CONAN_DEFINES_LIBPQ_RELEASE )
set(CONAN_BUILD_MODULES_PATHS_LIBPQ_RELEASE )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_LIBPQ_RELEASE )

set(CONAN_C_FLAGS_LIBPQ_RELEASE "")
set(CONAN_CXX_FLAGS_LIBPQ_RELEASE "")
set(CONAN_SHARED_LINKER_FLAGS_LIBPQ_RELEASE "")
set(CONAN_EXE_LINKER_FLAGS_LIBPQ_RELEASE "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_LIBPQ_RELEASE_LIST "")
set(CONAN_CXX_FLAGS_LIBPQ_RELEASE_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_LIBPQ_RELEASE_LIST "")
set(CONAN_EXE_LINKER_FLAGS_LIBPQ_RELEASE_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_LIBPQ_RELEASE "${CONAN_FRAMEWORKS_LIBPQ_RELEASE}" "_LIBPQ" "_RELEASE")
# Append to aggregated values variable
set(CONAN_LIBS_LIBPQ_RELEASE ${CONAN_PKG_LIBS_LIBPQ_RELEASE} ${CONAN_SYSTEM_LIBS_LIBPQ_RELEASE} ${CONAN_FRAMEWORKS_FOUND_LIBPQ_RELEASE})


#################
###  ZLIB
#################
set(CONAN_ZLIB_ROOT_RELEASE "C:/Users/trafe_ccwwt/.conan/data/zlib/1.3.1/_/_/package/871c6303207aa1b539e682559d0b5180e15f0c30")
set(CONAN_INCLUDE_DIRS_ZLIB_RELEASE "C:/Users/trafe_ccwwt/.conan/data/zlib/1.3.1/_/_/package/871c6303207aa1b539e682559d0b5180e15f0c30/include")
set(CONAN_LIB_DIRS_ZLIB_RELEASE "C:/Users/trafe_ccwwt/.conan/data/zlib/1.3.1/_/_/package/871c6303207aa1b539e682559d0b5180e15f0c30/lib")
set(CONAN_BIN_DIRS_ZLIB_RELEASE )
set(CONAN_RES_DIRS_ZLIB_RELEASE )
set(CONAN_SRC_DIRS_ZLIB_RELEASE )
set(CONAN_BUILD_DIRS_ZLIB_RELEASE "C:/Users/trafe_ccwwt/.conan/data/zlib/1.3.1/_/_/package/871c6303207aa1b539e682559d0b5180e15f0c30/")
set(CONAN_FRAMEWORK_DIRS_ZLIB_RELEASE )
set(CONAN_LIBS_ZLIB_RELEASE zlib)
set(CONAN_PKG_LIBS_ZLIB_RELEASE zlib)
set(CONAN_SYSTEM_LIBS_ZLIB_RELEASE )
set(CONAN_FRAMEWORKS_ZLIB_RELEASE )
set(CONAN_FRAMEWORKS_FOUND_ZLIB_RELEASE "")  # Will be filled later
set(CONAN_DEFINES_ZLIB_RELEASE )
set(CONAN_BUILD_MODULES_PATHS_ZLIB_RELEASE )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_ZLIB_RELEASE )

set(CONAN_C_FLAGS_ZLIB_RELEASE "")
set(CONAN_CXX_FLAGS_ZLIB_RELEASE "")
set(CONAN_SHARED_LINKER_FLAGS_ZLIB_RELEASE "")
set(CONAN_EXE_LINKER_FLAGS_ZLIB_RELEASE "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_ZLIB_RELEASE_LIST "")
set(CONAN_CXX_FLAGS_ZLIB_RELEASE_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_ZLIB_RELEASE_LIST "")
set(CONAN_EXE_LINKER_FLAGS_ZLIB_RELEASE_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_ZLIB_RELEASE "${CONAN_FRAMEWORKS_ZLIB_RELEASE}" "_ZLIB" "_RELEASE")
# Append to aggregated values variable
set(CONAN_LIBS_ZLIB_RELEASE ${CONAN_PKG_LIBS_ZLIB_RELEASE} ${CONAN_SYSTEM_LIBS_ZLIB_RELEASE} ${CONAN_FRAMEWORKS_FOUND_ZLIB_RELEASE})


#################
###  BZIP2
#################
set(CONAN_BZIP2_ROOT_RELEASE "C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/9ee7772e425d35c1aa48c680ed5911828505a114")
set(CONAN_INCLUDE_DIRS_BZIP2_RELEASE "C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/9ee7772e425d35c1aa48c680ed5911828505a114/include")
set(CONAN_LIB_DIRS_BZIP2_RELEASE "C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/9ee7772e425d35c1aa48c680ed5911828505a114/lib")
set(CONAN_BIN_DIRS_BZIP2_RELEASE "C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/9ee7772e425d35c1aa48c680ed5911828505a114/bin")
set(CONAN_RES_DIRS_BZIP2_RELEASE )
set(CONAN_SRC_DIRS_BZIP2_RELEASE )
set(CONAN_BUILD_DIRS_BZIP2_RELEASE "C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/9ee7772e425d35c1aa48c680ed5911828505a114/")
set(CONAN_FRAMEWORK_DIRS_BZIP2_RELEASE )
set(CONAN_LIBS_BZIP2_RELEASE bz2)
set(CONAN_PKG_LIBS_BZIP2_RELEASE bz2)
set(CONAN_SYSTEM_LIBS_BZIP2_RELEASE )
set(CONAN_FRAMEWORKS_BZIP2_RELEASE )
set(CONAN_FRAMEWORKS_FOUND_BZIP2_RELEASE "")  # Will be filled later
set(CONAN_DEFINES_BZIP2_RELEASE )
set(CONAN_BUILD_MODULES_PATHS_BZIP2_RELEASE )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_BZIP2_RELEASE )

set(CONAN_C_FLAGS_BZIP2_RELEASE "")
set(CONAN_CXX_FLAGS_BZIP2_RELEASE "")
set(CONAN_SHARED_LINKER_FLAGS_BZIP2_RELEASE "")
set(CONAN_EXE_LINKER_FLAGS_BZIP2_RELEASE "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_BZIP2_RELEASE_LIST "")
set(CONAN_CXX_FLAGS_BZIP2_RELEASE_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_BZIP2_RELEASE_LIST "")
set(CONAN_EXE_LINKER_FLAGS_BZIP2_RELEASE_LIST "")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_BZIP2_RELEASE "${CONAN_FRAMEWORKS_BZIP2_RELEASE}" "_BZIP2" "_RELEASE")
# Append to aggregated values variable
set(CONAN_LIBS_BZIP2_RELEASE ${CONAN_PKG_LIBS_BZIP2_RELEASE} ${CONAN_SYSTEM_LIBS_BZIP2_RELEASE} ${CONAN_FRAMEWORKS_FOUND_BZIP2_RELEASE})


### Definition of global aggregated variables ###

set(CONAN_DEPENDENCIES_RELEASE libpqxx boost catch2 libpq zlib bzip2)

set(CONAN_INCLUDE_DIRS_RELEASE "C:/Users/trafe_ccwwt/.conan/data/libpqxx/7.7.4/_/_/package/ae18969a5607dfe18b8e981dbe7318ab669b9409/include"
			"C:/.conan/81d457/1/include"
			"C:/Users/trafe_ccwwt/.conan/data/catch2/3.1.0/_/_/package/8d0a8dbc116f331a5e1021bc6c34e6e08e9aebc0/include"
			"C:/Users/trafe_ccwwt/.conan/data/libpq/14.5/_/_/package/871c6303207aa1b539e682559d0b5180e15f0c30/include"
			"C:/Users/trafe_ccwwt/.conan/data/zlib/1.3.1/_/_/package/871c6303207aa1b539e682559d0b5180e15f0c30/include"
			"C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/9ee7772e425d35c1aa48c680ed5911828505a114/include" ${CONAN_INCLUDE_DIRS_RELEASE})
set(CONAN_LIB_DIRS_RELEASE "C:/Users/trafe_ccwwt/.conan/data/libpqxx/7.7.4/_/_/package/ae18969a5607dfe18b8e981dbe7318ab669b9409/lib"
			"C:/.conan/81d457/1/lib"
			"C:/Users/trafe_ccwwt/.conan/data/catch2/3.1.0/_/_/package/8d0a8dbc116f331a5e1021bc6c34e6e08e9aebc0/lib"
			"C:/Users/trafe_ccwwt/.conan/data/libpq/14.5/_/_/package/871c6303207aa1b539e682559d0b5180e15f0c30/lib"
			"C:/Users/trafe_ccwwt/.conan/data/zlib/1.3.1/_/_/package/871c6303207aa1b539e682559d0b5180e15f0c30/lib"
			"C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/9ee7772e425d35c1aa48c680ed5911828505a114/lib" ${CONAN_LIB_DIRS_RELEASE})
set(CONAN_BIN_DIRS_RELEASE "C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/9ee7772e425d35c1aa48c680ed5911828505a114/bin" ${CONAN_BIN_DIRS_RELEASE})
set(CONAN_RES_DIRS_RELEASE  ${CONAN_RES_DIRS_RELEASE})
set(CONAN_FRAMEWORK_DIRS_RELEASE  ${CONAN_FRAMEWORK_DIRS_RELEASE})
set(CONAN_LIBS_RELEASE pqxx libboost_contract libboost_coroutine libboost_fiber_numa libboost_fiber libboost_context libboost_graph libboost_iostreams libboost_json libboost_locale libboost_log_setup libboost_log libboost_math_c99 libboost_math_c99f libboost_math_c99l libboost_math_tr1 libboost_math_tr1f libboost_math_tr1l libboost_nowide libboost_program_options libboost_random libboost_regex libboost_stacktrace_noop libboost_stacktrace_windbg libboost_stacktrace_windbg_cached libboost_timer libboost_type_erasure libboost_thread libboost_chrono libboost_container libboost_date_time libboost_unit_test_framework libboost_prg_exec_monitor libboost_test_exec_monitor libboost_exception libboost_wave libboost_filesystem libboost_atomic libboost_wserialization libboost_serialization Catch2Main Catch2 libpq libpgport libpgcommon zlib bz2 ${CONAN_LIBS_RELEASE})
set(CONAN_PKG_LIBS_RELEASE pqxx libboost_contract libboost_coroutine libboost_fiber_numa libboost_fiber libboost_context libboost_graph libboost_iostreams libboost_json libboost_locale libboost_log_setup libboost_log libboost_math_c99 libboost_math_c99f libboost_math_c99l libboost_math_tr1 libboost_math_tr1f libboost_math_tr1l libboost_nowide libboost_program_options libboost_random libboost_regex libboost_stacktrace_noop libboost_stacktrace_windbg libboost_stacktrace_windbg_cached libboost_timer libboost_type_erasure libboost_thread libboost_chrono libboost_container libboost_date_time libboost_unit_test_framework libboost_prg_exec_monitor libboost_test_exec_monitor libboost_exception libboost_wave libboost_filesystem libboost_atomic libboost_wserialization libboost_serialization Catch2Main Catch2 libpq libpgport libpgcommon zlib bz2 ${CONAN_PKG_LIBS_RELEASE})
set(CONAN_SYSTEM_LIBS_RELEASE wsock32 ole32 dbgeng bcrypt ntdll Advapi32 user32 ws2_32 secur32 advapi32 shell32 crypt32 wldap32 ${CONAN_SYSTEM_LIBS_RELEASE})
set(CONAN_FRAMEWORKS_RELEASE  ${CONAN_FRAMEWORKS_RELEASE})
set(CONAN_FRAMEWORKS_FOUND_RELEASE "")  # Will be filled later
set(CONAN_DEFINES_RELEASE "-DBOOST_STACKTRACE_USE_NOOP"
			"-DBOOST_STACKTRACE_USE_WINDBG"
			"-DBOOST_STACKTRACE_USE_WINDBG_CACHED"
			"-DBOOST_ALL_NO_LIB" ${CONAN_DEFINES_RELEASE})
set(CONAN_BUILD_MODULES_PATHS_RELEASE  ${CONAN_BUILD_MODULES_PATHS_RELEASE})
set(CONAN_CMAKE_MODULE_PATH_RELEASE "C:/Users/trafe_ccwwt/.conan/data/catch2/3.1.0/_/_/package/8d0a8dbc116f331a5e1021bc6c34e6e08e9aebc0/lib/cmake/Catch2"
			"C:/Users/trafe_ccwwt/.conan/data/zlib/1.3.1/_/_/package/871c6303207aa1b539e682559d0b5180e15f0c30/"
			"C:/Users/trafe_ccwwt/.conan/data/bzip2/1.0.8/_/_/package/9ee7772e425d35c1aa48c680ed5911828505a114/" ${CONAN_CMAKE_MODULE_PATH_RELEASE})

set(CONAN_CXX_FLAGS_RELEASE " ${CONAN_CXX_FLAGS_RELEASE}")
set(CONAN_SHARED_LINKER_FLAGS_RELEASE " ${CONAN_SHARED_LINKER_FLAGS_RELEASE}")
set(CONAN_EXE_LINKER_FLAGS_RELEASE " ${CONAN_EXE_LINKER_FLAGS_RELEASE}")
set(CONAN_C_FLAGS_RELEASE " ${CONAN_C_FLAGS_RELEASE}")

# Apple Frameworks
conan_find_apple_frameworks(CONAN_FRAMEWORKS_FOUND_RELEASE "${CONAN_FRAMEWORKS_RELEASE}" "" "_RELEASE")
# Append to aggregated values variable: Use CONAN_LIBS instead of CONAN_PKG_LIBS to include user appended vars
set(CONAN_LIBS_RELEASE ${CONAN_LIBS_RELEASE} ${CONAN_SYSTEM_LIBS_RELEASE} ${CONAN_FRAMEWORKS_FOUND_RELEASE})
