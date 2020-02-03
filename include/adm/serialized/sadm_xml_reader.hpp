/// @file xml_reader.hpp
#pragma once
#include <string>
#include <memory>
#include <iosfwd>
#include "adm/detail/enum_bitmask.hpp"
#include "adm/export.h"
#include "adm/frame.hpp"

namespace adm {

  class Frame;

  namespace xml {
    /**
     * @brief Representation of available options to influence the behaviour
     * of the XML parser.
     *
     * `ParserOptions` satisfies the requirements of
     * [BitmaskType](http://en.cppreference.com/w/cpp/concept/BitmaskType).
     *
     * This means that the bitwise operators (e.g. `operator|` or `operator&`)
     * are defined for this type. Thus options may be combined by `OR`-ing the
     * respective values.
     *
     * @note No options have been implemented so far. As soon as this is done,
     * provide an usage example, a list describing the member constants
     * (options) and which options can be combined. Refer to
     * [std::filesystem::copy_options](http://en.cppreference.com/w/cpp/filesystem/copy_options)
     * for an example.
     * @ingroup xml
     */
    enum class ParserOptions : unsigned {
      none = 0x0  ///< default behaviour
    };
  }  // namespace xml

  /**
   * @addtogroup xml XML parsing and generation
   * @brief XML parsing and generation
   * @{
   */

  /**
   * @brief Parse an XML representation of the Audio Definition Model
   *
   * Read adm data from an `std::istream`.
   * @param stream input stream to read XML data
   * @param options Options to influence the XML parser behaviour
   */
  ADM_EXPORT std::shared_ptr<Frame> parseSadmXml(
      std::istream& stream,
      xml::ParserOptions options = xml::ParserOptions::none);

  /**
   * @}
   */
}  // namespace adm

ENABLE_ENUM_BITMASK_OPERATORS(adm::xml::ParserOptions);
