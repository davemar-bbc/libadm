#include "adm/private/xml_writer.hpp"
#include "adm/elements.hpp"
#include "adm/document.hpp"
#include "adm/frame.hpp"
#include "adm/private/rapidxml_formatter.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "adm/private/rapidxml_wrapper.hpp"

namespace adm {
  namespace xml {

    /// Check if a option/flag is set
    /**
     * Checks if the option @a flag is set within @a options.
     *
     * This is equivalent to an bitwise AND followed by a conversion to bool,
     * but should improve readability.
     */
    inline bool isSet(WriterOptions options, WriterOptions flag) {
      return static_cast<bool>(options & flag);
    }

    using NodePtr = rapidxml::xml_node<>*;

    XmlWriter::XmlWriter(WriterOptions options) : options_(options) {}

    std::ostream& XmlWriter::write(std::shared_ptr<const Document> document,
                                   std::ostream& stream) {
      XmlDocument xmlDocument;
      xmlDocument.setDiscardDefaults(
          !isSet(options_, WriterOptions::write_default_values));
      xmlDocument.addDeclaration();
      XmlNode root;
      if (isSet(options_, WriterOptions::itu_structure)) {
        root = xmlDocument.addItuStructure();
      } else {
        root = xmlDocument.addEbuStructure();
      }
      // clang-format off
      root.addBaseElements<AudioProgramme, AudioProgrammeId>(document, "audioProgramme", &formatAudioProgramme);
      root.addBaseElements<AudioContent, AudioContentId>(document, "audioContent", &formatAudioContent);
      root.addBaseElements<AudioObject, AudioObjectId>(document, "audioObject", &formatAudioObject);
      root.addBaseElements<AudioPackFormat, AudioPackFormatId>(document, "audioPackFormat", &formatAudioPackFormat);
      root.addBaseElements<AudioChannelFormat, AudioChannelFormatId>(document, "audioChannelFormat", &formatAudioChannelFormat);
      root.addBaseElements<AudioStreamFormat, AudioStreamFormatId>(document, "audioStreamFormat", &formatAudioStreamFormat);
      root.addBaseElements<AudioTrackFormat, AudioTrackFormatId>(document, "audioTrackFormat", &formatAudioTrackFormat);

      for (auto &element : document->template getElements<AudioTrackUid>()) {
        auto id = element->template get<AudioTrackUidId>();
        if (!isCommonDefinitionsId(id) && !element->isSilent()) {
          root.addElement(element, "audioTrackUID", &formatAudioTrackUid);
        }
      }
      // clang-format on
      return stream << xmlDocument;
    }

    std::ostream& XmlWriter::write(std::shared_ptr<const Frame> frame,
                                   std::ostream& stream) {
      XmlDocument xmlDocument;
      xmlDocument.setDiscardDefaults(
          !isSet(options_, WriterOptions::write_default_values));
      xmlDocument.addDeclaration();
      auto root = xmlDocument.addNode("frame");
      root.addAttribute("version", "ITU-R_BS.2125-1");
      root.addElement(frame->frameHeader(), "frameHeader", &formatFrameHeader);
      auto formatExtended = root.addNode("audioFormatExtended");
      formatExtended.addAttribute("version", "ITU-R_BS.2076-2");
      // clang-format off
      formatExtended.addBaseElements<AudioProgramme, AudioProgrammeId>(frame, "audioProgramme", &formatAudioProgramme);
      formatExtended.addBaseElements<AudioContent, AudioContentId>(frame, "audioContent", &formatAudioContent);
      formatExtended.addBaseElements<AudioObject, AudioObjectId>(frame, "audioObject", &formatAudioObject);
      formatExtended.addBaseElements<AudioPackFormat, AudioPackFormatId>(frame, "audioPackFormat", &formatAudioPackFormat);
      formatExtended.addBaseElements<AudioChannelFormat, AudioChannelFormatId>(frame, "audioChannelFormat", &formatAudioChannelFormat);
      formatExtended.addBaseElements<AudioStreamFormat, AudioStreamFormatId>(frame, "audioStreamFormat", &formatAudioStreamFormat);
      formatExtended.addBaseElements<AudioTrackFormat, AudioTrackFormatId>(frame, "audioTrackFormat", &formatAudioTrackFormat);

      for (auto &element : frame->template getElements<AudioTrackUid>()) {
        auto id = element->template get<AudioTrackUidId>();
        if (!isCommonDefinitionsId(id) && !element->isSilent()) {
          formatExtended.addElement(element, "audioTrackUID", &formatAudioTrackUid);
        }
      }      // clang-format on
      return stream << xmlDocument;
    }

  }  // namespace xml
}  // namespace adm
