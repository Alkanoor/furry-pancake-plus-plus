#ifndef TERMINAL_COLOR_AGGREGATOR_H
#define TERMINAL_COLOR_AGGREGATOR_H


#include "string_aggregator.h"


static constexpr const char black[] = "\x1B[30m";
static constexpr const char red[] = "\x1B[31m";
static constexpr const char green[] = "\x1B[32m";
static constexpr const char orange[] = "\x1B[33m";
static constexpr const char yellow[] = "\x1B[93m";
static constexpr const char blue[] = "\x1B[34m";
static constexpr const char magenta[] = "\x1B[35m";
static constexpr const char cyan[] = "\x1B[36m";
static constexpr const char white[] = "\x1B[37m";
static constexpr const char reset[] = "\x1B[0m";


template <typename Aggregator = void>
using Black_aggregator = String_aggregator<black, reset, Aggregator>;
using Basic_black_aggregator = String_aggregator<black, reset>;

template <typename Aggregator = void>
using Red_aggregator = String_aggregator<red, reset, Aggregator>;
using Basic_red_aggregator = String_aggregator<red, reset>;

template <typename Aggregator = void>
using Green_aggregator = String_aggregator<green, reset, Aggregator>;
using Basic_green_aggregator = String_aggregator<green, reset>;

template <typename Aggregator = void>
using Orange_aggregator = String_aggregator<orange, reset, Aggregator>;
using Basic_orange_aggregator = String_aggregator<orange, reset>;

template <typename Aggregator = void>
using Yellow_aggregator = String_aggregator<yellow, reset, Aggregator>;
using Basic_yellow_aggregator = String_aggregator<yellow, reset>;

template <typename Aggregator = void>
using Blue_aggregator = String_aggregator<blue, reset, Aggregator>;
using Basic_blue_aggregator = String_aggregator<blue, reset>;

template <typename Aggregator = void>
using Magenta_aggregator = String_aggregator<magenta, reset, Aggregator>;
using Basic_magenta_aggregator = String_aggregator<magenta, reset>;

template <typename Aggregator = void>
using Cyan_aggregator = String_aggregator<cyan, reset, Aggregator>;
using Basic_cyan_aggregator = String_aggregator<cyan, reset>;

template <typename Aggregator = void>
using White_aggregator = String_aggregator<white, reset, Aggregator>;
using Basic_white_aggregator = String_aggregator<white, reset>;


#endif
