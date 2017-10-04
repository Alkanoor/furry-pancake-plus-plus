#ifndef COMMON_LOGGERS_H
#define COMMON_LOGGERS_H


#include "../aggregators/terminal_color_aggregator.h"
#include "../handlers/header_aggregator_handler.h"
#include "../open_failed_behaviour_factory.h"
#include "../aggregators/string_aggregator.h"
#include "../handlers/aggregator_handler.h"
#include "../handlers/common_io_handlers.h"
#include "../aggregators/date_aggregator.h"
#include "../aggregators/aggregator.h"
#include "../handlers/safe_handler.h"
#include "../handlers/file_handler.h"
#include "../ostream_writable.h"
#include "thread_safe_logger.h"
#include "debug_logger.h"
#include "level_logger.h"
#include "logger.h"

#define INFO    0
#define WARNING 1
#define ERROR   2
#define SEVERE  3


static constexpr const char base_dir[] = "logs/";

static constexpr const char fatal[] = "[Fatal] ";
static constexpr const char severe[] = "[Severe] ";
static constexpr const char error[] = "[Error] ";
static constexpr const char warning[] = "[Warning] ";
static constexpr const char info[] = "[Info] ";
static constexpr const char debug[] = "[Debug] ";
static constexpr const char debug_no_space[] = "[Debug]";
static constexpr const char space[] = " ";
static constexpr const char fatal_filename[] = "fatal_errors.log";
static constexpr const char main_errors_filename[] = "main_errors.log";
static constexpr const char severe_filename[] = "_severe.log";
static constexpr const char error_filename[] = "_errors.log";
static constexpr const char warning_filename[] = "_warning.log";
static constexpr const char info_filename[] = "_info.log";
static constexpr const char debug_filename[] = "current_debug.log";


typedef Logger<Date_aggregator<>, Stdout_handler>                                   basic_timed_logger;
typedef Logger<Stdout_handler>                                                      basic_logger;
typedef Logger<Stderr_handler>                                                      basic_error_logger;
typedef Thread_safe_logger<Date_aggregator<>, Safe_handler<Stdout_handler> >        basic_thread_safe_timed_logger;
typedef Thread_safe_logger<Safe_handler<Stdout_handler> >                           basic_thread_safe_logger;
typedef Thread_safe_logger<Safe_handler<Stderr_handler> >                           basic_thread_safe_error_logger;

typedef Logger<Date_aggregator<String_header<fatal> >,
               Stderr_handler,
               Safe_handler<File_handler_fail_behaviour<fatal_filename, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::throw_keyword> > > fatal_error_logger;

typedef Dynamic_file_handler_fail_behaviour<String_header<base_dir, Date_aggregator<String_header<severe_filename> > >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::log_and_throw_keyword, true> dated_severe_handler;
typedef Dynamic_file_handler_fail_behaviour<String_header<base_dir, Date_aggregator<String_header<error_filename> > >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::log_and_throw_keyword, true> dated_error_handler;
typedef Dynamic_file_handler_fail_behaviour<String_header<base_dir, Date_aggregator<String_header<warning_filename> > >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::log_keyword, true> dated_warning_handler;
typedef Dynamic_file_handler_fail_behaviour<String_header<base_dir, Date_aggregator<String_header<info_filename> > >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::log_keyword, true> dated_info_handler;
typedef Dynamic_file_handler_fail_behaviour<String_header<base_dir, String_header<main_errors_filename> >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::throw_keyword> main_errors_handler;
typedef Dynamic_file_handler_fail_behaviour<String_header<base_dir, String_header<debug_filename> >, Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::log_keyword, true> debug_handler;

typedef Safe_handler<dated_severe_handler> safe_dated_severe_handler;
typedef Safe_handler<dated_error_handler> safe_dated_error_handler;
typedef Safe_handler<dated_warning_handler> safe_dated_warning_handler;
typedef Safe_handler<dated_info_handler> safe_dated_info_handler;
typedef Safe_handler<main_errors_handler> safe_main_errors_handler;
typedef Safe_handler<debug_handler> safe_debug_handler;

typedef Logger<Date_aggregator<String_header<error> >,
               Stderr_handler,
               dated_error_handler> error_logger;
typedef Logger<Date_aggregator<String_header<error> >,
               Safe_handler<Stderr_handler>,
               safe_dated_error_handler> safe_error_logger;

typedef Logger<Date_aggregator<String_header<warning> >,
               Stderr_handler,
               dated_warning_handler> warning_logger;
typedef Logger<Date_aggregator<String_header<warning> >,
               Safe_handler<Stderr_handler>,
               safe_dated_warning_handler> thread_safe_warning_logger;

typedef Logger<Date_aggregator<String_header<info> >,
               Stderr_handler,
               dated_info_handler > info_logger;
