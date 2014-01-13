#ifndef HISTOGRAM_TOOLS_HH
#define HISTOGRAM_TOOLS_HH

#include <TH1.h>

namespace math {

  class HistogramTools {
  public:
    /// constructor from TH1
    HistogramTools(TH1 *histo) : histo_(histo) { };
    /// destructor
    ~HistogramTools() { };

    /// get wffective RMS
    double effectiveSigma();

  private:
    TH1 *histo_;
  };
}

#endif

