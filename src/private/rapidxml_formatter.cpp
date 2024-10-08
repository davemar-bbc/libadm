#include "adm/private/rapidxml_formatter.hpp"
#include "adm/private/rapidxml_wrapper.hpp"
#include "adm/serial/frame_format_id.hpp"
#include <adm/serial/changed_ids.hpp>
#include "adm/private/changed_id_traits.hpp"

namespace adm {
  namespace xml {

    namespace detail {
      template <typename F>
      auto wrapWithTimeRef(F fn, TimeReference timeReference) {
        return [fn, timeReference](XmlNode &node, auto const &element) {
          fn(node, element, timeReference);
        };
      }

      template <typename T>
      void addBlockTimeParameters(T const &block, XmlNode &node,
                                  TimeReference timeReference) {
        if (timeReference == TimeReference::TOTAL) {
          node.addOptionalAttribute<Rtime>(&block, "rtime");
          node.addOptionalAttribute<Duration>(&block, "duration");
        } else {
          node.addOptionalAttribute<Rtime>(&block, "lstart");
          node.addOptionalAttribute<Duration>(&block, "lduration");
        }
      }

      std::string toString(const std::string &string) { return string; }
      std::string toString(const Time &time) { return formatTimecode(time); }
      std::string toString(const AudioProgrammeId &id) { return formatId(id); }
      std::string toString(const AudioContentId &id) { return formatId(id); }
      std::string toString(const AudioObjectId &id) { return formatId(id); }
      std::string toString(const AudioPackFormatId &id) { return formatId(id); }
      std::string toString(const AudioChannelFormatId &id) {
        return formatId(id);
      }
      std::string toString(const AudioBlockFormatId &id) {
        return formatId(id);
      }
      std::string toString(const AudioStreamFormatId &id) {
        return formatId(id);
      }
      std::string toString(const AudioTrackFormatId &id) {
        return formatId(id);
      }
      std::string toString(const AudioTrackUidId &id) { return formatId(id); }
      std::string toString(const FrameFormatId &id) { return formatId(id); }
      std::string toString(const TransportId &id) { return formatId(id); }
      std::string toString(FrameType frameType) {
        return formatValue(frameType);
      }
      std::string toString(ChangedIdStatus status) {
        return formatValue(status);
      }
      std::string toString(TimeReference timeReference) {
        return formatValue(timeReference);
      }

      struct MultiElementAttributeFormatter {
        MultiElementAttributeFormatter(const std::string &a,
                                       const std::string &v)
            : attribute(a), value(v) {}
        template <typename Element>
        void operator()(XmlNode &node, Element element) {
          node.addAttribute(attribute, value);
          node.setValue(element.get());
        }
        std::string attribute;
        std::string value;
      };

      MultiElementAttributeFormatter formatMultiElementAttribute(
          const std::string &attribute, const std::string &value) {
        return MultiElementAttributeFormatter(attribute, value);
      }

    }  // namespace detail

    void formatAudioProgramme(
        XmlNode &node, const std::shared_ptr<const AudioProgramme> programme) {
      // clang-format off
      node.addAttribute<AudioProgrammeId>(programme, "audioProgrammeID");
      node.addOptionalAttribute<AudioProgrammeName>(programme, "audioProgrammeName");
      node.addOptionalAttribute<AudioProgrammeLanguage>(programme, "audioProgrammeLanguage");
      node.addOptionalAttribute<Start>(programme, "start");
      node.addOptionalAttribute<End>(programme, "end");
      node.addOptionalAttribute<MaxDuckingDepth>(programme, "maxDuckingDepth");
      node.addReferences<AudioContent, AudioContentId>(programme, "audioContentIDRef");
      node.addVectorElements<LoudnessMetadatas>(programme, "loudnessMetadata", &formatLoudnessMetadata);
      node.addVectorElements<Labels>(programme, "audioProgrammeLabel", &formatLabel);
      // clang-format on
    }

    void formatLoudnessMetadata(XmlNode &node,
                                const LoudnessMetadata loudnessMetadata) {
      node.addOptionalAttribute<LoudnessMethod>(&loudnessMetadata,
                                                "loudnessMethod");
      node.addOptionalAttribute<LoudnessRecType>(&loudnessMetadata,
                                                 "loudnessRecType");
      node.addOptionalAttribute<LoudnessCorrectionType>(
          &loudnessMetadata, "loudnessCorrectionType");
      node.addOptionalElement<IntegratedLoudness>(&loudnessMetadata,
                                                  "integratedLoudness");
      node.addOptionalElement<LoudnessRange>(&loudnessMetadata,
                                             "loudnessRange");
      node.addOptionalElement<MaxTruePeak>(&loudnessMetadata, "maxTruePeak");
      node.addOptionalElement<MaxMomentary>(&loudnessMetadata, "maxMomentary");
      node.addOptionalElement<MaxShortTerm>(&loudnessMetadata, "maxShortTerm");
      node.addOptionalElement<DialogueLoudness>(&loudnessMetadata,
                                                "dialogueLoudness");
    }