typedef Logger<Date_aggregator<String_header<info> >,
               Safe_handler<Stderr_handler>,
               safe_dated_info_handler> thread_safe_info_logger;

typedef Logger<Date_aggregator<String_header<error> >,
               Stderr_handler,
               main_errors_handler> errors_to_keep_logger;
typedef Logger<Date_aggregator<String_header<error> >,
               Safe_handler<Stderr_handler>,
               safe_main_errors_handler> thread_safe_errors_to_keep_logger;

typedef Debug_logger<String_header<debug_no_space, Date_aggregator<String_header<space> > >,
                     Stdout_handler,
                     debug_handler> basic_debug_logger;
typedef Debug_logger<String_header<debug_no_space, Date_aggregator<String_header<space> > >,
                     Safe_handler<Stdout_handler>,
                     safe_debug_handler> thread_safe_debug_logger;

typedef Aggregator_handler<dated_severe_handler, dated_error_handler, dated_warning_handler, dated_info_handler> info_warning_error_severe_handler;
typedef Aggregator_handler<dated_error_handler, dated_warning_handler, dated_info_handler> info_warning_error_handler;
typedef Aggregator_handler<dated_warning_handler, dated_info_handler> info_warning_handler;
typedef Aggregator_handler<dated_info_handler> info_handler;

typedef Black_aggregator<Stdout_handler> black_stdout;
typedef Red_aggregator<Stdout_handler> red_stdout;
typedef Green_aggregator<Stdout_handler> green_stdout;
typedef Orange_aggregator<Stdout_handler> orange_stdout;
typedef Yellow_aggregator<Stdout_handler> yellow_stdout;
typedef Blue_aggregator<Stdout_handler> blue_stdout;
typedef Magenta_aggregator<Stdout_handler> magenta_stdout;
typedef Cyan_aggregator<Stdout_handler> cyan_stdout;
typedef White_aggregator<Stdout_handler> white_stdout;

typedef Logger<black_stdout> black_logger;
typedef Logger<red_stdout> red_logger;
typedef Logger<green_stdout> green_logger;
typedef Logger<orange_stdout> orange_logger;
typedef Logger<yellow_stdout> yellow_logger;
typedef Logger<blue_stdout> blue_logger;
typedef Logger<magenta_stdout> magenta_logger;
typedef Logger<cyan_stdout> cyan_logger;

typedef Logger<Header_aggregator_handler<Red_aggregator<Date_aggregator<String_header<severe> > >, Stdout_handler> > dated_severe_red_logger;
typedef Logger<Header_aggregator_handler<Orange_aggregator<Date_aggregator<String_header<error> > >, Stdout_handler> > dated_errors_orange_logger;
typedef Logger<Header_aggregator_handler<Yellow_aggregator<Date_aggregator<String_header<warning> > >, Stdout_handler> > dated_warning_yellow_logger;
typedef Logger<Header_aggregator_handler<Date_aggregator<String_header<info> >, Stdout_handler> > dated_info_white_logger;

typedef Logger<Header_aggregator_handler<Red_aggregator<String_header<severe> >, Stdout_handler> > severe_red_logger;
typedef Logger<Header_aggregator_handler<Orange_aggregator<String_header<error> >, Stdout_handler> > errors_orange_logger;
typedef Logger<Header_aggregator_handler<Yellow_aggregator<String_header<warning> >, Stdout_handler> > warning_yellow_logger;
typedef Logger<Header_handler<info, Stdout_handler> > info_white_logger;

typedef Logger<Header_aggregator_handler<Red_aggregator<Date_aggregator<String_header<severe> > >, Safe_handler<Stdout_handler> > > safe_dated_severe_red_logger;
typedef Logger<Header_aggregator_handler<Orange_aggregator<Date_aggregator<String_header<error> > >, Safe_handler<Stdout_handler> > > safe_dated_errors_orange_logger;
typedef Logger<Header_aggregator_handler<Yellow_aggregator<Date_aggregator<String_header<warning> > >, Safe_handler<Stdout_handler> > > safe_dated_warning_yellow_logger;
typedef Logger<Header_aggregator_handler<Date_aggregator<String_header<info> >, Safe_handler<Stdout_handler> > > safe_dated_info_white_logger;

typedef Logger<Header_aggregator_handler<Red_aggregator<String_header<severe> >, Safe_handler<Stdout_handler> > > safe_severe_red_logger;
typedef Logger<Header_aggregator_handler<Orange_aggregator<String_header<error> >, Safe_handler<Stdout_handler> > > safe_errors_orange_logger;
typedef Logger<Header_aggregator_handler<Yellow_aggregator<String_header<warning> >, Safe_handler<Stdout_handler> > > safe_warning_yellow_logger;
typedef Logger<Header_handler<info, Safe_handler<Stdout_handler> > > safe_info_white_logger;


