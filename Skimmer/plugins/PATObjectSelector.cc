#include "DataFormats/Common/interface/RefVector.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectCountFilter.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include <vector>

namespace pat {


  typedef SingleObjectSelector<
              std::vector<PackedCandidate>,
              StringCutObjectSelector<PackedCandidate>
          > PATPackedCandidateSelector;
  typedef SingleObjectSelector<
              std::vector<PackedCandidate>,
              StringCutObjectSelector<PackedCandidate>,
              edm::RefVector<std::vector<PackedCandidate> >
          > PATPackedCandidateRefSelector;
}

#include "FWCore/Framework/interface/MakerMacros.h"

using namespace pat;

DEFINE_FWK_MODULE(PATPackedCandidateSelector);
DEFINE_FWK_MODULE(PATPackedCandidateRefSelector);