    void formatAudioContent(XmlNode &node,
                            std::shared_ptr<const AudioContent> content) {
      // clang-format off
      node.addAttribute<AudioContentId>(content, "audioContentID");
      node.addOptionalAttribute<AudioContentName>(content, "audioContentName");
      node.addOptionalAttribute<AudioContentLanguage>(content, "audioContentLanguage");
      node.addReferences<AudioObject, AudioObjectId>(content, "audioObjectIDRef");
      node.addVectorElements<LoudnessMetadatas>(content, "loudnessMetadata", &formatLoudnessMetadata);
      node.addOptionalElement<NonDialogueContentKind>(content, "dialogue", &formatNonDialogueContentKind);
      node.addOptionalElement<DialogueContentKind>(content, "dialogue", &formatDialogueContentKind);
      node.addOptionalElement<MixedContentKind>(content, "dialogue", &formatMixedContentKind);
      node.addVectorElements<Labels>(content, "audioContentLabel", &formatLabel);
      // clang-format on
    }

    void formatNonDialogueContentKind(
        XmlNode &node, const NonDialogueContentKind contentKind) {
      // clang-format off
      node.addAttribute("nonDialogueContentKind", std::to_string(contentKind.get()));
      node.setValue(std::string("0"));
      // clang-format on
    }

    void formatDialogueContentKind(XmlNode &node,
                                   const DialogueContentKind contentKind) {
      // clang-format off
      node.addAttribute("dialogueContentKind", std::to_string(contentKind.get()));
      node.setValue(std::string("1"));
      // clang-format on
    }

    void formatMixedContentKind(XmlNode &node,
                                const MixedContentKind contentKind) {
      // clang-format off
      node.addAttribute("mixedContentKind", std::to_string(contentKind.get()));
      node.setValue(std::string("2"));
      // clang-format on
    }

    void formatAudioObject(XmlNode &node,
                           std::shared_ptr<const AudioObject> object) {
      // clang-format off
      node.addAttribute<AudioObjectId>(object, "audioObjectID");
      node.addOptionalAttribute<AudioObjectName>(object, "audioObjectName");
      node.addOptionalAttribute<Start>(object, "start");
      node.addOptionalAttribute<Duration>(object, "duration");
      node.addOptionalAttribute<DialogueId>(object, "dialogue");
      node.addOptionalAttribute<Importance>(object, "importance");
      node.addOptionalAttribute<Interact>(object, "interact");
      node.addOptionalAttribute<DisableDucking>(object, "disableDucking");
      node.addOptionalElement<AudioObjectInteraction>(object, "audioObjectInteraction", &formatAudioObjectInteraction);
      node.addReferences<AudioPackFormat, AudioPackFormatId>(object, "audioPackFormatIDRef");
      node.addReferences<AudioObject, AudioObjectId>(object, "audioObjectIDRef");
      for (auto &element : object->getComplementaryObjects()) {
        node.addElement<AudioObjectId>(element, "audioComplementaryObjectIDRef");
      }
      node.addReferences<AudioTrackUid, AudioTrackUidId>(object, "audioTrackUIDRef");
      node.addVectorElements<Labels>(object, "audioObjectLabel", &formatLabel);
      node.addVectorElements<AudioComplementaryObjectGroupLabels>(
          object, "audioComplementaryObjectGroupLabel",
          &formatAudioComplementaryObjectGroupLabel);
      node.addOptionalElement<Gain>(object, "gain", &formatGain);
      node.addOptionalElement<HeadLocked>(object, "headLocked");
      node.addOptionalMultiElement<PositionOffset>(object, "positionOffset", &formatPositionOffset);
      node.addOptionalElement<Mute>(object, "mute");
      // clang-format on
    }

    void formatAudioObjectInteraction(
        XmlNode &node, const AudioObjectInteraction &objectInteraction) {
      // clang-format off
      node.addAttribute<OnOffInteract>(&objectInteraction, "onOffInteract");
      node.addOptionalAttribute<GainInteract>(&objectInteraction, "gainInteract");
      node.addOptionalAttribute<PositionInteract>(&objectInteraction, "positionInteract");
      node.addOptionalMultiElement<GainInteractionRange>(&objectInteraction, "gainInteractionRange", &formatGainInteractionRange);
      node.addOptionalMultiElement<PositionInteractionRange>(&objectInteraction, "positionInteractionRange", &formatPositionInteractionRange);
      // clang-format on
    }

    void formatGainInteractionRange(
        XmlNode &parentNode, const std::string &name,
        const GainInteractionRange &gainInteraction) {
      if (gainInteraction.has<GainInteractionMin>()) {
        auto minNode = parentNode.addNode(name);
        minNode.addAttribute("bound", "min");
        formatGain(minNode, gainInteraction.get<GainInteractionMin>().get());
      }
      if (gainInteraction.has<GainInteractionMax>()) {
        auto maxNode = parentNode.addNode(name);
        maxNode.addAttribute("bound", "max");
        formatGain(maxNode, gainInteraction.get<GainInteractionMax>().get());
      }
    }

