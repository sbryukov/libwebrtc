
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)


include_directories(../ 
					)

set (SYSWRAP_SRC 
	source/sleep.cc
	source/sort.cc
	source/event_win.cc
	source/atomic32_posix.cc
	source/event_tracer_unittest.cc
	source/critical_section_unittest.cc
	source/data_log_c.cc
	source/event_posix.cc
	source/trace_posix.cc
	source/thread_win.cc
	source/data_log_helpers_unittest.cc
	source/stringize_macros_unittest.cc
	source/trace_win.cc
	source/tick_util.cc
	source/condition_variable_unittest.cc
	source/thread_posix_unittest.cc
	source/condition_variable.cc
	source/logging.cc
	source/thread_unittest.cc
	source/data_log_no_op.cc
	source/unittest_utilities_unittest.cc
	source/logging_unittest.cc
	source/condition_variable_event_win.cc
	source/event.cc
	source/critical_section.cc
	source/condition_variable_native_win.cc
	source/data_log_unittest_disabled.cc
	source/atomic32_mac.cc
	source/event_tracer.cc
	source/rw_lock_posix.cc
	source/data_log_unittest.cc
	source/thread.cc
	source/file_impl.cc
	source/atomic32_win.cc
	source/data_log.cc
	source/logcat_trace_context.cc
	source/rw_lock_generic.cc
	source/rw_lock.cc
	source/cpu_info.cc
	source/aligned_malloc_unittest.cc
	source/clock.cc
	source/thread_posix.cc
	source/trace_impl.cc
	source/cpu_features.cc
	source/condition_variable_posix.cc
	source/critical_section_win.cc
	source/clock_unittest.cc
	source/rw_lock_win.cc
	source/critical_section_posix.cc
	source/aligned_malloc.cc
	)



add_library (system_wrappers ${SYSWRAP_SRC})
target_link_libraries (system_wrappers)