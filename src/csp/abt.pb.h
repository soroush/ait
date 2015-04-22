// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: abt.proto

#ifndef PROTOBUF_abt_2eproto__INCLUDED
#define PROTOBUF_abt_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
#include "common-protocols.pb.h"
// @@protoc_insertion_point(includes)

namespace AIT {
namespace protocols {
namespace csp {
namespace abt {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_abt_2eproto();
void protobuf_AssignDesc_abt_2eproto();
void protobuf_ShutdownFile_abt_2eproto();

class P_EndPoint;
class P_CommunicationProtocol;
class P_Message;
class P_Nogood;

enum CP_MessageType {
  T_INTRODUCE = 1,
  T_INTRODUCE_ACK = 2,
  T_REQUEST_LIST = 3,
  T_REQUEST_ACK = 4,
  T_LIST = 5,
  ERR_NO_MORE_AGENTS = 6,
  ERR_REPEATED_ID = 7,
  ERR_UNKNOWN = 8
};
bool CP_MessageType_IsValid(int value);
const CP_MessageType CP_MessageType_MIN = T_INTRODUCE;
const CP_MessageType CP_MessageType_MAX = ERR_UNKNOWN;
const int CP_MessageType_ARRAYSIZE = CP_MessageType_MAX + 1;

const ::google::protobuf::EnumDescriptor* CP_MessageType_descriptor();
inline const ::std::string& CP_MessageType_Name(CP_MessageType value) {
  return ::google::protobuf::internal::NameOfEnum(
    CP_MessageType_descriptor(), value);
}
inline bool CP_MessageType_Parse(
    const ::std::string& name, CP_MessageType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<CP_MessageType>(
    CP_MessageType_descriptor(), name, value);
}
enum P_MessageType {
  T_OK = 1,
  T_NOGOOD = 2,
  T_ADDLINK = 3,
  T_STOP = 4,
  T_OK_MONITOR = 5,
  T_FINISH = 6
};
bool P_MessageType_IsValid(int value);
const P_MessageType P_MessageType_MIN = T_OK;
const P_MessageType P_MessageType_MAX = T_FINISH;
const int P_MessageType_ARRAYSIZE = P_MessageType_MAX + 1;

const ::google::protobuf::EnumDescriptor* P_MessageType_descriptor();
inline const ::std::string& P_MessageType_Name(P_MessageType value) {
  return ::google::protobuf::internal::NameOfEnum(
    P_MessageType_descriptor(), value);
}
inline bool P_MessageType_Parse(
    const ::std::string& name, P_MessageType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<P_MessageType>(
    P_MessageType_descriptor(), name, value);
}
// ===================================================================

class P_EndPoint : public ::google::protobuf::Message {
 public:
  P_EndPoint();
  virtual ~P_EndPoint();
  
  P_EndPoint(const P_EndPoint& from);
  
  inline P_EndPoint& operator=(const P_EndPoint& from) {
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
  static const P_EndPoint& default_instance();
  
  void Swap(P_EndPoint* other);
  
  // implements Message ----------------------------------------------
  
  P_EndPoint* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const P_EndPoint& from);
  void MergeFrom(const P_EndPoint& from);
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
  
  // required string name = 1;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 1;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  
  // optional int32 priority = 2;
  inline bool has_priority() const;
  inline void clear_priority();
  static const int kPriorityFieldNumber = 2;
  inline ::google::protobuf::int32 priority() const;
  inline void set_priority(::google::protobuf::int32 value);
  
  // required string host = 3;
  inline bool has_host() const;
  inline void clear_host();
  static const int kHostFieldNumber = 3;
  inline const ::std::string& host() const;
  inline void set_host(const ::std::string& value);
  inline void set_host(const char* value);
  inline void set_host(const char* value, size_t size);
  inline ::std::string* mutable_host();
  inline ::std::string* release_host();
  
  // required int32 port = 4;
  inline bool has_port() const;
  inline void clear_port();
  static const int kPortFieldNumber = 4;
  inline ::google::protobuf::int32 port() const;
  inline void set_port(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:AIT.protocols.csp.abt.P_EndPoint)
 private:
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_priority();
  inline void clear_has_priority();
  inline void set_has_host();
  inline void clear_has_host();
  inline void set_has_port();
  inline void clear_has_port();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* name_;
  ::std::string* host_;
  ::google::protobuf::int32 priority_;
  ::google::protobuf::int32 port_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_abt_2eproto();
  friend void protobuf_AssignDesc_abt_2eproto();
  friend void protobuf_ShutdownFile_abt_2eproto();
  