    void formatLabel(XmlNode &node, const Label &label) {
      if (label.has<LabelValue>()) {
        node.setValue(label.get<LabelValue>().get());
      }
      if (label.has<LabelLanguage>()) {
        node.addAttribute("language", label.get<LabelLanguage>().get());
      }
    }

    void formatAudioComplementaryObjectGroupLabel(
        XmlNode &node, const AudioComplementaryObjectGroupLabel &label) {
      formatLabel(node, label.get());
    }

    void formatPositionInteractionRange(
        XmlNode &parentNode, const std::string &name,
        const PositionInteractionRange &positionInteraction) {
      if (positionInteraction.has<AzimuthInteractionMin>()) {
        auto minNode = parentNode.addNode(name);
        minNode.addAttribute("coordinate", "azimuth");
        minNode.addAttribute("bound", "min");
        minNode.setValue(positionInteraction.get<AzimuthInteractionMin>());
      }
      if (positionInteraction.has<ElevationInteractionMin>()) {
        auto minNode = parentNode.addNode(name);
        minNode.addAttribute("coordinate", "elevation");
        minNode.addAttribute("bound", "min");
        minNode.setValue(positionInteraction.get<ElevationInteractionMin>());
      }
      if (positionInteraction.has<DistanceInteractionMin>()) {
        auto minNode = parentNode.addNode(name);
        minNode.addAttribute("coordinate", "distance");
        minNode.addAttribute("bound", "min");
        minNode.setValue(positionInteraction.get<DistanceInteractionMin>());
      }
      if (positionInteraction.has<AzimuthInteractionMax>()) {
        auto maxNode = parentNode.addNode(name);
        maxNode.addAttribute("coordinate", "azimuth");
        maxNode.addAttribute("bound", "max");
        maxNode.setValue(positionInteraction.get<AzimuthInteractionMax>());
      }
      if (positionInteraction.has<ElevationInteractionMax>()) {
        auto maxNode = parentNode.addNode(name);
        maxNode.addAttribute("coordinate", "elevation");
        maxNode.addAttribute("bound", "max");
        maxNode.setValue(positionInteraction.get<ElevationInteractionMax>());
      }
      if (positionInteraction.has<DistanceInteractionMax>()) {
        auto maxNode = parentNode.addNode(name);
        maxNode.addAttribute("coordinate", "distance");
        maxNode.addAttribute("bound", "max");
        maxNode.setValue(positionInteraction.get<DistanceInteractionMax>());
      }
      if (positionInteraction.has<XInteractionMin>()) {
        auto minNode = parentNode.addNode(name);
        minNode.addAttribute("coordinate", "X");
        minNode.addAttribute("bound", "min");
        minNode.setValue(positionInteraction.get<XInteractionMin>());
      }
      if (positionInteraction.has<YInteractionMin>()) {
        auto minNode = parentNode.addNode(name);
        minNode.addAttribute("coordinate", "Y");
        minNode.addAttribute("bound", "min");
        minNode.setValue(positionInteraction.get<YInteractionMin>());
      }
      if (positionInteraction.has<ZInteractionMin>()) {
        auto minNode = parentNode.addNode(name);
        minNode.addAttribute("coordinate", "Z");
        minNode.addAttribute("bound", "min");
        minNode.setValue(positionInteraction.get<ZInteractionMin>());
      }
      if (positionInteraction.has<XInteractionMax>()) {
        auto maxNode = parentNode.addNode(name);
        maxNode.addAttribute("coordinate", "X");
        maxNode.addAttribute("bound", "max");
        maxNode.setValue(positionInteraction.get<XInteractionMax>());
      }
      if (positionInteraction.has<YInteractionMax>()) {
        auto maxNode = parentNode.addNode(name);
        maxNode.addAttribute("coordinate", "Y");
        maxNode.addAttribute("bound", "max");
        maxNode.setValue(positionInteraction.get<YInteractionMax>());
      }
      if (positionInteraction.has<ZInteractionMax>()) {
        auto maxNode = parentNode.addNode(name);
        maxNode.addAttribute("coordinate", "Z");
        maxNode.addAttribute("bound", "max");
        maxNode.setValue(positionInteraction.get<ZInteractionMax>());
      }
    }

    void formatAudioPackFormat(
        XmlNode &node, std::shared_ptr<const AudioPackFormat> packFormat) {
      // clang-format off
      node.addAttribute<AudioPackFormatId>(packFormat, "audioPackFormatID");
      node.addOptionalAttribute<AudioPackFormatName>(packFormat, "audioPackFormatName");
      node.addOptionalAttribute<TypeDescriptor>(packFormat, "typeLabel", &formatTypeLabel);
      node.addOptionalAttribute<TypeDescriptor>(packFormat, "typeDefinition", &formatTypeDefinition);
      node.addOptionalAttribute<Importance>(packFormat, "importance");
      node.addReferences<AudioChannelFormat, AudioChannelFormatId>(packFormat, "audioChannelFormatIDRef");
      node.addReferences<AudioPackFormat, AudioPackFormatId>(packFormat, "audioPackFormatIDRef");
      // clang-format on
    }

