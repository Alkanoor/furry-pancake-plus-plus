#include "../../utils/include/ostream_writable.hpp"
#include "../include/anomaly_detector.hpp"
#include <cstdlib>
#include <cstdio>
#include <ctime>


void hex_print(const std::string& s)
{
    for(auto i : s)
        printf("%02x", i);
    std::cout<<std::endl;
}

void print_anomaly(const std::string& anomaly, double weight, const Anomaly_detector<std::string, Entropy_discriminator, double, Kmeans>& detector)
{
    std::cout<<"Anormality detected !"<<std::endl;
    std::cout<<"It has been weighted "<<weight<<" with following detector properties : "<<detector<<std::endl;
    hex_print(anomaly);
}

int main()
{
    Kmeans<double> double_kmeans;
    Anomaly_detector<std::string, Entropy_discriminator, double, Kmeans> detector(double_kmeans, 20, print_anomaly);

    srand(time(nullptr));
    for(int i=0; i<1000; i++)
    {
        std::string s = generate_random_string(150+rand()%300);
        if((rand()%150) == 0)
            for(int j=0; j<4+rand()%10; j++)
                s += "a";

        detector.feed(s);
    }

    return 0;
}
