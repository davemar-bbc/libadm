/// @file time.hpp
#pragma once

#include <chrono>
#include <string>
#include "adm/detail/named_type.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for NamedType ::Start
  struct StartTag {};
  /// @brief NamedType for the start attribute
  using Start = detail::NamedType<std::chrono::nanoseconds, StartTag>;
  /// @brief Tag for NamedType ::End
  struct EndTag {};
  /// @brief NamedType for the end attribute
  using End = detail::NamedType<std::chrono::nanoseconds, EndTag>;
  /// @brief Tag for NamedType ::Duration
  struct DurationTag {};
  /// @brief NamedType for the duration attribute
  using Duration = detail::NamedType<std::chrono::nanoseconds, DurationTag>;
  /// @brief Tag for NamedType ::Rtime
  struct RtimeTag {};
  /// @brief NamedType for the rtime attribute
  using Rtime = detail::NamedType<std::chrono::nanoseconds, RtimeTag>;
  /// @brief Tag for NamedType ::Start
  struct LstartTag {};
  /// @brief NamedType for the lstart attribute
  using Lstart = detail::NamedType<std::chrono::nanoseconds, LstartTag>;
  /// @brief Tag for NamedType ::Lduration
  struct LdurationTag {};
  /// @brief NamedType for the lduration attribute
  using Lduration = detail::NamedType<std::chrono::nanoseconds, LdurationTag>;
  /// @brief Tag for NamedType ::FrameStart
  struct FrameStartTag {};
  /// @brief NamedType for the sadm start attribute
  using FrameStart = detail::NamedType<std::chrono::nanoseconds, FrameStartTag>;
  /// @brief Tag for NamedType ::FrameDuration
  struct FrameDurationTag {};
  /// @brief NamedType for the sadm duration attribute
  using FrameDuration =
      detail::NamedType<std::chrono::nanoseconds, FrameDurationTag>;

  /// @brief Parse an adm timecode and convert it to a std::chrono::duration
  ADM_EXPORT std::chrono::nanoseconds parseTimecode(
      const std::string& timecode);
  /// @brief Format a std::chrono::duration object as an adm timecode string
  ADM_EXPORT std::string formatTimecode(const std::chrono::nanoseconds& time);

}  // namespace adm