    void formatAudioChannelFormat(
        XmlNode &node, std::shared_ptr<const AudioChannelFormat> channelFormat,
        TimeReference timeReference) {
      // clang-format off
      node.addAttribute<AudioChannelFormatId>(channelFormat, "audioChannelFormatID");
      node.addOptionalAttribute<AudioChannelFormatName>(channelFormat, "audioChannelFormatName");
      node.addOptionalAttribute<TypeDescriptor>(channelFormat, "typeLabel", &formatTypeLabel);
      node.addOptionalAttribute<TypeDescriptor>(channelFormat, "typeDefinition", &formatTypeDefinition);
      node.addOptionalMultiElement<Frequency>(channelFormat, "frequency", &formatFrequency);

      auto channelType = channelFormat->get<TypeDescriptor>();
      if (channelType == TypeDefinition::DIRECT_SPEAKERS) {
        node.addElements<AudioBlockFormatDirectSpeakers>(channelFormat, "audioBlockFormat", detail::wrapWithTimeRef(formatBlockFormatDirectSpeakers, timeReference));
      } else if (channelType == TypeDefinition::MATRIX) {
        node.addElements<AudioBlockFormatMatrix>(channelFormat, "audioBlockFormat", detail::wrapWithTimeRef(formatBlockFormatMatrix, timeReference));
      } else if (channelType == TypeDefinition::OBJECTS) {
        node.addElements<AudioBlockFormatObjects>(channelFormat, "audioBlockFormat", detail::wrapWithTimeRef(formatBlockFormatObjects, timeReference));
      } else if (channelType == TypeDefinition::HOA) {
        node.addElements<AudioBlockFormatHoa>(channelFormat, "audioBlockFormat", detail::wrapWithTimeRef(formatBlockFormatHoa, timeReference));
      } else if (channelType == TypeDefinition::BINAURAL) {
        node.addElements<AudioBlockFormatBinaural>(channelFormat, "audioBlockFormat", detail::wrapWithTimeRef(formatBlockFormatBinaural, timeReference));
      }
      // clang-format on
    }

    void formatBlockFormatDirectSpeakers(
        XmlNode &node, const AudioBlockFormatDirectSpeakers &audioBlock,
        TimeReference timeReference) {
      // clang-format off
      node.addAttribute<AudioBlockFormatId>(&audioBlock, "audioBlockFormatID");
      detail::addBlockTimeParameters(audioBlock, node, timeReference);
      node.addOptionalAttribute<InitializeBlock>(&audioBlock, "initializeBlock");
      node.addMultiElement<SpeakerLabels>(&audioBlock, "speakerLabel", &formatSpeakerLabels);
      if(audioBlock.has<SphericalSpeakerPosition>()) {
        node.addMultiElement<SphericalSpeakerPosition>(&audioBlock, "position", &formatSphericalSpeakerPosition);
      }
      if(audioBlock.has<CartesianSpeakerPosition>()) {
        node.addMultiElement<CartesianSpeakerPosition>(&audioBlock, "position", &formatCartesianSpeakerPosition);
      }

      node.addOptionalElement<HeadLocked>(&audioBlock, "headLocked");
      node.addOptionalElement<HeadphoneVirtualise>(&audioBlock, "headphoneVirtualise", &formatHeadphoneVirtualise);
      // clang-format on
      node.addOptionalElement<Gain>(&audioBlock, "gain", &formatGain);
      node.addOptionalElement<Importance>(&audioBlock, "importance");
    }

    void formatSpeakerLabels(XmlNode &parentNode, const std::string &name,
                             const SpeakerLabels &speakerLabels) {
      for (const auto &speakerLabel : speakerLabels) {
        auto speakerLabelNode = parentNode.addNode(name);
        speakerLabelNode.setValue(speakerLabel);
      }
    }

    void formatSphericalSpeakerPosition(
        XmlNode &parentNode, const std::string &name,
        const SphericalSpeakerPosition &position) {
      auto azimuthNode = parentNode.addNode(name);
      azimuthNode.addAttribute("coordinate", "azimuth");
      if (position.has<ScreenEdgeLock>()) {
        auto screenEdgeLock = position.get<ScreenEdgeLock>();
        azimuthNode.addOptionalAttribute<HorizontalEdge>(&screenEdgeLock,
                                                         "screenEdgeLock");
      }
      azimuthNode.setValue(position.get<Azimuth>());
      parentNode.addOptionalElement<AzimuthMin>(
          &position, name, [](XmlNode &node, AzimuthMin const &min) {
            node.addAttribute("coordinate", "azimuth");
            node.addAttribute("bound", "min");
            node.setValue(min.get());
          });
      parentNode.addOptionalElement<AzimuthMax>(
          &position, name, [](XmlNode &node, AzimuthMax const &max) {
            node.addAttribute("coordinate", "azimuth");
            node.addAttribute("bound", "max");
            node.setValue(max.get());
          });

      auto elevationNode = parentNode.addNode(name);
      elevationNode.addAttribute("coordinate", "elevation");
      if (position.has<ScreenEdgeLock>()) {
        auto screenEdgeLock = position.get<ScreenEdgeLock>();
        elevationNode.addOptionalAttribute<VerticalEdge>(&screenEdgeLock,
                                                         "screenEdgeLock");
      }
      elevationNode.setValue(position.get<Elevation>());

      parentNode.addOptionalElement<ElevationMin>(
          &position, name, [](XmlNode &node, ElevationMin const &min) {
            node.addAttribute("coordinate", "elevation");
            node.addAttribute("bound", "min");
            node.setValue(min.get());
          });
      parentNode.addOptionalElement<ElevationMax>(
          &position, name, [](XmlNode &node, ElevationMax const &max) {
            node.addAttribute("coordinate", "elevation");
            node.addAttribute("bound", "max");
            node.setValue(max.get());
          });

      parentNode.addOptionalElement<Distance>(
          &position, name, [](XmlNode &node, Distance const &distance) {
            node.addAttribute("coordinate", "distance");
            node.setValue(distance.get());
          });
      parentNode.addOptionalElement<DistanceMin>(
          &position, name, [](XmlNode &node, DistanceMin const &min) {
            node.addAttribute("coordinate", "distance");
            node.addAttribute("bound", "min");
            node.setValue(min.get());
          });
      parentNode.addOptionalElement<DistanceMax>(
          &position, name, [](XmlNode &node, DistanceMax const &max) {
            node.addAttribute("coordinate", "distance");
            node.addAttribute("bound", "max");
            node.setValue(max.get());
          });
    }

