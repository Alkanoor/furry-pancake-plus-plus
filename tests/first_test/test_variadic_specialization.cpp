#include "../../logging/include/aggregators/date_aggregator.h"
#include "../../logging/include/aggregators/terminal_color_aggregator.h"
#include "../../logging/include/handlers/aggregator_handler.h"
#include "../../logging/include/handlers/file_handler.h"
#include "../../logging/include/open_failed_behaviour_factory.h"
#include "../../logging/include/loggers/common_loggers.h"



static constexpr const char ok[] = "ok";
static constexpr const char google[] = "googl";
static constexpr const char end[] = "end";
static constexpr const char mdr[] = "  AMD ";
static constexpr const char fin[] = " FINFIN ";
static constexpr const char filename[] = "lol.txt";
static constexpr const char throw_keyword[] = "throw";



int main()
{
    std::cout<<String_aggregator<google, ok>::aggregate_tail("le temps long")<<std::endl;
    std::cout<<String_aggregator<ok, end, String_header<google> >::aggregate_tail("mdr")<<std::endl;
    std::cout<<String_header<ok, String_aggregator<google, end> >::aggregate("mdr")<<std::endl;
    std::cout<<String_aggregator<ok, end, String_aggregator<google, ok> >::aggregate_tail("  aaa ")<<std::endl;
    std::cout<<String_aggregator<ok, end, String_aggregator<google, ok, Date_aggregator<String_aggregator<mdr, fin, Basic_date_aggregator> > > >::aggregate("  aaa ")<<std::endl;
    std::cout<<String_aggregator<ok, end, String_aggregator<google, ok, Date_aggregator<String_aggregator<mdr, fin> > > >::aggregate_tail("  aaa ")<<std::endl;

    std::cout<<String_header<ok, String_aggregator<google, end, Red_aggregator<Date_aggregator<String_header<mdr> > > > >::aggregate(" hihi ")<<std::endl;
    std::cout<<String_header<ok, String_aggregator<google, end, Red_aggregator<Date_aggregator<String_header<mdr> > > > >::aggregate_tail(" hihi ")<<std::endl;

    std::cout<<Red_aggregator<String_aggregator<ok, google, Yellow_aggregator<String_aggregator<ok, google, Blue_aggregator<String_aggregator<ok, google,
                                Green_aggregator<String_aggregator<ok, google, Magenta_aggregator<String_aggregator<ok, google, White_aggregator<String_aggregator<ok, google, Orange_aggregator<String_aggregator<end, fin>>>>>>>>>>>>>>::aggregate("=======")<<std::endl;
    std::cout<<Red_aggregator<String_aggregator<ok, google, Yellow_aggregator<String_aggregator<ok, google, Blue_aggregator<String_aggregator<ok, google,
                                Green_aggregator<String_aggregator<ok, google, Magenta_aggregator<String_aggregator<ok, google, White_aggregator<String_aggregator<ok, google, Orange_aggregator<String_aggregator<end, fin>>>>>>>>>>>>>>::aggregate_tail("=======")<<std::endl;


    basic_logger::stream<<"ooooooooooooo";
    basic_logger::write_endline("ppppppppp");
    basic_timed_logger::write_endline("Ahahah");
    std::cout<<std::endl<<std::endl;

    error_logger::write_endline("ici");

    dated_severe_red_logger::write_endline("ce para", 2);
    dated_errors_orange_logger::write_endline("f0l13");
    dated_warning_yellow_logger::write_endline("montre plein ", 2);
    dated_info_white_logger::write_endline("couleurs");

    error_logger::write_endline("fin erreur");

    dated_warning_yellow_logger::log(std::cout, "mdrrr", 2, "PLOL")<<"mdddd"<<std::endl;

    dated_level_logger::write_endline("EROOR warning info SEVER");
    dated_level_logger::set_level(3);
    dated_level_logger::write_endline("OOOOO");

    dated_level_logger::set_level(2);
    dated_level_logger::write_endline("not so important");

    debug_recursive_level_logger::set_level(3);
    debug_recursive_level_logger::write_endline("ceci est un test");
    debug_recursive_level_logger::set_debug(false);
    debug_recursive_level_logger::write_endline("should not be printed");
    debug_recursive_level_logger::set_debug(true);
    debug_recursive_level_logger::write_endline("nice");

    logger::write_endline("LE THREAD SAFE MARCHE E TTOU MARCHE");
    logger::debug(false);
    logger::set_level(2);
    logger::write("not printed");
    logger::debug(true);
    logger::write_endline("now printed");
    logger::set_level(1);
    logger::write_endline("cerise sur le gateau d'un beau logger");

    logger::write_endline(std::vector<int>({123,123,234,3,3230}));

    return 0;
}
