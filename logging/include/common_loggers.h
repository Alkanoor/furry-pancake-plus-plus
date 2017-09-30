#ifndef COMMON_LOGGERS_H
#define COMMON_LOGGERS_H


#include "aggregators/terminal_color_aggregator.h"
#include "open_failed_behaviour_factory.h"
#include "aggregators/string_aggregator.h"
#include "handlers/common_io_handlers.h"
#include "aggregators/date_aggregator.h"
#include "aggregators/aggregator.h"
#include "handlers/safe_handler.h"
#include "handlers/file_handler.h"
#include "logger.h"


static constexpr const char base_dir[] = "logs/";

static constexpr const char fatal[] = "[Fatal]";
static constexpr const char error[] = "[Error]";
static constexpr const char warning[] = "[Warning]";
static constexpr const char info[] = "[Info]";
static constexpr const char debug[] = "[Debug]";
static constexpr const char fatal_filename[] = "fatal_errors.log";
static constexpr const char main_errors_filename[] = "main_errors.log";
static constexpr const char error_filename[] = "_errors.log";
static constexpr const char warning_filename[] = "_warning.log";
static constexpr const char info_filename[] = "_info.log";
static constexpr const char debug_filename[] = "current_debug.log";


typedef Logger<Date_aggregator<>, Stdout_handler>                       basic_timed_logger;
typedef Logger<Stdout_handler>                                          basic_logger;
typedef Logger<Stderr_handler>                                          basic_error_logger;
typedef Logger<Date_aggregator<>, Safe_handler<Stdout_handler> >        basic_thread_safe_timed_logger;
typedef Logger<Safe_handler<Stdout_handler> >                           basic_thread_safe_logger;
typedef Logger<Safe_handler<Stderr_handler> >                           basic_thread_safe_error_logger;

typedef Logger<Date_aggregator<String_header<fatal> >,
               Safe_handler<File_handler_fail_behaviour<fatal_filename, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::throw_keyword> > > fatal_error_logger;

typedef Logger<Date_aggregator<String_header<error> >,
               Stderr_handler,
               Dynamic_file_handler_fail_behaviour<String_header<base_dir, Date_aggregator<String_header<error_filename> > >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::throw_keyword, true> > error_logger;
typedef Logger<Date_aggregator<String_header<error> >,
               Stderr_handler,
               Safe_handler<Dynamic_file_handler_fail_behaviour<String_header<base_dir, Date_aggregator<String_header<error_filename> > >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::throw_keyword, true> > > safe_error_logger;

typedef Logger<Date_aggregator<String_header<warning> >,
               Stderr_handler,
               Dynamic_file_handler_fail_behaviour<String_header<base_dir, Date_aggregator<String_header<warning_filename> > >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::log_keyword, true> > warning_logger;
typedef Logger<Date_aggregator<String_header<warning> >,
               Stderr_handler,
               Safe_handler<Dynamic_file_handler_fail_behaviour<String_header<base_dir, Date_aggregator<String_header<warning_filename> > >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::log_keyword, true> > > thread_safe_warning_logger;

typedef Logger<Date_aggregator<String_header<info> >,
              Stderr_handler,
              Dynamic_file_handler_fail_behaviour<String_header<base_dir, Date_aggregator<String_header<info_filename> > >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::log_keyword, true> > info_logger;
typedef Logger<Date_aggregator<String_header<info> >,
              Stderr_handler,
              Safe_handler<Dynamic_file_handler_fail_behaviour<String_header<base_dir, Date_aggregator<String_header<info_filename> > >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::log_keyword, true> > > thread_safe_info_logger;

typedef Logger<Date_aggregator<String_header<error> >,
               Stderr_handler,
               Dynamic_file_handler_fail_behaviour<String_header<base_dir, String_header<main_errors_filename> >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::throw_keyword> > error_to_keep_logger;
typedef Logger<Date_aggregator<String_header<error> >,
               Stderr_handler,
               Safe_handler<Dynamic_file_handler_fail_behaviour<String_header<base_dir, String_header<main_errors_filename> >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::log_keyword> > > thread_safe_error_to_keep_logger;

typedef Logger<String_header<debug, Date_aggregator<>>,
               Stdout_handler,
               Dynamic_file_handler_fail_behaviour<String_header<base_dir, String_header<debug_filename> >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::log_keyword, true> > debug_logger;
typedef Logger<String_header<debug, Date_aggregator<>>,
               Stdout_handler,
               Safe_handler<Dynamic_file_handler_fail_behaviour<String_header<base_dir, String_header<debug_filename> >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::log_keyword, true> > > thread_safe_debug_logger;

// TODO : implement colored leveled logger


#endif