    void formatCartesianSpeakerPosition(
        XmlNode &parentNode, const std::string &name,
        const CartesianSpeakerPosition &position) {
      auto xNode = parentNode.addNode(name);
      xNode.addAttribute("coordinate", "X");
      if (position.has<ScreenEdgeLock>()) {
        auto screenEdgeLock = position.get<ScreenEdgeLock>();
        xNode.addOptionalAttribute<HorizontalEdge>(&screenEdgeLock,
                                                   "screenEdgeLock");
      }
      xNode.setValue(position.get<X>());

      parentNode.addOptionalElement<XMin>(
          &position, name, [](XmlNode &node, XMin const &min) {
            node.addAttribute("coordinate", "X");
            node.addAttribute("bound", "min");
            node.setValue(min.get());
          });
      parentNode.addOptionalElement<XMax>(
          &position, name, [](XmlNode &node, XMax const &max) {
            node.addAttribute("coordinate", "X");
            node.addAttribute("bound", "max");
            node.setValue(max.get());
          });

      auto yNode = parentNode.addNode(name);
      yNode.addAttribute("coordinate", "Y");
      if (position.has<ScreenEdgeLock>()) {
        auto screenEdgeLock = position.get<ScreenEdgeLock>();
        yNode.addOptionalAttribute<VerticalEdge>(&screenEdgeLock,
                                                 "screenEdgeLock");
      }
      yNode.setValue(position.get<Y>());

      parentNode.addOptionalElement<YMin>(
          &position, name, [](XmlNode &node, YMin const &min) {
            node.addAttribute("coordinate", "Y");
            node.addAttribute("bound", "min");
            node.setValue(min.get());
          });
      parentNode.addOptionalElement<YMax>(
          &position, name, [](XmlNode &node, YMax const &max) {
            node.addAttribute("coordinate", "Y");
            node.addAttribute("bound", "max");
            node.setValue(max.get());
          });

      parentNode.addOptionalElement<Z>(&position, name,
                                       [](XmlNode &node, Z const &z) {
                                         node.addAttribute("coordinate", "Z");
                                         node.setValue(z.get());
                                       });
      parentNode.addOptionalElement<ZMin>(
          &position, name, [](XmlNode &node, ZMin const &min) {
            node.addAttribute("coordinate", "Z");
            node.addAttribute("bound", "min");
            node.setValue(min.get());
          });
      parentNode.addOptionalElement<ZMax>(
          &position, name, [](XmlNode &node, ZMax const &max) {
            node.addAttribute("coordinate", "Z");
            node.addAttribute("bound", "max");
            node.setValue(max.get());
          });
    }

    void formatFrequency(XmlNode &parentNode, const std::string &name,
                         const Frequency &frequency) {
      if (frequency.has<LowPass>()) {
        auto lowPassNode = parentNode.addNode(name);
        lowPassNode.addAttribute("typeDefinition", "lowPass");
        lowPassNode.setValue(frequency.get<LowPass>());
      }
      if (frequency.has<HighPass>()) {
        auto highPassNode = parentNode.addNode(name);
        highPassNode.addAttribute("typeDefinition", "highPass");
        highPassNode.setValue(frequency.get<HighPass>());
      }
    }

    void formatBlockFormatMatrix(XmlNode &node,
                                 const AudioBlockFormatMatrix &audioBlock,
                                 TimeReference timeReference) {
      // clang-format off
      node.addAttribute<AudioBlockFormatId>(&audioBlock, "audioBlockFormatID");
      detail::addBlockTimeParameters(audioBlock, node, timeReference);
      node.addOptionalAttribute<InitializeBlock>(&audioBlock, "initializeBlock");
      // TODO: add missing matrix attributes and elements
      // clang-format on
    }

