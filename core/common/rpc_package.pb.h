// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: rpc_package.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_rpc_5fpackage_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_rpc_5fpackage_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3008000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3008000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_rpc_5fpackage_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_rpc_5fpackage_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_rpc_5fpackage_2eproto;
class RpcPackage;
class RpcPackageDefaultTypeInternal;
extern RpcPackageDefaultTypeInternal _RpcPackage_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::RpcPackage* Arena::CreateMaybeMessage<::RpcPackage>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class RpcPackage :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:RpcPackage) */ {
 public:
  RpcPackage();
  virtual ~RpcPackage();

  RpcPackage(const RpcPackage& from);
  RpcPackage(RpcPackage&& from) noexcept
    : RpcPackage() {
    *this = ::std::move(from);
  }

  inline RpcPackage& operator=(const RpcPackage& from) {
    CopyFrom(from);
    return *this;
  }
  inline RpcPackage& operator=(RpcPackage&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const RpcPackage& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const RpcPackage* internal_default_instance() {
    return reinterpret_cast<const RpcPackage*>(
               &_RpcPackage_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(RpcPackage* other);
  friend void swap(RpcPackage& a, RpcPackage& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline RpcPackage* New() const final {
    return CreateMaybeMessage<RpcPackage>(nullptr);
  }

  RpcPackage* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<RpcPackage>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const RpcPackage& from);
  void MergeFrom(const RpcPackage& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  #else
  bool MergePartialFromCodedStream(
      ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream* output) const final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(RpcPackage* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "RpcPackage";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_rpc_5fpackage_2eproto);
    return ::descriptor_table_rpc_5fpackage_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // bytes data = 2;
  void clear_data();
  static const int kDataFieldNumber = 2;
  const std::string& data() const;
  void set_data(const std::string& value);
  void set_data(std::string&& value);
  void set_data(const char* value);
  void set_data(const void* value, size_t size);
  std::string* mutable_data();
  std::string* release_data();
  void set_allocated_data(std::string* data);

  // int32 index = 1;
  void clear_index();
  static const int kIndexFieldNumber = 1;
  ::PROTOBUF_NAMESPACE_ID::int32 index() const;
  void set_index(::PROTOBUF_NAMESPACE_ID::int32 value);

  // uint32 callback_id = 3;
  void clear_callback_id();
  static const int kCallbackIdFieldNumber = 3;
  ::PROTOBUF_NAMESPACE_ID::uint32 callback_id() const;
  void set_callback_id(::PROTOBUF_NAMESPACE_ID::uint32 value);

  // @@protoc_insertion_point(class_scope:RpcPackage)
 private:
  class HasBitSetters;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr data_;
  ::PROTOBUF_NAMESPACE_ID::int32 index_;
  ::PROTOBUF_NAMESPACE_ID::uint32 callback_id_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_rpc_5fpackage_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// RpcPackage

// int32 index = 1;
inline void RpcPackage::clear_index() {
  index_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 RpcPackage::index() const {
  // @@protoc_insertion_point(field_get:RpcPackage.index)
  return index_;
}
inline void RpcPackage::set_index(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  index_ = value;
  // @@protoc_insertion_point(field_set:RpcPackage.index)
}

// bytes data = 2;
inline void RpcPackage::clear_data() {
  data_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& RpcPackage::data() const {
  // @@protoc_insertion_point(field_get:RpcPackage.data)
  return data_.GetNoArena();
}
inline void RpcPackage::set_data(const std::string& value) {
  
  data_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:RpcPackage.data)
}
inline void RpcPackage::set_data(std::string&& value) {
  
  data_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:RpcPackage.data)
}
inline void RpcPackage::set_data(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  data_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:RpcPackage.data)
}
inline void RpcPackage::set_data(const void* value, size_t size) {
  
  data_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:RpcPackage.data)
}
inline std::string* RpcPackage::mutable_data() {
  
  // @@protoc_insertion_point(field_mutable:RpcPackage.data)
  return data_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* RpcPackage::release_data() {
  // @@protoc_insertion_point(field_release:RpcPackage.data)
  
  return data_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void RpcPackage::set_allocated_data(std::string* data) {
  if (data != nullptr) {
    
  } else {
    
  }
  data_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), data);
  // @@protoc_insertion_point(field_set_allocated:RpcPackage.data)
}

// uint32 callback_id = 3;
inline void RpcPackage::clear_callback_id() {
  callback_id_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 RpcPackage::callback_id() const {
  // @@protoc_insertion_point(field_get:RpcPackage.callback_id)
  return callback_id_;
}
inline void RpcPackage::set_callback_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  callback_id_ = value;
  // @@protoc_insertion_point(field_set:RpcPackage.callback_id)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_rpc_5fpackage_2eproto
