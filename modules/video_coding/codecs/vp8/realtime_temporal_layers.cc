/* Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license
*  that can be found in the LICENSE file in the root of the source
*  tree. An additional intellectual property rights grant can be found
*  in the file PATENTS.  All contributing project authors may
*  be found in the AUTHORS file in the root of the source tree.
*/

#include <stdlib.h>
#include <algorithm>

#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "modules/video_coding/codecs/interface/video_codec_interface.h"
#include "modules/video_coding/codecs/vp8/include/vp8_common_types.h"
#include "modules/video_coding/codecs/vp8/temporal_layers.h"

// This file implements logic to adapt the number of temporal layers based on
// input frame rate in order to avoid having the base layer being relaying at
// a below acceptable framerate.
namespace webrtc {
namespace {
enum {
  kTemporalUpdateLast = VP8_EFLAG_NO_UPD_GF | VP8_EFLAG_NO_UPD_ARF |
                        VP8_EFLAG_NO_REF_GF | VP8_EFLAG_NO_REF_ARF,

  kTemporalUpdateGolden =
      VP8_EFLAG_NO_REF_ARF | VP8_EFLAG_NO_UPD_ARF | VP8_EFLAG_NO_UPD_LAST,

  kTemporalUpdateGoldenWithoutDependency =
      kTemporalUpdateGolden | VP8_EFLAG_NO_REF_GF,

  kTemporalUpdateAltref = VP8_EFLAG_NO_UPD_GF | VP8_EFLAG_NO_UPD_LAST,

  kTemporalUpdateAltrefWithoutDependency =
      kTemporalUpdateAltref | VP8_EFLAG_NO_REF_ARF | VP8_EFLAG_NO_REF_GF,

  kTemporalUpdateNone = VP8_EFLAG_NO_UPD_GF | VP8_EFLAG_NO_UPD_ARF |
                        VP8_EFLAG_NO_UPD_LAST | VP8_EFLAG_NO_UPD_ENTROPY,

  kTemporalUpdateNoneNoRefAltref = kTemporalUpdateNone | VP8_EFLAG_NO_REF_ARF,

  kTemporalUpdateNoneNoRefGoldenRefAltRef =
      VP8_EFLAG_NO_REF_GF | VP8_EFLAG_NO_UPD_GF | VP8_EFLAG_NO_UPD_ARF |
      VP8_EFLAG_NO_UPD_LAST | VP8_EFLAG_NO_UPD_ENTROPY,

  kTemporalUpdateGoldenWithoutDependencyRefAltRef =
      VP8_EFLAG_NO_REF_GF | VP8_EFLAG_NO_UPD_ARF | VP8_EFLAG_NO_UPD_LAST,

  kTemporalUpdateLastRefAltRef =
      VP8_EFLAG_NO_UPD_GF | VP8_EFLAG_NO_UPD_ARF | VP8_EFLAG_NO_REF_GF,

  kTemporalUpdateGoldenRefAltRef = VP8_EFLAG_NO_UPD_ARF | VP8_EFLAG_NO_UPD_LAST,

  kTemporalUpdateLastAndGoldenRefAltRef =
      VP8_EFLAG_NO_UPD_ARF | VP8_EFLAG_NO_REF_GF,

  kTemporalUpdateLastRefAll = VP8_EFLAG_NO_UPD_ARF | VP8_EFLAG_NO_UPD_GF,
};

int CalculateNumberOfTemporalLayers(int current_temporal_layers,
                                    int input_fps) {
  if (input_fps >= 24) {
    return 3;
  }
  if (input_fps >= 20 && current_temporal_layers >= 3) {
    // Keep doing 3 temporal layers until we go below 20fps.
    return 3;
  }
  if (input_fps >= 10) {
    return 2;
  }
  if (input_fps > 8 && current_temporal_layers >= 2) {
    // keep doing 2 temporal layers until we go below 8fps
    return 2;
  }
  return 1;
}

class RealTimeTemporalLayers : public TemporalLayers {
 public:
  RealTimeTemporalLayers(int max_num_temporal_layers,
                         uint8_t initial_tl0_pic_idx)
      : temporal_layers_(1),
        max_temporal_layers_(max_num_temporal_layers),
        tl0_pic_idx_(initial_tl0_pic_idx),
        frame_counter_(static_cast<unsigned int>(-1)),
        timestamp_(0),
        last_base_layer_sync_(0),
        layer_ids_length_(0),
        layer_ids_(NULL),
        encode_flags_length_(0),
        encode_flags_(NULL) {
    assert(max_temporal_layers_ >= 1);
    assert(max_temporal_layers_ <= 3);
  }

  virtual ~RealTimeTemporalLayers() {}