  void InitAsDefaultInstance();
  static P_EndPoint* default_instance_;
};
// -------------------------------------------------------------------

class P_CommunicationProtocol : public ::google::protobuf::Message {
 public:
  P_CommunicationProtocol();
  virtual ~P_CommunicationProtocol();
  
  P_CommunicationProtocol(const P_CommunicationProtocol& from);
  
  inline P_CommunicationProtocol& operator=(const P_CommunicationProtocol& from) {
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
  static const P_CommunicationProtocol& default_instance();
  
  void Swap(P_CommunicationProtocol* other);
  
  // implements Message ----------------------------------------------
  
  P_CommunicationProtocol* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const P_CommunicationProtocol& from);
  void MergeFrom(const P_CommunicationProtocol& from);
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
  
  // required .AIT.protocols.csp.abt.CP_MessageType type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline AIT::protocols::csp::abt::CP_MessageType type() const;
  inline void set_type(AIT::protocols::csp::abt::CP_MessageType value);
  
  // optional int32 priority = 2;
  inline bool has_priority() const;
  inline void clear_priority();
  static const int kPriorityFieldNumber = 2;
  inline ::google::protobuf::int32 priority() const;
  inline void set_priority(::google::protobuf::int32 value);
  
  // optional .AIT.protocols.csp.abt.P_EndPoint identity = 3;
  inline bool has_identity() const;
  inline void clear_identity();
  static const int kIdentityFieldNumber = 3;
  inline const ::AIT::protocols::csp::abt::P_EndPoint& identity() const;
  inline ::AIT::protocols::csp::abt::P_EndPoint* mutable_identity();
  inline ::AIT::protocols::csp::abt::P_EndPoint* release_identity();
  
  // repeated .AIT.protocols.csp.abt.P_EndPoint others = 4;
  inline int others_size() const;
  inline void clear_others();
  static const int kOthersFieldNumber = 4;
  inline const ::AIT::protocols::csp::abt::P_EndPoint& others(int index) const;
  inline ::AIT::protocols::csp::abt::P_EndPoint* mutable_others(int index);
  inline ::AIT::protocols::csp::abt::P_EndPoint* add_others();
  inline const ::google::protobuf::RepeatedPtrField< ::AIT::protocols::csp::abt::P_EndPoint >&
      others() const;
  inline ::google::protobuf::RepeatedPtrField< ::AIT::protocols::csp::abt::P_EndPoint >*
      mutable_others();
  
  // @@protoc_insertion_point(class_scope:AIT.protocols.csp.abt.P_CommunicationProtocol)
 private:
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_priority();
  inline void clear_has_priority();
  inline void set_has_identity();
  inline void clear_has_identity();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  int type_;
  ::google::protobuf::int32 priority_;
  ::AIT::protocols::csp::abt::P_EndPoint* identity_;
  ::google::protobuf::RepeatedPtrField< ::AIT::protocols::csp::abt::P_EndPoint > others_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_abt_2eproto();
  friend void protobuf_AssignDesc_abt_2eproto();
  friend void protobuf_ShutdownFile_abt_2eproto();
  
  void InitAsDefaultInstance();
  static P_CommunicationProtocol* default_instance_;
};
// -------------------------------------------------------------------

class P_Message : public ::google::protobuf::Message {
 public:
  P_Message();
  virtual ~P_Message();
  
  P_Message(const P_Message& from);
  
  inline P_Message& operator=(const P_Message& from) {
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
  static const P_Message& default_instance();
  
  void Swap(P_Message* other);
  
  // implements Message ----------------------------------------------
  
  P_Message* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const P_Message& from);
  void MergeFrom(const P_Message& from);
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
  
  // required .AIT.protocols.csp.abt.P_MessageType type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline AIT::protocols::csp::abt::P_MessageType type() const;
  inline void set_type(AIT::protocols::csp::abt::P_MessageType value);
  
  // required int32 sender = 2;
  inline bool has_sender() const;
  inline void clear_sender();
  static const int kSenderFieldNumber = 2;
  inline ::google::protobuf::int32 sender() const;
  inline void set_sender(::google::protobuf::int32 value);
  
  // optional .AIT.protocols.csp.P_Assignment assignment = 3;
  inline bool has_assignment() const;
  inline void clear_assignment();
  static const int kAssignmentFieldNumber = 3;
  inline const ::AIT::protocols::csp::P_Assignment& assignment() const;
  inline ::AIT::protocols::csp::P_Assignment* mutable_assignment();
  inline ::AIT::protocols::csp::P_Assignment* release_assignment();
  
