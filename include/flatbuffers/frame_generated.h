// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_FRAME_H_
#define FLATBUFFERS_GENERATED_FRAME_H_

#include "flatbuffers/flatbuffers.h"

struct Frame;
struct FrameBuilder;

enum FrameFormat : int8_t {
  FrameFormat_Gray = 0,
  FrameFormat_Bgr = 16,
  FrameFormat_Bgra = 24,
  FrameFormat_MIN = FrameFormat_Gray,
  FrameFormat_MAX = FrameFormat_Bgra
};

inline const FrameFormat (&EnumValuesFrameFormat())[3] {
  static const FrameFormat values[] = {
    FrameFormat_Gray,
    FrameFormat_Bgr,
    FrameFormat_Bgra
  };
  return values;
}

inline const char *EnumNameFrameFormat(FrameFormat e) {
  switch (e) {
    case FrameFormat_Gray: return "Gray";
    case FrameFormat_Bgr: return "Bgr";
    case FrameFormat_Bgra: return "Bgra";
    default: return "";
  }
}

struct Frame FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef FrameBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_T = 4,
    VT_BEGIN_T = 6,
    VT_END_T = 8,
    VT_EXPOSURE_BEGIN_T = 10,
    VT_EXPOSURE_END_T = 12,
    VT_FORMAT = 14,
    VT_WIDTH = 16,
    VT_HEIGHT = 18,
    VT_OFFSET_X = 20,
    VT_OFFSET_Y = 22,
    VT_PIXELS = 24
  };
  int64_t t() const {
    return GetField<int64_t>(VT_T, 0);
  }
  int64_t begin_t() const {
    return GetField<int64_t>(VT_BEGIN_T, 0);
  }
  int64_t end_t() const {
    return GetField<int64_t>(VT_END_T, 0);
  }
  int64_t exposure_begin_t() const {
    return GetField<int64_t>(VT_EXPOSURE_BEGIN_T, 0);
  }
  int64_t exposure_end_t() const {
    return GetField<int64_t>(VT_EXPOSURE_END_T, 0);
  }
  FrameFormat format() const {
    return static_cast<FrameFormat>(GetField<int8_t>(VT_FORMAT, 0));
  }
  int16_t width() const {
    return GetField<int16_t>(VT_WIDTH, 0);
  }
  int16_t height() const {
    return GetField<int16_t>(VT_HEIGHT, 0);
  }
  int16_t offset_x() const {
    return GetField<int16_t>(VT_OFFSET_X, 0);
  }
  int16_t offset_y() const {
    return GetField<int16_t>(VT_OFFSET_Y, 0);
  }
  const flatbuffers::Vector<uint8_t> *pixels() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_PIXELS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int64_t>(verifier, VT_T, 8) &&
           VerifyField<int64_t>(verifier, VT_BEGIN_T, 8) &&
           VerifyField<int64_t>(verifier, VT_END_T, 8) &&
           VerifyField<int64_t>(verifier, VT_EXPOSURE_BEGIN_T, 8) &&
           VerifyField<int64_t>(verifier, VT_EXPOSURE_END_T, 8) &&
           VerifyField<int8_t>(verifier, VT_FORMAT, 1) &&
           VerifyField<int16_t>(verifier, VT_WIDTH, 2) &&
           VerifyField<int16_t>(verifier, VT_HEIGHT, 2) &&
           VerifyField<int16_t>(verifier, VT_OFFSET_X, 2) &&
           VerifyField<int16_t>(verifier, VT_OFFSET_Y, 2) &&
           VerifyOffset(verifier, VT_PIXELS) &&
           verifier.VerifyVector(pixels()) &&
           verifier.EndTable();
  }
};