    void formatBlockFormatObjects(XmlNode &node,
                                  const AudioBlockFormatObjects &audioBlock,
                                  TimeReference timeReference) {
      // clang-format off
      node.addAttribute<AudioBlockFormatId>(&audioBlock, "audioBlockFormatID");
      detail::addBlockTimeParameters(audioBlock, node, timeReference);
      node.addOptionalAttribute<InitializeBlock>(&audioBlock, "initializeBlock");
      node.addMultiElement<Position>(&audioBlock, "position", &formatPosition);
      node.addOptionalElement<Width>(&audioBlock, "width");
      node.addOptionalElement<Height>(&audioBlock, "height");
      node.addOptionalElement<Depth>(&audioBlock, "depth");
      node.addOptionalElement<Cartesian>(&audioBlock, "cartesian");
      node.addOptionalElement<Gain>(&audioBlock, "gain", &formatGain);
      node.addOptionalElement<Diffuse>(&audioBlock, "diffuse");
      node.addOptionalElement<ChannelLock>(&audioBlock, "channelLock", &formatChannelLock);
      node.addOptionalElement<ObjectDivergence>(&audioBlock, "objectDivergence", &formatObjectDivergence);
      node.addOptionalElement<JumpPosition>(&audioBlock, "jumpPosition", &formatJumpPosition);
      node.addOptionalElement<ScreenRef>(&audioBlock, "screenRef");
      node.addOptionalElement<HeadLocked>(&audioBlock, "headLocked");
      node.addOptionalElement<HeadphoneVirtualise>(&audioBlock, "headphoneVirtualise", &formatHeadphoneVirtualise);
      // clang-format on
      // TODO: add zoneExclusion
      node.addOptionalElement<Importance>(&audioBlock, "importance");
    }

    void formatPosition(XmlNode &parentNode, const std::string &name,
                        const Position &position) {
      if (isSpherical(position)) {
        auto sphericalPosition = boost::get<SphericalPosition>(position);
        parentNode.addElement(
            sphericalPosition.get<Azimuth>(), name,
            detail::formatMultiElementAttribute("coordinate", "azimuth"));
        parentNode.addElement(
            sphericalPosition.get<Elevation>(), name,
            detail::formatMultiElementAttribute("coordinate", "elevation"));
        parentNode.addOptionalElement<Distance>(
            &sphericalPosition, name,
            detail::formatMultiElementAttribute("coordinate", "distance"));
      } else {
        auto cartesianPosition = boost::get<CartesianPosition>(position);
        parentNode.addElement(
            cartesianPosition.get<X>(), name,
            detail::formatMultiElementAttribute("coordinate", "X"));
        parentNode.addElement(
            cartesianPosition.get<Y>(), name,
            detail::formatMultiElementAttribute("coordinate", "Y"));
        parentNode.addOptionalElement<Z>(
            &cartesianPosition, name,
            detail::formatMultiElementAttribute("coordinate", "Z"));
      }
    }

    void formatGain(XmlNode &node, const Gain &gain) {
      if (gain.isLinear()) {
        node.setValue(gain.asLinear());
      } else {
        node.setValue(gain.asDb());
        node.addAttribute("gainUnit", "dB");
      }
    }

    void formatPositionOffset(XmlNode &parentNode, const std::string &name,
                              const PositionOffset &positionOffset) {
      if (isSpherical(positionOffset)) {
        auto sphericalPosition =
            boost::get<SphericalPositionOffset>(positionOffset);
        parentNode.addOptionalElement<AzimuthOffset>(
            &sphericalPosition, name,
            detail::formatMultiElementAttribute("coordinate", "azimuth"));
        parentNode.addOptionalElement<ElevationOffset>(
            &sphericalPosition, name,
            detail::formatMultiElementAttribute("coordinate", "elevation"));
        parentNode.addOptionalElement<DistanceOffset>(
            &sphericalPosition, name,
            detail::formatMultiElementAttribute("coordinate", "distance"));
      } else {
        auto cartesianPosition =
            boost::get<CartesianPositionOffset>(positionOffset);
        parentNode.addOptionalElement<XOffset>(
            &cartesianPosition, name,
            detail::formatMultiElementAttribute("coordinate", "X"));
        parentNode.addOptionalElement<YOffset>(
            &cartesianPosition, name,
            detail::formatMultiElementAttribute("coordinate", "Y"));
        parentNode.addOptionalElement<ZOffset>(
            &cartesianPosition, name,
            detail::formatMultiElementAttribute("coordinate", "Z"));
      }
    }

    void formatChannelLock(XmlNode &node, const ChannelLock &channelLock) {
      node.addOptionalAttribute<MaxDistance>(&channelLock, "maxDistance");
      node.setValue(channelLock.get<ChannelLockFlag>());
    }

    void formatObjectDivergence(XmlNode &node,
                                const ObjectDivergence &objectDivergence) {
      // clang-format off
      node.addOptionalAttribute<AzimuthRange>(&objectDivergence, "azimuthRange");
      node.addOptionalAttribute<PositionRange>(&objectDivergence, "positionRange");
      node.setValue(objectDivergence.get<Divergence>());
      // clang-format on
    }

