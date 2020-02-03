#pragma once

#include "adm/document.hpp"
#include "adm/frame.hpp"

namespace adm {

  ADM_EXPORT std::shared_ptr<Document> deepCopy(
      std::shared_ptr<const Document> document);

  ADM_EXPORT void deepCopyTo(std::shared_ptr<const Document> src,
                             std::shared_ptr<Document> dest);

  ADM_EXPORT void deepCopyTo(std::shared_ptr<const Document> src,
                             std::shared_ptr<Frame> dest);

}  // namespace adm