struct FrameBuilder {
  typedef Frame Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_t(int64_t t) {
    fbb_.AddElement<int64_t>(Frame::VT_T, t, 0);
  }
  void add_begin_t(int64_t begin_t) {
    fbb_.AddElement<int64_t>(Frame::VT_BEGIN_T, begin_t, 0);
  }
  void add_end_t(int64_t end_t) {
    fbb_.AddElement<int64_t>(Frame::VT_END_T, end_t, 0);
  }
  void add_exposure_begin_t(int64_t exposure_begin_t) {
    fbb_.AddElement<int64_t>(Frame::VT_EXPOSURE_BEGIN_T, exposure_begin_t, 0);
  }
  void add_exposure_end_t(int64_t exposure_end_t) {
    fbb_.AddElement<int64_t>(Frame::VT_EXPOSURE_END_T, exposure_end_t, 0);
  }
  void add_format(FrameFormat format) {
    fbb_.AddElement<int8_t>(Frame::VT_FORMAT, static_cast<int8_t>(format), 0);
  }
  void add_width(int16_t width) {
    fbb_.AddElement<int16_t>(Frame::VT_WIDTH, width, 0);
  }
  void add_height(int16_t height) {
    fbb_.AddElement<int16_t>(Frame::VT_HEIGHT, height, 0);
  }
  void add_offset_x(int16_t offset_x) {
    fbb_.AddElement<int16_t>(Frame::VT_OFFSET_X, offset_x, 0);
  }
  void add_offset_y(int16_t offset_y) {
    fbb_.AddElement<int16_t>(Frame::VT_OFFSET_Y, offset_y, 0);
  }
  void add_pixels(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> pixels) {
    fbb_.AddOffset(Frame::VT_PIXELS, pixels);
  }
  explicit FrameBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<Frame> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Frame>(end);
    return o;
  }
};

inline flatbuffers::Offset<Frame> CreateFrame(
    flatbuffers::FlatBufferBuilder &_fbb,
    int64_t t = 0,
    int64_t begin_t = 0,
    int64_t end_t = 0,
    int64_t exposure_begin_t = 0,
    int64_t exposure_end_t = 0,
    FrameFormat format = FrameFormat_Gray,
    int16_t width = 0,
    int16_t height = 0,
    int16_t offset_x = 0,
    int16_t offset_y = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> pixels = 0) {
  FrameBuilder builder_(_fbb);
  builder_.add_exposure_end_t(exposure_end_t);
  builder_.add_exposure_begin_t(exposure_begin_t);
  builder_.add_end_t(end_t);
  builder_.add_begin_t(begin_t);
  builder_.add_t(t);
  builder_.add_pixels(pixels);
  builder_.add_offset_y(offset_y);
  builder_.add_offset_x(offset_x);
  builder_.add_height(height);
  builder_.add_width(width);
  builder_.add_format(format);
  return builder_.Finish();
}

inline flatbuffers::Offset<Frame> CreateFrameDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int64_t t = 0,
    int64_t begin_t = 0,
    int64_t end_t = 0,
    int64_t exposure_begin_t = 0,
    int64_t exposure_end_t = 0,
    FrameFormat format = FrameFormat_Gray,
    int16_t width = 0,
    int16_t height = 0,
    int16_t offset_x = 0,
    int16_t offset_y = 0,
    const std::vector<uint8_t> *pixels = nullptr) {
  auto pixels__ = pixels ? _fbb.CreateVector<uint8_t>(*pixels) : 0;
  return CreateFrame(
      _fbb,
      t,
      begin_t,
      end_t,
      exposure_begin_t,
      exposure_end_t,
      format,
      width,
      height,
      offset_x,
      offset_y,
      pixels__);
}

inline const Frame *GetFrame(const void *buf) {
  return flatbuffers::GetRoot<Frame>(buf);
}

inline const Frame *GetSizePrefixedFrame(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<Frame>(buf);
}

inline bool VerifyFrameBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Frame>(nullptr);
}

inline bool VerifySizePrefixedFrameBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<Frame>(nullptr);
}

inline void FinishFrameBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Frame> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedFrameBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Frame> root) {
  fbb.FinishSizePrefixed(root);
}

#endif  // FLATBUFFERS_GENERATED_FRAME_H_