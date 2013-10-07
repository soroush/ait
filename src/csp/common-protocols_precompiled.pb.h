// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: common-protocols.proto

#ifndef PROTOBUF_common_2dprotocols_2eproto__INCLUDED
#define PROTOBUF_common_2dprotocols_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace AIT {
namespace protocols {
namespace csp {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_common_2dprotocols_2eproto();
void protobuf_AssignDesc_common_2dprotocols_2eproto();
void protobuf_ShutdownFile_common_2dprotocols_2eproto();

class P_Assignment;
class P_CompoundAssignment;

// ===================================================================

class P_Assignment : public ::google::protobuf::Message {
 public:
  P_Assignment();
  virtual ~P_Assignment();

  P_Assignment(const P_Assignment& from);

  inline P_Assignment& operator=(const P_Assignment& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const P_Assignment& default_instance();

  void Swap(P_Assignment* other);

  // implements Message ----------------------------------------------

  P_Assignment* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const P_Assignment& from);
  void MergeFrom(const P_Assignment& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // optional int32 value = 2;
  inline bool has_value() const;
  inline void clear_value();
  static const int kValueFieldNumber = 2;
  inline ::google::protobuf::int32 value() const;
  inline void set_value(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:AIT.protocols.csp.P_Assignment)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_value();
  inline void clear_has_value();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 id_;
  ::google::protobuf::int32 value_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_common_2dprotocols_2eproto();
  friend void protobuf_AssignDesc_common_2dprotocols_2eproto();
  friend void protobuf_ShutdownFile_common_2dprotocols_2eproto();

  void InitAsDefaultInstance();
  static P_Assignment* default_instance_;
};
// -------------------------------------------------------------------

class P_CompoundAssignment : public ::google::protobuf::Message {
 public:
  P_CompoundAssignment();
  virtual ~P_CompoundAssignment();

  P_CompoundAssignment(const P_CompoundAssignment& from);

  inline P_CompoundAssignment& operator=(const P_CompoundAssignment& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const P_CompoundAssignment& default_instance();

  void Swap(P_CompoundAssignment* other);

  // implements Message ----------------------------------------------

  P_CompoundAssignment* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const P_CompoundAssignment& from);
  void MergeFrom(const P_CompoundAssignment& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .AIT.protocols.csp.P_Assignment assignments = 1;
  inline int assignments_size() const;
  inline void clear_assignments();
  static const int kAssignmentsFieldNumber = 1;
  inline const ::AIT::protocols::csp::P_Assignment& assignments(int index) const;
  inline ::AIT::protocols::csp::P_Assignment* mutable_assignments(int index);
  inline ::AIT::protocols::csp::P_Assignment* add_assignments();
  inline const ::google::protobuf::RepeatedPtrField< ::AIT::protocols::csp::P_Assignment >&
      assignments() const;
  inline ::google::protobuf::RepeatedPtrField< ::AIT::protocols::csp::P_Assignment >*
      mutable_assignments();

  // @@protoc_insertion_point(class_scope:AIT.protocols.csp.P_CompoundAssignment)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::AIT::protocols::csp::P_Assignment > assignments_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_common_2dprotocols_2eproto();
  friend void protobuf_AssignDesc_common_2dprotocols_2eproto();
  friend void protobuf_ShutdownFile_common_2dprotocols_2eproto();

  void InitAsDefaultInstance();
  static P_CompoundAssignment* default_instance_;
};
// ===================================================================


// ===================================================================

// P_Assignment

// required int32 id = 1;
inline bool P_Assignment::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void P_Assignment::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void P_Assignment::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void P_Assignment::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 P_Assignment::id() const {
  return id_;
}
inline void P_Assignment::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
}

// optional int32 value = 2;
inline bool P_Assignment::has_value() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void P_Assignment::set_has_value() {
  _has_bits_[0] |= 0x00000002u;
}
inline void P_Assignment::clear_has_value() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void P_Assignment::clear_value() {
  value_ = 0;
  clear_has_value();
}
inline ::google::protobuf::int32 P_Assignment::value() const {
  return value_;
}
inline void P_Assignment::set_value(::google::protobuf::int32 value) {
  set_has_value();
  value_ = value;
}

// -------------------------------------------------------------------

// P_CompoundAssignment

// repeated .AIT.protocols.csp.P_Assignment assignments = 1;
inline int P_CompoundAssignment::assignments_size() const {
  return assignments_.size();
}
inline void P_CompoundAssignment::clear_assignments() {
  assignments_.Clear();
}
inline const ::AIT::protocols::csp::P_Assignment& P_CompoundAssignment::assignments(int index) const {
  return assignments_.Get(index);
}
inline ::AIT::protocols::csp::P_Assignment* P_CompoundAssignment::mutable_assignments(int index) {
  return assignments_.Mutable(index);
}
inline ::AIT::protocols::csp::P_Assignment* P_CompoundAssignment::add_assignments() {
  return assignments_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::AIT::protocols::csp::P_Assignment >&
P_CompoundAssignment::assignments() const {
  return assignments_;
}
inline ::google::protobuf::RepeatedPtrField< ::AIT::protocols::csp::P_Assignment >*
P_CompoundAssignment::mutable_assignments() {
  return &assignments_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace csp
}  // namespace protocols
}  // namespace AIT

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_common_2dprotocols_2eproto__INCLUDED
