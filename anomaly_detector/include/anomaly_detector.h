#ifndef ANOMALY_DETECTOR_H
#define ANOMALY_DETECTOR_H


#include <functional>
#include <ostream>


template <typename In, typename <template <typename, typename> class Discriminator>, typename Out, typename Dist=Out, typename Clustering, typename Proba_type = double>
class Anomaly_detector
{
    public:
        Anomaly_detector(const Clustering<Dist, Out, Proba_type>& clustering_method,
                         const std::function<void(const Out&, Dist, const Anomaly_detector<In, Discriminator, Out, Dist, Clustering, Proba_type>& detector));

        void log(std::ostream& os);

    private:
        Proba_type _abnormalities_threshold;
};


#endif
