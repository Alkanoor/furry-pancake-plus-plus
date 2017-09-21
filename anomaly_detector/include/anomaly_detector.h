#ifndef ANOMALY_DETECTOR_H
#define ANOMALY_DETECTOR_H


#include <functional>
#include <ostream>


template <typename In, typename <template <typename, typename> class Discriminator>, typename Out, typename Dist=Out, typename Clustering, typename Proba = double>
class Anomaly_detector
{
    public:
        Anomaly_detector(const Clustering<Dist, Out, Proba>& clustering_method,
                         int number_inputs_before_first_clustering = 10, Proba abnormalities_threshold = 0.2,
                         const std::function<void(const Out&, Dist, const Anomaly_detector<In, Discriminator, Out, Dist, Clustering, Proba>& detector));

        bool feed(const In& data);
        Proba feed(const In& data, int& cluster);

        void log(std::ostream& os);

    private:
        Proba _abnormalities_threshold;
};


#endif