  // optional .AIT.protocols.csp.P_CompoundAssignment nogood = 4;
  inline bool has_nogood() const;
  inline void clear_nogood();
  static const int kNogoodFieldNumber = 4;
  inline const ::AIT::protocols::csp::P_CompoundAssignment& nogood() const;
  inline ::AIT::protocols::csp::P_CompoundAssignment* mutable_nogood();
  inline ::AIT::protocols::csp::P_CompoundAssignment* release_nogood();
  
  // optional int32 sequence = 5;
  inline bool has_sequence() const;
  inline void clear_sequence();
  static const int kSequenceFieldNumber = 5;
  inline ::google::protobuf::int32 sequence() const;
  inline void set_sequence(::google::protobuf::int32 value);
  
  // optional bool approved = 6;
  inline bool has_approved() const;
  inline void clear_approved();
  static const int kApprovedFieldNumber = 6;
  inline bool approved() const;
  inline void set_approved(bool value);
  
  // @@protoc_insertion_point(class_scope:AIT.protocols.csp.abt.P_Message)
 private:
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_sender();
  inline void clear_has_sender();
  inline void set_has_assignment();
  inline void clear_has_assignment();
  inline void set_has_nogood();
  inline void clear_has_nogood();
  inline void set_has_sequence();
  inline void clear_has_sequence();
  inline void set_has_approved();
  inline void clear_has_approved();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  int type_;
  ::google::protobuf::int32 sender_;
  ::AIT::protocols::csp::P_Assignment* assignment_;
  ::AIT::protocols::csp::P_CompoundAssignment* nogood_;
  ::google::protobuf::int32 sequence_;
  bool approved_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(6 + 31) / 32];
  
  friend void  protobuf_AddDesc_abt_2eproto();
  friend void protobuf_AssignDesc_abt_2eproto();
  friend void protobuf_ShutdownFile_abt_2eproto();
  
  void InitAsDefaultInstance();
  static P_Message* default_instance_;
};
// -------------------------------------------------------------------

class P_Nogood : public ::google::protobuf::Message {
 public:
  P_Nogood();
  virtual ~P_Nogood();
  
  P_Nogood(const P_Nogood& from);
  
  inline P_Nogood& operator=(const P_Nogood& from) {
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
  static const P_Nogood& default_instance();
  
  void Swap(P_Nogood* other);
  
  // implements Message ----------------------------------------------
  
  P_Nogood* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const P_Nogood& from);
  void MergeFrom(const P_Nogood& from);
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
  
  // required .AIT.protocols.csp.P_CompoundAssignment lhs = 1;
  inline bool has_lhs() const;
  inline void clear_lhs();
  static const int kLhsFieldNumber = 1;
  inline const ::AIT::protocols::csp::P_CompoundAssignment& lhs() const;
  inline ::AIT::protocols::csp::P_CompoundAssignment* mutable_lhs();
  inline ::AIT::protocols::csp::P_CompoundAssignment* release_lhs();
  
  // required .AIT.protocols.csp.P_Assignment rhs = 2;
  inline bool has_rhs() const;
  inline void clear_rhs();
  static const int kRhsFieldNumber = 2;
  inline const ::AIT::protocols::csp::P_Assignment& rhs() const;
  inline ::AIT::protocols::csp::P_Assignment* mutable_rhs();
  inline ::AIT::protocols::csp::P_Assignment* release_rhs();
  
  // @@protoc_insertion_point(class_scope:AIT.protocols.csp.abt.P_Nogood)
 private:
  inline void set_has_lhs();
  inline void clear_has_lhs();
  inline void set_has_rhs();
  inline void clear_has_rhs();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::AIT::protocols::csp::P_CompoundAssignment* lhs_;
  ::AIT::protocols::csp::P_Assignment* rhs_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_abt_2eproto();
  friend void protobuf_AssignDesc_abt_2eproto();
  friend void protobuf_ShutdownFile_abt_2eproto();
  