    void formatJumpPosition(XmlNode &node, const JumpPosition &jumpPosition) {
      // clang-format off
      node.addOptionalAttribute<InterpolationLength>(&jumpPosition, "interpolationLength", &formatInterpolationLength);
      node.setValue(jumpPosition.get<JumpPositionFlag>());
      // clang-format on
    }

    void formatBlockFormatHoa(XmlNode &node,
                              const AudioBlockFormatHoa &audioBlock,
                              TimeReference timeReference) {
      // clang-format off
      node.addAttribute<AudioBlockFormatId>(&audioBlock, "audioBlockFormatID");
      detail::addBlockTimeParameters(audioBlock, node, timeReference);
      node.addOptionalAttribute<InitializeBlock>(&audioBlock, "initializeBlock");
      node.addElement<Order>(&audioBlock, "order");
      node.addElement<Degree>(&audioBlock, "degree");
      node.addOptionalElement<NfcRefDist>(&audioBlock, "nfcRefDist");
      node.addOptionalElement<ScreenRef>(&audioBlock, "screenRef");
      node.addOptionalElement<Normalization>(&audioBlock, "normalization");
      node.addOptionalElement<Equation>(&audioBlock, "equation");
      node.addOptionalElement<HeadLocked>(&audioBlock, "headLocked");
      node.addOptionalElement<HeadphoneVirtualise>(&audioBlock, "headphoneVirtualise", &formatHeadphoneVirtualise);
      // clang-format on
      node.addOptionalElement<Gain>(&audioBlock, "gain", &formatGain);
      node.addOptionalElement<Importance>(&audioBlock, "importance");
    }

    void formatBlockFormatBinaural(XmlNode &node,
                                   const AudioBlockFormatBinaural &audioBlock,
                                   TimeReference timeReference) {
      // clang-format off
      node.addAttribute<AudioBlockFormatId>(&audioBlock, "audioBlockFormatID");
      detail::addBlockTimeParameters(audioBlock, node, timeReference);
      node.addOptionalAttribute<InitializeBlock>(&audioBlock, "initializeBlock");
      // clang-format on
      node.addOptionalElement<Gain>(&audioBlock, "gain", &formatGain);
      node.addOptionalElement<Importance>(&audioBlock, "importance");
    }

    void formatAudioStreamFormat(
        XmlNode &node, std::shared_ptr<const AudioStreamFormat> streamFormat) {
      // clang-format off
      node.addAttribute<AudioStreamFormatId>(streamFormat, "audioStreamFormatID");
      node.addOptionalAttribute<AudioStreamFormatName>(streamFormat, "audioStreamFormatName");
      node.addOptionalAttribute<FormatDescriptor>(streamFormat, "formatLabel", &formatFormatLabel);
      node.addOptionalAttribute<FormatDescriptor>(streamFormat, "formatDefinition", &formatFormatDefinition);

      node.addOptionalReference<AudioChannelFormat, AudioChannelFormatId>(streamFormat, "audioChannelFormatIDRef");
      node.addOptionalReference<AudioPackFormat, AudioPackFormatId>(streamFormat, "audioPackFormatIDRef");
      node.addAudioTrackFormatReferences(streamFormat, "audioTrackFormatIDRef");
      // clang-format on
    }

    void formatAudioTrackFormat(
        XmlNode &node, std::shared_ptr<const AudioTrackFormat> trackFormat) {
      // clang-format off
      node.addAttribute<AudioTrackFormatId>(trackFormat, "audioTrackFormatID");
      node.addOptionalAttribute<AudioTrackFormatName>(trackFormat, "audioTrackFormatName");
      node.addOptionalAttribute<FormatDescriptor>(trackFormat, "formatLabel", &formatFormatLabel);
      node.addOptionalAttribute<FormatDescriptor>(trackFormat, "formatDefinition", &formatFormatDefinition);
      node.addOptionalReference<AudioStreamFormat, AudioStreamFormatId>(trackFormat, "audioStreamFormatIDRef");
      // clang-format on
    }

    void formatHeadphoneVirtualise(
        XmlNode &node, const HeadphoneVirtualise &headphoneVirtualise) {
      node.addOptionalAttribute<Bypass>(&headphoneVirtualise, "bypass");
      node.addOptionalAttribute<DirectToReverberantRatio>(&headphoneVirtualise,
                                                          "DRR");
    }

    void formatAudioTrackUid(XmlNode &node,
                             std::shared_ptr<const AudioTrackUid> trackUid) {
      node.addAttribute<AudioTrackUidId>(trackUid, "UID");
      node.addOptionalAttribute<SampleRate>(trackUid, "sampleRate");
      node.addOptionalAttribute<BitDepth>(trackUid, "bitDepth");
      node.addOptionalReference<AudioTrackFormat, AudioTrackFormatId>(
          trackUid, "audioTrackFormatIDRef");
      node.addOptionalReference<AudioChannelFormat, AudioChannelFormatId>(
          trackUid, "audioChannelFormatIDRef");
      node.addOptionalReference<AudioPackFormat, AudioPackFormatId>(
          trackUid, "audioPackFormatIDRef");
    }

