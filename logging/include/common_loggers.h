#ifndef COMMON_LOGGERS_H
#define COMMON_LOGGERS_H


#include "open_failed_behaviour_factory.h"
#include "common_io_handlers.h"
#include "Safe_handler.h"
#include "file_handler.h"
#include "aggregator.h"
#include "logger.h"


static const std::string base_dir = "logs/";

typedef Logger<Date_aggregator, Stdout_handler> basic_timed_logger;
typedef Logger<String_aggregator<"">, Stdout_handler> basic_logger;
typedef Logger<String_aggregator<"">, Stderr_handler> basic_error_logger;
typedef Logger<Date_aggregator, Safe_handler<Stdout_handler> > basic_thread_safe_timed_logger;
typedef Logger<String_aggregator<"">, Safe_handler<Stdout_handler> > basic_thread_safe_logger;
typedef Logger<String_aggregator<"">, Safe_handler<Stderr_handler> > basic_thread_safe_error_logger;

typedef Logger<String_aggregator<"[Fatal]">,
               Safe_handler<File_Handler<String_aggregator<"fatal_errors.log">, false, Open_failed_behaviour_factory, std::string, "throw"> > > fatal_error_logger;

typedef Logger<Date_aggregator<String_aggregator<"[Error]"> >,
               Stderr_handler,
               File_Handler<String_aggregator<base_dir, Date_aggregator<String_aggregator<"_errors.log"> > >, true, Open_failed_behaviour_factory, std::string, "throw"> > error_logger;
typedef Logger<Date_aggregator<String_aggregator<"[Error]"> >,
               Stderr_handler,
               Safe_handler<File_Handler<String_aggregator<base_dir, Date_aggregator<String_aggregator<"_errors.log"> > >, true, Open_failed_behaviour_factory, std::string, "throw"> > > thread_safe_error_logger;

typedef Logger<Date_aggregator<String_aggregator<"[Warning]"> >,
               Stderr_handler,
               File_Handler<String_aggregator<base_dir, Date_aggregator<String_aggregator<"_warning.log"> > >, true, Open_failed_behaviour_factory, std::string, "log"> > warning_logger;
typedef Logger<Date_aggregator<String_aggregator<"[Warning]"> >,
               Stderr_handler,
               Safe_handler<File_Handler<String_aggregator<base_dir, Date_aggregator<String_aggregator<"_warning.log"> > >, true, Open_failed_behaviour_factory, std::string, "log"> > > thread_safe_warning_logger;

typedef Logger<String_aggregator<"[Info]">,
               Stdout_handler,
               File_Handler<String_aggregator<base_dir, Date_aggregator<String_aggregator<"_info.log"> > >, true, Open_failed_behaviour_factory, std::string, "log"> > info_logger;
typedef Logger<String_aggregator<"[Info]">,
               Stdout_handler,
               Safe_handler<File_Handler<String_aggregator<base_dir, Date_aggregator<String_aggregator<"_info.log"> > >, true, Open_failed_behaviour_factory, std::string, "log"> > > thread_safe_info_logger;

typedef Logger<Date_aggregator<String_aggregator<"[Error]"> >,
               Stderr_handler,
               File_Handler<String_aggregator<base_dir+"main_errors.log">, false, Open_failed_behaviour_factory, std::string, "throw"> > error_to_keep_logger;
typedef Logger<Date_aggregator<String_aggregator<"[Error]"> >,
               Stderr_handler,
               Safe_handler<File_Handler<String_aggregator<base_dir+"main_errors.log">, false, Open_failed_behaviour_factory, std::string, "throw"> > > thread_safe_error_to_keep_logger;

typedef Logger<String_aggregator<"[Debug]", Date_aggregator>,
               Stdout_handler,
               File_Handler<String_aggregator<base_dir+"current_debug.log">, true, Open_failed_behaviour_factory, std::string, "log"> > debug_logger;
typedef Logger<String_aggregator<"[Debug]", Date_aggregator>,
               Stdout_handler,
               Safe_handler<File_Handler<String_aggregator<base_dir+"current_debug.log">, true, Open_failed_behaviour_factory, std::string, "log"> > > thread_safe_debug_logger;

// TODO : implement colored leveled logger


#endif