  virtual bool ConfigureBitrates(int bitrate_kbit,
                                 int max_bitrate_kbit,
                                 int framerate,
                                 vpx_codec_enc_cfg_t* cfg) {
    temporal_layers_ =
        CalculateNumberOfTemporalLayers(temporal_layers_, framerate);
    temporal_layers_ = std::min(temporal_layers_, max_temporal_layers_);
    assert(temporal_layers_ >= 1 && temporal_layers_ <= 3);

    cfg->ts_number_layers = temporal_layers_;
    for (int tl = 0; tl < temporal_layers_; ++tl) {
      cfg->ts_target_bitrate[tl] =
          bitrate_kbit * kVp8LayerRateAlloction[temporal_layers_ - 1][tl];
    }

    switch (temporal_layers_) {
      case 1: {
        static const unsigned int layer_ids[] = {0u};
        layer_ids_ = layer_ids;
        layer_ids_length_ = sizeof(layer_ids) / sizeof(*layer_ids);

        static const int encode_flags[] = {kTemporalUpdateLastRefAll};
        encode_flags_length_ = sizeof(encode_flags) / sizeof(*layer_ids);
        encode_flags_ = encode_flags;

        cfg->ts_rate_decimator[0] = 1;
        cfg->ts_periodicity = layer_ids_length_;
      } break;

      case 2: {
        static const unsigned int layer_ids[] = {0u, 1u};
        layer_ids_ = layer_ids;
        layer_ids_length_ = sizeof(layer_ids) / sizeof(*layer_ids);

        static const int encode_flags[] = {
          kTemporalUpdateLastAndGoldenRefAltRef,
          kTemporalUpdateGoldenWithoutDependencyRefAltRef,
          kTemporalUpdateLastRefAltRef, kTemporalUpdateGoldenRefAltRef,
          kTemporalUpdateLastRefAltRef, kTemporalUpdateGoldenRefAltRef,
          kTemporalUpdateLastRefAltRef, kTemporalUpdateNone
        };
        encode_flags_length_ = sizeof(encode_flags) / sizeof(*layer_ids);
        encode_flags_ = encode_flags;

        cfg->ts_rate_decimator[0] = 2;
        cfg->ts_rate_decimator[1] = 1;
        cfg->ts_periodicity = layer_ids_length_;
      } break;

      case 3: {
        static const unsigned int layer_ids[] = {0u, 2u, 1u, 2u};
        layer_ids_ = layer_ids;
        layer_ids_length_ = sizeof(layer_ids) / sizeof(*layer_ids);

        static const int encode_flags[] = {
          kTemporalUpdateLastAndGoldenRefAltRef,
          kTemporalUpdateNoneNoRefGoldenRefAltRef,
          kTemporalUpdateGoldenWithoutDependencyRefAltRef, kTemporalUpdateNone,
          kTemporalUpdateLastRefAltRef, kTemporalUpdateNone,
          kTemporalUpdateGoldenRefAltRef, kTemporalUpdateNone
        };
        encode_flags_length_ = sizeof(encode_flags) / sizeof(*layer_ids);
        encode_flags_ = encode_flags;

        cfg->ts_rate_decimator[0] = 4;
        cfg->ts_rate_decimator[1] = 2;
        cfg->ts_rate_decimator[2] = 1;
        cfg->ts_periodicity = layer_ids_length_;
      } break;

      default:
        assert(false);
        return false;
    }
    memcpy(
        cfg->ts_layer_id, layer_ids_, sizeof(unsigned int) * layer_ids_length_);
    return true;
  }

  virtual int EncodeFlags(uint32_t timestamp) {
    frame_counter_++;
    return CurrentEncodeFlags();
  }

  int CurrentEncodeFlags() const {
    assert(encode_flags_length_ > 0 && encode_flags_ != NULL);
    int index = frame_counter_ % encode_flags_length_;
    assert(index >= 0 && index < encode_flags_length_);
    return encode_flags_[index];
  }

  virtual int CurrentLayerId() const {
    assert(layer_ids_length_ > 0 && layer_ids_ != NULL);
    int index = frame_counter_ % layer_ids_length_;
    assert(index >= 0 && index < layer_ids_length_);
    return layer_ids_[index];
  }

  virtual void PopulateCodecSpecific(bool base_layer_sync,
                                     CodecSpecificInfoVP8* vp8_info,
                                     uint32_t timestamp) {
    assert(temporal_layers_ > 0);

    if (temporal_layers_ == 1) {
      vp8_info->temporalIdx = kNoTemporalIdx;
      vp8_info->layerSync = false;
      vp8_info->tl0PicIdx = kNoTl0PicIdx;
    } else {
      if (base_layer_sync) {
        vp8_info->temporalIdx = 0;
        vp8_info->layerSync = true;
      } else {
        vp8_info->temporalIdx = CurrentLayerId();
        int temporal_reference = CurrentEncodeFlags();

        if (temporal_reference == kTemporalUpdateAltrefWithoutDependency ||
            temporal_reference == kTemporalUpdateGoldenWithoutDependency ||
            temporal_reference ==
                kTemporalUpdateGoldenWithoutDependencyRefAltRef ||
            temporal_reference == kTemporalUpdateNoneNoRefGoldenRefAltRef ||
            (temporal_reference == kTemporalUpdateNone &&
             temporal_layers_ == 4)) {
          vp8_info->layerSync = true;
        } else {
          vp8_info->layerSync = false;
        }
      }
      if (last_base_layer_sync_ && vp8_info->temporalIdx != 0) {
        // Regardless of pattern the frame after a base layer sync will always
        // be a layer sync.
        vp8_info->layerSync = true;
      }
      if (vp8_info->temporalIdx == 0 && timestamp != timestamp_) {
        timestamp_ = timestamp;
        tl0_pic_idx_++;
      }
      last_base_layer_sync_ = base_layer_sync;
      vp8_info->tl0PicIdx = tl0_pic_idx_;
    }
  }

  void FrameEncoded(unsigned int size, uint32_t timestamp) {}

 private:
  int temporal_layers_;
  int max_temporal_layers_;

  int tl0_pic_idx_;
  unsigned int frame_counter_;
  uint32_t timestamp_;
  bool last_base_layer_sync_;

  // Pattern of temporal layer ids.
  int layer_ids_length_;
  const unsigned int* layer_ids_;

  // Pattern of encode flags.
  int encode_flags_length_;
  const int* encode_flags_;
};
}  // namespace

TemporalLayers* RealTimeTemporalLayersFactory::Create(
    int max_temporal_layers,
    uint8_t initial_tl0_pic_idx) const {
  return new RealTimeTemporalLayers(max_temporal_layers, initial_tl0_pic_idx);
}
}  // namespace webrtc
