// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_FILEDATATABLE_H_
#define FLATBUFFERS_GENERATED_FILEDATATABLE_H_

#include "flatbuffers/flatbuffers.h"

struct PacketHeader;

struct FileDataDefinition;
struct FileDataDefinitionBuilder;

struct FileDataTable;
struct FileDataTableBuilder;

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) PacketHeader FLATBUFFERS_FINAL_CLASS {
 private:
  int32_t stream_id_;
  int32_t size_;

 public:
  PacketHeader()
      : stream_id_(0),
        size_(0) {
  }
  PacketHeader(int32_t _stream_id, int32_t _size)
      : stream_id_(flatbuffers::EndianScalar(_stream_id)),
        size_(flatbuffers::EndianScalar(_size)) {
  }
  int32_t stream_id() const {
    return flatbuffers::EndianScalar(stream_id_);
  }
  int32_t size() const {
    return flatbuffers::EndianScalar(size_);
  }
};
FLATBUFFERS_STRUCT_END(PacketHeader, 8);

struct FileDataDefinition FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef FileDataDefinitionBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_BYTE_OFFSET = 4,
    VT_PACKET_INFO = 6,
    VT_NUM_ELEMENTS = 8,
    VT_TIMESTAMP_START = 10,
    VT_TIMESTAMP_END = 12
  };
  int64_t byte_offset() const {
    return GetField<int64_t>(VT_BYTE_OFFSET, 0);
  }
  const PacketHeader *packet_info() const {
    return GetStruct<const PacketHeader *>(VT_PACKET_INFO);
  }
  int64_t num_elements() const {
    return GetField<int64_t>(VT_NUM_ELEMENTS, 0);
  }
  int64_t timestamp_start() const {
    return GetField<int64_t>(VT_TIMESTAMP_START, 0);
  }
  int64_t timestamp_end() const {
    return GetField<int64_t>(VT_TIMESTAMP_END, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int64_t>(verifier, VT_BYTE_OFFSET) &&
           VerifyField<PacketHeader>(verifier, VT_PACKET_INFO) &&
           VerifyField<int64_t>(verifier, VT_NUM_ELEMENTS) &&
           VerifyField<int64_t>(verifier, VT_TIMESTAMP_START) &&
           VerifyField<int64_t>(verifier, VT_TIMESTAMP_END) &&
           verifier.EndTable();
  }
};

struct FileDataDefinitionBuilder {
  typedef FileDataDefinition Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_byte_offset(int64_t byte_offset) {
    fbb_.AddElement<int64_t>(FileDataDefinition::VT_BYTE_OFFSET, byte_offset, 0);
  }
  void add_packet_info(const PacketHeader *packet_info) {
    fbb_.AddStruct(FileDataDefinition::VT_PACKET_INFO, packet_info);
  }
  void add_num_elements(int64_t num_elements) {
    fbb_.AddElement<int64_t>(FileDataDefinition::VT_NUM_ELEMENTS, num_elements, 0);
  }
  void add_timestamp_start(int64_t timestamp_start) {
    fbb_.AddElement<int64_t>(FileDataDefinition::VT_TIMESTAMP_START, timestamp_start, 0);
  }
  void add_timestamp_end(int64_t timestamp_end) {
    fbb_.AddElement<int64_t>(FileDataDefinition::VT_TIMESTAMP_END, timestamp_end, 0);
  }
  explicit FileDataDefinitionBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<FileDataDefinition> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<FileDataDefinition>(end);
    return o;
  }
};

inline flatbuffers::Offset<FileDataDefinition> CreateFileDataDefinition(
    flatbuffers::FlatBufferBuilder &_fbb,
    int64_t byte_offset = 0,
    const PacketHeader *packet_info = 0,
    int64_t num_elements = 0,
    int64_t timestamp_start = 0,
    int64_t timestamp_end = 0) {
  FileDataDefinitionBuilder builder_(_fbb);
  builder_.add_timestamp_end(timestamp_end);
  builder_.add_timestamp_start(timestamp_start);
  builder_.add_num_elements(num_elements);
  builder_.add_byte_offset(byte_offset);
  builder_.add_packet_info(packet_info);
  return builder_.Finish();
}

struct FileDataTable FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef FileDataTableBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TABLE = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<FileDataDefinition>> *table() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<FileDataDefinition>> *>(VT_TABLE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_TABLE) &&
           verifier.VerifyVector(table()) &&
           verifier.VerifyVectorOfTables(table()) &&
           verifier.EndTable();
  }
};

struct FileDataTableBuilder {
  typedef FileDataTable Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_table(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<FileDataDefinition>>> table) {
    fbb_.AddOffset(FileDataTable::VT_TABLE, table);
  }
  explicit FileDataTableBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<FileDataTable> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<FileDataTable>(end);
    return o;
  }
};

inline flatbuffers::Offset<FileDataTable> CreateFileDataTable(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<FileDataDefinition>>> table = 0) {
  FileDataTableBuilder builder_(_fbb);
  builder_.add_table(table);
  return builder_.Finish();
}

inline flatbuffers::Offset<FileDataTable> CreateFileDataTableDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<FileDataDefinition>> *table = nullptr) {
  auto table__ = table ? _fbb.CreateVector<flatbuffers::Offset<FileDataDefinition>>(*table) : 0;
  return CreateFileDataTable(
      _fbb,
      table__);
}

inline const FileDataTable *GetFileDataTable(const void *buf) {
  return flatbuffers::GetRoot<FileDataTable>(buf);
}

inline const FileDataTable *GetSizePrefixedFileDataTable(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<FileDataTable>(buf);
}

inline bool VerifyFileDataTableBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<FileDataTable>(nullptr);
}

inline bool VerifySizePrefixedFileDataTableBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<FileDataTable>(nullptr);
}

inline void FinishFileDataTableBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<FileDataTable> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedFileDataTableBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<FileDataTable> root) {
  fbb.FinishSizePrefixed(root);
}

#endif  // FLATBUFFERS_GENERATED_FILEDATATABLE_H_
