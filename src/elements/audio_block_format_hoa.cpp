#include "adm/elements/audio_block_format_hoa.hpp"

namespace adm {

  namespace {
    const Rtime rtimeDefault{std::chrono::seconds(0)};
  }

  // ---- Getter ---- //
  AudioBlockFormatId AudioBlockFormatHoa::get(
      detail::ParameterTraits<AudioBlockFormatId>::tag) const {
    return id_;
  }
  Rtime AudioBlockFormatHoa::get(detail::ParameterTraits<Rtime>::tag) const {
    return boost::get_optional_value_or(rtime_, rtimeDefault);
  }
  Duration AudioBlockFormatHoa::get(
      detail::ParameterTraits<Duration>::tag) const {
    return duration_.get();
  }

  // ---- Has ---- //
  bool AudioBlockFormatHoa::has(
      detail::ParameterTraits<AudioBlockFormatId>::tag) const {
    return true;
  }
  bool AudioBlockFormatHoa::has(detail::ParameterTraits<Rtime>::tag) const {
    return true;
  }
  bool AudioBlockFormatHoa::has(detail::ParameterTraits<Duration>::tag) const {
    return duration_ != boost::none;
  }

  // ---- isDefault ---- //
  bool AudioBlockFormatHoa::isDefault(
      detail::ParameterTraits<Rtime>::tag) const {
    return duration_ == boost::none;
  }

  // ---- Setter ---- //
  void AudioBlockFormatHoa::set(AudioBlockFormatId id) { id_ = id; }
  void AudioBlockFormatHoa::set(Rtime rtime) { rtime_ = rtime; }
  void AudioBlockFormatHoa::set(Duration duration) { duration_ = duration; }

  // ---- Unsetter ---- //
  void AudioBlockFormatHoa::unset(detail::ParameterTraits<Rtime>::tag) {
    rtime_ = boost::none;
  }
  void AudioBlockFormatHoa::unset(detail::ParameterTraits<Duration>::tag) {
    duration_ = boost::none;
  }

}  // namespace adm
