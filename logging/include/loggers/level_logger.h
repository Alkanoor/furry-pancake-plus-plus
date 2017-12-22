#ifndef LEVEL_LOGGER_H
#define LEVEL_LOGGER_H


#include "logger.h"


template <int Level, typename Handler, bool Level_must_be_equal = false>
class Level_handler
{};

template <typename ... Args>
class Level_logger;

template <int Level, typename Handler, bool Level_must_be_equal, typename ... Following>
class Level_logger<Level_handler<Level, Handler, Level_must_be_equal>, Following ...> : public Level_logger<Following ...>
{
    public:
        static void set_level(int level)
        {
            _current_level = level;
        }

        template <typename ... T>
        static std::ostream& log(std::ostream& os, T&& ... data)
        {
            _impl_Logger_helper::_impl_detail<T ...>::write_to_ostream(os, std::forward<T>(data) ...);
            os<<std::endl;

            Handler::write_endline(os, std::forward<T>(data) ...);
            Level_logger<Following ...>::log_private(os, std::forward<T>(data) ...);
            return os;
        }

        template <typename ... T>
        static bool write(T&& ... data)
        {
            return write_private(_current_level, std::forward<T>(data) ...);
        }

        template <typename ... T>
        static bool write_endline(T&& ... data)
        {
            return write_endline_private(_current_level, std::forward<T>(data) ...);
        }

    protected:
        template <typename ... T>
        static std::ostream& log_private(std::ostream& os, T&& ... data)
        {
            Handler::write_endline(os, std::forward<T>(data) ...);
            Level_logger<Following ...>::log_private(os, std::forward<T>(data) ...);
            return os;
        }

        template <typename ... T>
        static bool write_private(int current_level, T&& ... data)
        {
            bool tmp1 = true;
            if((!Level_must_be_equal && current_level>=Level) || (Level_must_be_equal && current_level==Level))
                tmp1 = Handler::write(std::forward<T>(data) ...);
            bool tmp2 = Level_logger<Following ...>::write_private(current_level, std::forward<T>(data) ...);
            return tmp1 && tmp2;
        }

        template <typename ... T>
        static bool write_endline_private(int current_level, T&& ... data)
        {
            bool tmp1 = true;
            if((!Level_must_be_equal && current_level>=Level) || (Level_must_be_equal && current_level==Level))
                tmp1 = Handler::write_endline(std::forward<T>(data) ...);
            bool tmp2 = Level_logger<Following ...>::write_endline_private(current_level, std::forward<T>(data) ...);
            return tmp1 && tmp2;
        }

    private:
        static int _current_level;
};

template <int Level, typename Handler, bool Level_must_be_equal>
class Level_logger<Level_handler<Level, Handler, Level_must_be_equal> >
{
    public:
        static void set_level(int level)
        {
            _current_level = level;
        }

        template <typename ... T>
        static std::ostream& log(std::ostream& os, T&& ... data)
        {
            _impl_Logger_helper::_impl_detail<T ...>::write_to_ostream(os, std::forward<T>(data) ...);
            os<<std::endl;

            Handler::write_endline(os, std::forward<T>(data) ...);
            return os;
        }

        template <typename ... T>
        static bool write(T&& ... data)
        {
            if((!Level_must_be_equal && _current_level>=Level) || (Level_must_be_equal && _current_level==Level))
                return Handler::write(std::forward<T>(data) ...);
            return true;
        }

        template <typename ... T>
        static bool write_endline(T&& ... data)
        {
            if((!Level_must_be_equal && _current_level>=Level) || (Level_must_be_equal && _current_level==Level))
                return Handler::write_endline(std::forward<T>(data) ...);
            return true;
        }

    protected: // write_private and write_endline_private are declared for compatibility reasons
        template <typename ... T>
        static bool write_private(int current_level, T&& ... data)
        {
            if((!Level_must_be_equal && current_level>=Level) || (Level_must_be_equal && current_level==Level))
                return Handler::write(std::forward<T>(data) ...);
            return true;
        }

        template <typename ... T>
        static bool write_endline_private(int current_level, T&& ... data)
        {
            if((!Level_must_be_equal && current_level>=Level) || (Level_must_be_equal && current_level==Level))
                return Handler::write_endline(std::forward<T>(data) ...);
            return true;
        }

    private:
        static int _current_level;
};


template <int Level, typename Handler, bool Level_must_be_equal, typename ... Following>
int Level_logger<Level_handler<Level, Handler, Level_must_be_equal>, Following ...>::_current_level = 0;

template <int Level, typename Handler, bool Level_must_be_equal>
int Level_logger<Level_handler<Level, Handler, Level_must_be_equal> >::_current_level = 0;


#endif