    namespace {
      template <typename T>
      void formatIdRef(XmlNode &parent, const ChangedIds &ids) {
        for (auto const &ref : ids.get<T>()) {
          auto refNode =
              parent.addNode(detail::ChangedIdTraits<T>::elementName);
          refNode.setValue(
              formatId(ref.template get<
                       typename detail::ChangedIdTraits<T>::id_type>()));
          refNode.template addAttribute<ChangedIdStatus>(&ref, "status");
        }
      }
    }  // namespace

    void formatChangedIds(XmlNode &node, const ChangedIds &changedIds) {
      formatIdRef<ChangedAudioChannelFormatIds>(node, changedIds);
      formatIdRef<ChangedAudioPackFormatIds>(node, changedIds);
      formatIdRef<ChangedAudioTrackUidIds>(node, changedIds);
      formatIdRef<ChangedAudioTrackFormatIds>(node, changedIds);
      formatIdRef<ChangedAudioStreamFormatIds>(node, changedIds);
      formatIdRef<ChangedAudioObjectIds>(node, changedIds);
      formatIdRef<ChangedAudioContentIds>(node, changedIds);
      formatIdRef<ChangedAudioProgrammeIds>(node, changedIds);
    }

    void formatFrameFormat(XmlNode &node, const FrameFormat &format) {
      node.addAttribute<FrameFormatId>(&format, "frameFormatID");
      node.addAttribute<Start>(&format, "start");
      node.addAttribute<Duration>(&format, "duration");
      node.addAttribute<FrameType>(&format, "type");
      node.addOptionalAttribute<TimeReference>(&format, "timeReference");
      node.addOptionalAttribute<FlowId>(&format, "flowID");
      node.addOptionalAttribute<CountToFull>(&format, "countToFull");
      node.addOptionalAttribute<NumMetadataChunks>(&format,
                                                   "numMetadataChunks");
      node.addOptionalAttribute<CountToSameChunk>(&format, "countToSameChunk");
      node.addOptionalElement<ChangedIds>(&format, "changedIDs",
                                          &formatChangedIds);
    }

    void formatTransportTrackFormat(XmlNode &node,
                                    const TransportTrackFormat &format) {
      node.addAttribute<TransportId>(&format, "transportID");
      node.addOptionalAttribute<TransportName>(&format, "transportName");
      node.addOptionalAttribute<NumTracks>(&format, "numTracks");
      node.addOptionalAttribute<NumIds>(&format, "numIDs");
      for (const auto &audioTrack : format.get<AudioTracks>()) {
        auto trackNode = node.addNode("audioTrack");
        trackNode.addAttribute<TrackId>(&audioTrack, "trackID");
        trackNode.addOptionalAttribute<FormatDescriptor>(
            &audioTrack, "formatLabel", &formatFormatLabel);
        trackNode.addOptionalAttribute<FormatDescriptor>(
            &audioTrack, "formatDefinition", &formatFormatDefinition);
        for (const auto &uidId : audioTrack.get<AudioTrackUidRefs>()) {
          trackNode.addElement("audioTrackUIDRef", formatId(uidId));
        }
      }
    }

    void formatFrameHeader(XmlNode &node, const FrameHeader &header) {
      node.addElement<FrameFormat>(header.get<FrameFormat>(), "frameFormat",
                                   &formatFrameFormat);
      node.addOptionalElement<ProfileList>(&header, "profileList",
                                           &formatProfileList);
      for (const auto &transportTrack : header.get<TransportTrackFormats>()) {
        node.addElement(transportTrack, "transportTrackFormat",
                        &formatTransportTrackFormat);
      }
    }

    void formatProfileList(XmlNode &node, const ProfileList &profileList) {
      node.addVectorElements<Profiles>(&profileList, "profile", &formatProfile);
    }

    void formatProfile(XmlNode &node, const Profile &profile) {
      node.setValue(profile.get<ProfileValue>());
      node.addAttribute<ProfileName>(&profile, "profileName");
      node.addAttribute<ProfileVersion>(&profile, "profileVersion");
      node.addAttribute<ProfileLevel>(&profile, "profileLevel");
    }

    void formatTagList(XmlNode &node, const TagList &tagList) {
      node.addVectorElements<TagGroups>(&tagList, "tagGroup", &formatTagGroup);
    }

    void formatTagGroup(XmlNode &node, const TagGroup &tagGroup) {
      node.addVectorElements<TTags>(&tagGroup, "tag", &formatTag);
      node.addReferences<AudioProgramme, AudioProgrammeId>(&tagGroup, "audioProgrammeIDRef");
      node.addReferences<AudioContent, AudioContentId>(&tagGroup, "audioContentIDRef");
      node.addReferences<AudioObject, AudioObjectId>(&tagGroup, "audioObjectIDRef");
    }

    void formatTag(XmlNode &node, const TTag &tag) {
      node.setValue(tag.get<TTagValue>());
      node.addAttribute<TTagClass>(&tag, "class");
    }

  }  // namespace xml
}  // namespace adm
