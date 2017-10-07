#include "../../logging/include/handlers/pool_ostream_handler.h"
#include "../../logging/include/handlers/dynamic_file_handler.h"
#include "../../logging/include/loggers/common_loggers.h"


static constexpr const char ok[] = "ok";
static constexpr const char google[] = "googl";
static constexpr const char end[] = "end";
static constexpr const char mdr[] = "  AMD ";
static constexpr const char fin[] = " FINFIN ";
static constexpr const char throw_keyword[] = "throw";


class Throwing_handler : public Ostream_handler<Throwing_handler>
{
    friend class Handler<Throwing_handler>;

    public:
        static bool initialize()
        {
            throw std::runtime_error("Error: Unknown exception caught.");
            return true;
        }
};


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

    typedef Dynamic_file_handler<Open_failed_behaviour_factory, const char*, Open_failed_behaviour_factory::throw_keyword> dynamic_handler;
    dynamic_handler first_dyn("logs/inexisting.qosdk");
    dynamic_handler sec_dyn("logs_/inexisting.qosdk");


    typedef Pool_ostream_handler<int, 0> pool1;
    typedef Pool_ostream_handler<int, 1> pool2;
    typedef Pool_ostream_handler<int, 2> pool3;

    pool1::add_stream<debug_handler>();
    pool1::add_stream<dynamic_handler>(&sec_dyn);
    pool1::add_stream<dynamic_handler>(&first_dyn);

    try
    {
        pool1::write_endline("First test see behaviour file not found");
    }
    catch(const std::runtime_error& e)
    {
        std::cout<<"Exception caught (1): "<<e.what()<<std::endl;
    }


    pool2::add_stream<dated_severe_handler>();
    pool2::add_stream<Throwing_handler>();
    pool2::add_stream<dated_info_handler>();

    try
    {
        pool2::write_endline("Should throw after first print");
    }
    catch(const std::runtime_error & e)
    {
        std::cout<<"Exception caught (2): "<<e.what()<<std::endl;
    }


    pool3::add_stream<debug_handler>();
    pool3::add_stream<dynamic_handler>(&first_dyn);

    try
    {
        pool3::write_endline("Third test see behaviour file not found");
    }
    catch(const std::runtime_error& e)
    {
        std::cout<<"Exception caught (3): "<<e.what()<<std::endl;
    }

    debug_logger::set_level(GREAT);
    debug_logger::write_endline("This is great");
    debug_logger::set_level(SUPER);
    debug_logger::write_endline("This is super");
    debug_logger::set_level(GOOD);
    debug_logger::write_endline("This is good");
    debug_logger::set_level(QUESTION);
    debug_logger::write_endline("This is question");
    debug_logger::set_level(SURPRISING);
    debug_logger::write_endline("This is surprising");
    debug_logger::set_level(NEUTRAL);
    debug_logger::write_endline("This is neutral");
    debug_logger::set_level(NEGATIVE);
    debug_logger::write_endline("This is negative");
    debug_logger::set_level(BAD);
    debug_logger::write_endline("This is bad");
    debug_logger::set_level(CRITICAL);
    debug_logger::write_endline("This is critical");

    return 0;
}