typedef Level_logger<Level_handler<0, Header_handler<info, dated_info_handler>, true>, Level_handler<0, info_white_logger, true>,
                     Level_handler<1, Header_handler<warning, dated_warning_handler>, true>, Level_handler<1, warning_yellow_logger, true>,
                     Level_handler<2, Header_handler<error, dated_error_handler>, true>, Level_handler<2, errors_orange_logger, true>,
                     Level_handler<3, Header_handler<severe, dated_severe_handler>, true>, Level_handler<3, severe_red_logger, true> > level_logger;

typedef Level_logger<Level_handler<0, Header_aggregator_handler<Date_aggregator<String_header<info> >, dated_info_handler>, true>, Level_handler<0, dated_info_white_logger, true>,
                     Level_handler<1, Header_aggregator_handler<Date_aggregator<String_header<warning> >, dated_warning_handler>, true>, Level_handler<1, dated_warning_yellow_logger, true>,
                     Level_handler<2, Header_aggregator_handler<Date_aggregator<String_header<error> >, dated_error_handler>, true>, Level_handler<2, dated_errors_orange_logger, true>,
                     Level_handler<3, Header_aggregator_handler<Date_aggregator<String_header<severe> >, dated_severe_handler>, true>, Level_handler<3, dated_severe_red_logger, true> > dated_level_logger;


typedef Level_logger<Level_handler<0, Header_handler<info, info_handler>, true>, Level_handler<0, info_white_logger, true>,
                     Level_handler<1, Header_handler<warning, info_warning_handler>, true>, Level_handler<1, warning_yellow_logger, true>,
                     Level_handler<2, Header_handler<error, info_warning_error_handler>, true>, Level_handler<2, errors_orange_logger, true>,
                     Level_handler<3, Header_handler<severe, info_warning_error_severe_handler>, true>, Level_handler<3, severe_red_logger, true> > recursive_level_logger;

typedef Level_logger<Level_handler<0, Header_aggregator_handler<Date_aggregator<String_header<info> >, info_handler>, true>, Level_handler<0, dated_info_white_logger, true>,
                     Level_handler<1, Header_aggregator_handler<Date_aggregator<String_header<warning> >, info_warning_handler>, true>, Level_handler<1, dated_warning_yellow_logger, true>,
                     Level_handler<2, Header_aggregator_handler<Date_aggregator<String_header<error> >, info_warning_error_handler>, true>, Level_handler<2, dated_errors_orange_logger, true>,
                     Level_handler<3, Header_aggregator_handler<Date_aggregator<String_header<severe> >, info_warning_error_severe_handler>, true>, Level_handler<3, dated_severe_red_logger, true> > recursive_dated_level_logger;

typedef Debug_aggretator_logger<level_logger> debug_level_logger;
typedef Debug_aggretator_logger<dated_level_logger> debug_dated_level_logger;
typedef Debug_aggretator_logger<recursive_level_logger> debug_recursive_level_logger;
typedef Debug_aggretator_logger<recursive_dated_level_logger> debug_recursive_dated_level_logger;


typedef Level_logger<Level_handler<0, Header_handler<info, safe_dated_info_handler>, true>, Level_handler<0, safe_info_white_logger, true>,
                     Level_handler<1, Header_handler<warning, safe_dated_warning_handler>, true>, Level_handler<1, safe_warning_yellow_logger, true>,
                     Level_handler<2, Header_handler<error, safe_dated_error_handler>, true>, Level_handler<2, safe_errors_orange_logger, true>,
                     Level_handler<3, Header_handler<severe, safe_dated_severe_handler>, true>, Level_handler<3, safe_severe_red_logger, true> > safe_level_logger;

typedef Level_logger<Level_handler<0, Header_aggregator_handler<Date_aggregator<String_header<info> >, safe_dated_info_handler>, true>, Level_handler<0, safe_dated_info_white_logger, true>,
                     Level_handler<1, Header_aggregator_handler<Date_aggregator<String_header<warning> >, safe_dated_warning_handler>, true>, Level_handler<1, safe_dated_warning_yellow_logger, true>,
                     Level_handler<2, Header_aggregator_handler<Date_aggregator<String_header<error> >, safe_dated_error_handler>, true>, Level_handler<2, safe_dated_errors_orange_logger, true>,
                     Level_handler<3, Header_aggregator_handler<Date_aggregator<String_header<severe> >, safe_dated_severe_handler>, true>, Level_handler<3, safe_dated_severe_red_logger, true> > safe_dated_level_logger;

typedef Thread_safe_aggretator_logger<thread_safe_debug_logger> debug_logger;
typedef Debug_aggretator_logger<Thread_safe_aggretator_logger<safe_level_logger> > logger;
typedef Debug_aggretator_logger<Thread_safe_aggretator_logger<safe_dated_level_logger> > dated_logger;


#endif