  void InitAsDefaultInstance();
  static P_Nogood* default_instance_;
};
// ===================================================================


// ===================================================================

// P_EndPoint

// required string name = 1;
inline bool P_EndPoint::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void P_EndPoint::set_has_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void P_EndPoint::clear_has_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void P_EndPoint::clear_name() {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& P_EndPoint::name() const {
  return *name_;
}
inline void P_EndPoint::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void P_EndPoint::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void P_EndPoint::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* P_EndPoint::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  return name_;
}
inline ::std::string* P_EndPoint::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional int32 priority = 2;
inline bool P_EndPoint::has_priority() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void P_EndPoint::set_has_priority() {
  _has_bits_[0] |= 0x00000002u;
}
inline void P_EndPoint::clear_has_priority() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void P_EndPoint::clear_priority() {
  priority_ = 0;
  clear_has_priority();
}
inline ::google::protobuf::int32 P_EndPoint::priority() const {
  return priority_;
}
inline void P_EndPoint::set_priority(::google::protobuf::int32 value) {
  set_has_priority();
  priority_ = value;
}

// required string host = 3;
inline bool P_EndPoint::has_host() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void P_EndPoint::set_has_host() {
  _has_bits_[0] |= 0x00000004u;
}
inline void P_EndPoint::clear_has_host() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void P_EndPoint::clear_host() {
  if (host_ != &::google::protobuf::internal::kEmptyString) {
    host_->clear();
  }
  clear_has_host();
}
inline const ::std::string& P_EndPoint::host() const {
  return *host_;
}
inline void P_EndPoint::set_host(const ::std::string& value) {
  set_has_host();
  if (host_ == &::google::protobuf::internal::kEmptyString) {
    host_ = new ::std::string;
  }
  host_->assign(value);
}
inline void P_EndPoint::set_host(const char* value) {
  set_has_host();
  if (host_ == &::google::protobuf::internal::kEmptyString) {
    host_ = new ::std::string;
  }
  host_->assign(value);
}
inline void P_EndPoint::set_host(const char* value, size_t size) {
  set_has_host();
  if (host_ == &::google::protobuf::internal::kEmptyString) {
    host_ = new ::std::string;
  }
  host_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* P_EndPoint::mutable_host() {
  set_has_host();
  if (host_ == &::google::protobuf::internal::kEmptyString) {
    host_ = new ::std::string;
  }
  return host_;
}
inline ::std::string* P_EndPoint::release_host() {
  clear_has_host();
  if (host_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = host_;
    host_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required int32 port = 4;
inline bool P_EndPoint::has_port() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void P_EndPoint::set_has_port() {
  _has_bits_[0] |= 0x00000008u;
}
inline void P_EndPoint::clear_has_port() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void P_EndPoint::clear_port() {
  port_ = 0;
  clear_has_port();
}
inline ::google::protobuf::int32 P_EndPoint::port() const {
  return port_;
}
inline void P_EndPoint::set_port(::google::protobuf::int32 value) {
  set_has_port();
  port_ = value;
}

// -------------------------------------------------------------------

// P_CommunicationProtocol

// required .AIT.protocols.csp.abt.CP_MessageType type = 1;
inline bool P_CommunicationProtocol::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void P_CommunicationProtocol::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void P_CommunicationProtocol::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void P_CommunicationProtocol::clear_type() {
  type_ = 1;
  clear_has_type();
}
inline AIT::protocols::csp::abt::CP_MessageType P_CommunicationProtocol::type() const {
  return static_cast< AIT::protocols::csp::abt::CP_MessageType >(type_);
}
inline void P_CommunicationProtocol::set_type(AIT::protocols::csp::abt::CP_MessageType value) {
  GOOGLE_DCHECK(AIT::protocols::csp::abt::CP_MessageType_IsValid(value));
  set_has_type();
  type_ = value;
}

// optional int32 priority = 2;
inline bool P_CommunicationProtocol::has_priority() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void P_CommunicationProtocol::set_has_priority() {
  _has_bits_[0] |= 0x00000002u;
}
inline void P_CommunicationProtocol::clear_has_priority() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void P_CommunicationProtocol::clear_priority() {
  priority_ = 0;
  clear_has_priority();
}
inline ::google::protobuf::int32 P_CommunicationProtocol::priority() const {
  return priority_;
}
inline void P_CommunicationProtocol::set_priority(::google::protobuf::int32 value) {
  set_has_priority();
  priority_ = value;
}

// optional .AIT.protocols.csp.abt.P_EndPoint identity = 3;
inline bool P_CommunicationProtocol::has_identity() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void P_CommunicationProtocol::set_has_identity() {
  _has_bits_[0] |= 0x00000004u;
}
inline void P_CommunicationProtocol::clear_has_identity() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void P_CommunicationProtocol::clear_identity() {
  if (identity_ != NULL) identity_->::AIT::protocols::csp::abt::P_EndPoint::Clear();
  clear_has_identity();
}
inline const ::AIT::protocols::csp::abt::P_EndPoint& P_CommunicationProtocol::identity() const {
  return identity_ != NULL ? *identity_ : *default_instance_->identity_;
}
inline ::AIT::protocols::csp::abt::P_EndPoint* P_CommunicationProtocol::mutable_identity() {
  set_has_identity();
  if (identity_ == NULL) identity_ = new ::AIT::protocols::csp::abt::P_EndPoint;
  return identity_;
}
inline ::AIT::protocols::csp::abt::P_EndPoint* P_CommunicationProtocol::release_identity() {
  clear_has_identity();
  ::AIT::protocols::csp::abt::P_EndPoint* temp = identity_;
  identity_ = NULL;
  return temp;
}

// repeated .AIT.protocols.csp.abt.P_EndPoint others = 4;
inline int P_CommunicationProtocol::others_size() const {
  return others_.size();
}
inline void P_CommunicationProtocol::clear_others() {
  others_.Clear();
}
inline const ::AIT::protocols::csp::abt::P_EndPoint& P_CommunicationProtocol::others(int index) const {
  return others_.Get(index);
}
inline ::AIT::protocols::csp::abt::P_EndPoint* P_CommunicationProtocol::mutable_others(int index) {
  return others_.Mutable(index);
}
inline ::AIT::protocols::csp::abt::P_EndPoint* P_CommunicationProtocol::add_others() {
  return others_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::AIT::protocols::csp::abt::P_EndPoint >&
P_CommunicationProtocol::others() const {
  return others_;
}
inline ::google::protobuf::RepeatedPtrField< ::AIT::protocols::csp::abt::P_EndPoint >*
P_CommunicationProtocol::mutable_others() {
  return &others_;
}

// -------------------------------------------------------------------

// P_Message

// required .AIT.protocols.csp.abt.P_MessageType type = 1;
inline bool P_Message::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void P_Message::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void P_Message::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void P_Message::clear_type() {
  type_ = 1;
  clear_has_type();
}
inline AIT::protocols::csp::abt::P_MessageType P_Message::type() const {
  return static_cast< AIT::protocols::csp::abt::P_MessageType >(type_);
}
inline void P_Message::set_type(AIT::protocols::csp::abt::P_MessageType value) {
  GOOGLE_DCHECK(AIT::protocols::csp::abt::P_MessageType_IsValid(value));
  set_has_type();
  type_ = value;
}

// required int32 sender = 2;
inline bool P_Message::has_sender() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void P_Message::set_has_sender() {
  _has_bits_[0] |= 0x00000002u;
}
inline void P_Message::clear_has_sender() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void P_Message::clear_sender() {
  sender_ = 0;
  clear_has_sender();
}
inline ::google::protobuf::int32 P_Message::sender() const {
  return sender_;
}
inline void P_Message::set_sender(::google::protobuf::int32 value) {
  set_has_sender();
  sender_ = value;
}

// optional .AIT.protocols.csp.P_Assignment assignment = 3;
inline bool P_Message::has_assignment() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void P_Message::set_has_assignment() {
  _has_bits_[0] |= 0x00000004u;
}
inline void P_Message::clear_has_assignment() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void P_Message::clear_assignment() {
  if (assignment_ != NULL) assignment_->::AIT::protocols::csp::P_Assignment::Clear();
  clear_has_assignment();
}
inline const ::AIT::protocols::csp::P_Assignment& P_Message::assignment() const {
  return assignment_ != NULL ? *assignment_ : *default_instance_->assignment_;
}
inline ::AIT::protocols::csp::P_Assignment* P_Message::mutable_assignment() {
  set_has_assignment();
  if (assignment_ == NULL) assignment_ = new ::AIT::protocols::csp::P_Assignment;
  return assignment_;
}
inline ::AIT::protocols::csp::P_Assignment* P_Message::release_assignment() {
  clear_has_assignment();
  ::AIT::protocols::csp::P_Assignment* temp = assignment_;
  assignment_ = NULL;
  return temp;
}

// optional .AIT.protocols.csp.P_CompoundAssignment nogood = 4;
inline bool P_Message::has_nogood() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void P_Message::set_has_nogood() {
  _has_bits_[0] |= 0x00000008u;
}
inline void P_Message::clear_has_nogood() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void P_Message::clear_nogood() {
  if (nogood_ != NULL) nogood_->::AIT::protocols::csp::P_CompoundAssignment::Clear();
  clear_has_nogood();
}
inline const ::AIT::protocols::csp::P_CompoundAssignment& P_Message::nogood() const {
  return nogood_ != NULL ? *nogood_ : *default_instance_->nogood_;
}
inline ::AIT::protocols::csp::P_CompoundAssignment* P_Message::mutable_nogood() {
  set_has_nogood();
  if (nogood_ == NULL) nogood_ = new ::AIT::protocols::csp::P_CompoundAssignment;
  return nogood_;
}
inline ::AIT::protocols::csp::P_CompoundAssignment* P_Message::release_nogood() {
  clear_has_nogood();
  ::AIT::protocols::csp::P_CompoundAssignment* temp = nogood_;
  nogood_ = NULL;
  return temp;
}

// optional int32 sequence = 5;
inline bool P_Message::has_sequence() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void P_Message::set_has_sequence() {
  _has_bits_[0] |= 0x00000010u;
}
inline void P_Message::clear_has_sequence() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void P_Message::clear_sequence() {
  sequence_ = 0;
  clear_has_sequence();
}
inline ::google::protobuf::int32 P_Message::sequence() const {
  return sequence_;
}
inline void P_Message::set_sequence(::google::protobuf::int32 value) {
  set_has_sequence();
  sequence_ = value;
}

// optional bool approved = 6;
inline bool P_Message::has_approved() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void P_Message::set_has_approved() {
  _has_bits_[0] |= 0x00000020u;
}
inline void P_Message::clear_has_approved() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void P_Message::clear_approved() {
  approved_ = false;
  clear_has_approved();
}
inline bool P_Message::approved() const {
  return approved_;
}
inline void P_Message::set_approved(bool value) {
  set_has_approved();
  approved_ = value;
}

// -------------------------------------------------------------------

// P_Nogood

// required .AIT.protocols.csp.P_CompoundAssignment lhs = 1;
inline bool P_Nogood::has_lhs() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void P_Nogood::set_has_lhs() {
  _has_bits_[0] |= 0x00000001u;
}
inline void P_Nogood::clear_has_lhs() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void P_Nogood::clear_lhs() {
  if (lhs_ != NULL) lhs_->::AIT::protocols::csp::P_CompoundAssignment::Clear();
  clear_has_lhs();
}
inline const ::AIT::protocols::csp::P_CompoundAssignment& P_Nogood::lhs() const {
  return lhs_ != NULL ? *lhs_ : *default_instance_->lhs_;
}
inline ::AIT::protocols::csp::P_CompoundAssignment* P_Nogood::mutable_lhs() {
  set_has_lhs();
  if (lhs_ == NULL) lhs_ = new ::AIT::protocols::csp::P_CompoundAssignment;
  return lhs_;
}
inline ::AIT::protocols::csp::P_CompoundAssignment* P_Nogood::release_lhs() {
  clear_has_lhs();
  ::AIT::protocols::csp::P_CompoundAssignment* temp = lhs_;
  lhs_ = NULL;
  return temp;
}

// required .AIT.protocols.csp.P_Assignment rhs = 2;
inline bool P_Nogood::has_rhs() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void P_Nogood::set_has_rhs() {
  _has_bits_[0] |= 0x00000002u;
}
inline void P_Nogood::clear_has_rhs() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void P_Nogood::clear_rhs() {
  if (rhs_ != NULL) rhs_->::AIT::protocols::csp::P_Assignment::Clear();
  clear_has_rhs();
}
inline const ::AIT::protocols::csp::P_Assignment& P_Nogood::rhs() const {
  return rhs_ != NULL ? *rhs_ : *default_instance_->rhs_;
}
inline ::AIT::protocols::csp::P_Assignment* P_Nogood::mutable_rhs() {
  set_has_rhs();
  if (rhs_ == NULL) rhs_ = new ::AIT::protocols::csp::P_Assignment;
  return rhs_;
}
inline ::AIT::protocols::csp::P_Assignment* P_Nogood::release_rhs() {
  clear_has_rhs();
  ::AIT::protocols::csp::P_Assignment* temp = rhs_;
  rhs_ = NULL;
  return temp;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace abt
}  // namespace csp
}  // namespace protocols
}  // namespace AIT

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< AIT::protocols::csp::abt::CP_MessageType>() {
  return AIT::protocols::csp::abt::CP_MessageType_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< AIT::protocols::csp::abt::P_MessageType>() {
  return AIT::protocols::csp::abt::P_MessageType_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_abt_2eproto__INCLUDED