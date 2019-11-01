// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: init.proto

#include "init.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)

namespace protobuf_bwapierror_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_bwapierror_2eproto ::google::protobuf::internal::SCCInfo<0> scc_info_Error;
}  // namespace protobuf_bwapierror_2eproto
namespace bwapi {
namespace init {
class ClientBroadcastDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<ClientBroadcast>
      _instance;
} _ClientBroadcast_default_instance_;
class ServerResponseDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<ServerResponse>
      _instance;
} _ServerResponse_default_instance_;
class ClientResolutionDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<ClientResolution>
      _instance;
} _ClientResolution_default_instance_;
}  // namespace init
}  // namespace bwapi
namespace protobuf_init_2eproto {
static void InitDefaultsClientBroadcast() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::bwapi::init::_ClientBroadcast_default_instance_;
    new (ptr) ::bwapi::init::ClientBroadcast();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::bwapi::init::ClientBroadcast::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_ClientBroadcast =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsClientBroadcast}, {}};

static void InitDefaultsServerResponse() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::bwapi::init::_ServerResponse_default_instance_;
    new (ptr) ::bwapi::init::ServerResponse();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::bwapi::init::ServerResponse::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<1> scc_info_ServerResponse =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 1, InitDefaultsServerResponse}, {
      &protobuf_bwapierror_2eproto::scc_info_Error.base,}};

static void InitDefaultsClientResolution() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::bwapi::init::_ClientResolution_default_instance_;
    new (ptr) ::bwapi::init::ClientResolution();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::bwapi::init::ClientResolution::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<1> scc_info_ClientResolution =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 1, InitDefaultsClientResolution}, {
      &protobuf_bwapierror_2eproto::scc_info_Error.base,}};

void InitDefaults() {
  ::google::protobuf::internal::InitSCC(&scc_info_ClientBroadcast.base);
  ::google::protobuf::internal::InitSCC(&scc_info_ServerResponse.base);
  ::google::protobuf::internal::InitSCC(&scc_info_ClientResolution.base);
}

}  // namespace protobuf_init_2eproto
namespace bwapi {
namespace init {
bool Protocol_IsValid(int value) {
  switch (value) {
    case 0:
      return true;
    default:
      return false;
  }
}


// ===================================================================

void ClientBroadcast::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int ClientBroadcast::kApiVersionFieldNumber;
const int ClientBroadcast::kBwapiVersionFieldNumber;
const int ClientBroadcast::kTournamentFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

ClientBroadcast::ClientBroadcast()
  : ::google::protobuf::MessageLite(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_init_2eproto::scc_info_ClientBroadcast.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:bwapi.init.ClientBroadcast)
}
ClientBroadcast::ClientBroadcast(const ClientBroadcast& from)
  : ::google::protobuf::MessageLite(),
      _internal_metadata_(NULL) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  bwapiversion_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.bwapiversion().size() > 0) {
    bwapiversion_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.bwapiversion_);
  }
  ::memcpy(&apiversion_, &from.apiversion_,
    static_cast<size_t>(reinterpret_cast<char*>(&tournament_) -
    reinterpret_cast<char*>(&apiversion_)) + sizeof(tournament_));
  // @@protoc_insertion_point(copy_constructor:bwapi.init.ClientBroadcast)
}

void ClientBroadcast::SharedCtor() {
  bwapiversion_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&apiversion_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&tournament_) -
      reinterpret_cast<char*>(&apiversion_)) + sizeof(tournament_));
}

ClientBroadcast::~ClientBroadcast() {
  // @@protoc_insertion_point(destructor:bwapi.init.ClientBroadcast)
  SharedDtor();
}

void ClientBroadcast::SharedDtor() {
  bwapiversion_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void ClientBroadcast::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ClientBroadcast& ClientBroadcast::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_init_2eproto::scc_info_ClientBroadcast.base);
  return *internal_default_instance();
}


void ClientBroadcast::Clear() {
// @@protoc_insertion_point(message_clear_start:bwapi.init.ClientBroadcast)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  bwapiversion_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&apiversion_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&tournament_) -
      reinterpret_cast<char*>(&apiversion_)) + sizeof(tournament_));
  _internal_metadata_.Clear();
}

bool ClientBroadcast::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::LiteUnknownFieldSetter unknown_fields_setter(
      &_internal_metadata_);
  ::google::protobuf::io::StringOutputStream unknown_fields_output(
      unknown_fields_setter.buffer());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_output, false);
  // @@protoc_insertion_point(parse_start:bwapi.init.ClientBroadcast)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // uint32 apiVersion = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &apiversion_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // string bwapiVersion = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(18u /* 18 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_bwapiversion()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->bwapiversion().data(), static_cast<int>(this->bwapiversion().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "bwapi.init.ClientBroadcast.bwapiVersion"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // bool tournament = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(24u /* 24 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &tournament_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:bwapi.init.ClientBroadcast)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:bwapi.init.ClientBroadcast)
  return false;
#undef DO_
}

void ClientBroadcast::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:bwapi.init.ClientBroadcast)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint32 apiVersion = 1;
  if (this->apiversion() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->apiversion(), output);
  }

  // string bwapiVersion = 2;
  if (this->bwapiversion().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->bwapiversion().data(), static_cast<int>(this->bwapiversion().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "bwapi.init.ClientBroadcast.bwapiVersion");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->bwapiversion(), output);
  }

  // bool tournament = 3;
  if (this->tournament() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(3, this->tournament(), output);
  }

  output->WriteRaw((::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).data(),
                   static_cast<int>((::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).size()));
  // @@protoc_insertion_point(serialize_end:bwapi.init.ClientBroadcast)
}

size_t ClientBroadcast::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:bwapi.init.ClientBroadcast)
  size_t total_size = 0;

  total_size += (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).size();

  // string bwapiVersion = 2;
  if (this->bwapiversion().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->bwapiversion());
  }

  // uint32 apiVersion = 1;
  if (this->apiversion() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->apiversion());
  }

  // bool tournament = 3;
  if (this->tournament() != 0) {
    total_size += 1 + 1;
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void ClientBroadcast::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const ClientBroadcast*>(&from));
}

void ClientBroadcast::MergeFrom(const ClientBroadcast& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:bwapi.init.ClientBroadcast)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.bwapiversion().size() > 0) {

    bwapiversion_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.bwapiversion_);
  }
  if (from.apiversion() != 0) {
    set_apiversion(from.apiversion());
  }
  if (from.tournament() != 0) {
    set_tournament(from.tournament());
  }
}

void ClientBroadcast::CopyFrom(const ClientBroadcast& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:bwapi.init.ClientBroadcast)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ClientBroadcast::IsInitialized() const {
  return true;
}

void ClientBroadcast::Swap(ClientBroadcast* other) {
  if (other == this) return;
  InternalSwap(other);
}
void ClientBroadcast::InternalSwap(ClientBroadcast* other) {
  using std::swap;
  bwapiversion_.Swap(&other->bwapiversion_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(apiversion_, other->apiversion_);
  swap(tournament_, other->tournament_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::std::string ClientBroadcast::GetTypeName() const {
  return "bwapi.init.ClientBroadcast";
}


// ===================================================================

void ServerResponse::InitAsDefaultInstance() {
  ::bwapi::init::_ServerResponse_default_instance_._instance.get_mutable()->error_ = const_cast< ::bwapi::error::Error*>(
      ::bwapi::error::Error::internal_default_instance());
}
void ServerResponse::clear_error() {
  if (GetArenaNoVirtual() == NULL && error_ != NULL) {
    delete error_;
  }
  error_ = NULL;
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int ServerResponse::kApiVersionFieldNumber;
const int ServerResponse::kEngineTypeFieldNumber;
const int ServerResponse::kEngineVersionFieldNumber;
const int ServerResponse::kErrorFieldNumber;
const int ServerResponse::kSupportedProtocolsFieldNumber;
const int ServerResponse::kPortFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

ServerResponse::ServerResponse()
  : ::google::protobuf::MessageLite(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_init_2eproto::scc_info_ServerResponse.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:bwapi.init.ServerResponse)
}
ServerResponse::ServerResponse(const ServerResponse& from)
  : ::google::protobuf::MessageLite(),
      _internal_metadata_(NULL),
      supportedprotocols_(from.supportedprotocols_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  enginetype_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.enginetype().size() > 0) {
    enginetype_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.enginetype_);
  }
  engineversion_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.engineversion().size() > 0) {
    engineversion_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.engineversion_);
  }
  if (from.has_error()) {
    error_ = new ::bwapi::error::Error(*from.error_);
  } else {
    error_ = NULL;
  }
  ::memcpy(&apiversion_, &from.apiversion_,
    static_cast<size_t>(reinterpret_cast<char*>(&port_) -
    reinterpret_cast<char*>(&apiversion_)) + sizeof(port_));
  // @@protoc_insertion_point(copy_constructor:bwapi.init.ServerResponse)
}

void ServerResponse::SharedCtor() {
  enginetype_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  engineversion_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&error_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&port_) -
      reinterpret_cast<char*>(&error_)) + sizeof(port_));
}

ServerResponse::~ServerResponse() {
  // @@protoc_insertion_point(destructor:bwapi.init.ServerResponse)
  SharedDtor();
}

void ServerResponse::SharedDtor() {
  enginetype_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  engineversion_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (this != internal_default_instance()) delete error_;
}

void ServerResponse::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ServerResponse& ServerResponse::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_init_2eproto::scc_info_ServerResponse.base);
  return *internal_default_instance();
}


void ServerResponse::Clear() {
// @@protoc_insertion_point(message_clear_start:bwapi.init.ServerResponse)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  supportedprotocols_.Clear();
  enginetype_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  engineversion_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (GetArenaNoVirtual() == NULL && error_ != NULL) {
    delete error_;
  }
  error_ = NULL;
  ::memset(&apiversion_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&port_) -
      reinterpret_cast<char*>(&apiversion_)) + sizeof(port_));
  _internal_metadata_.Clear();
}

bool ServerResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::LiteUnknownFieldSetter unknown_fields_setter(
      &_internal_metadata_);
  ::google::protobuf::io::StringOutputStream unknown_fields_output(
      unknown_fields_setter.buffer());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_output, false);
  // @@protoc_insertion_point(parse_start:bwapi.init.ServerResponse)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // uint32 apiVersion = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &apiversion_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // string engineType = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(18u /* 18 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_enginetype()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->enginetype().data(), static_cast<int>(this->enginetype().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "bwapi.init.ServerResponse.engineType"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // string engineVersion = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(26u /* 26 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_engineversion()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->engineversion().data(), static_cast<int>(this->engineversion().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "bwapi.init.ServerResponse.engineVersion"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .bwapi.error.Error error = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(34u /* 34 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_error()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated .bwapi.init.Protocol supportedProtocols = 5;
      case 5: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(42u /* 42 & 0xFF */)) {
          ::google::protobuf::uint32 length;
          DO_(input->ReadVarint32(&length));
          ::google::protobuf::io::CodedInputStream::Limit limit = input->PushLimit(static_cast<int>(length));
          while (input->BytesUntilLimit() > 0) {
            int value;
            DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
            add_supportedprotocols(static_cast< ::bwapi::init::Protocol >(value));
          }
          input->PopLimit(limit);
        } else if (
            static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(40u /* 40 & 0xFF */)) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          add_supportedprotocols(static_cast< ::bwapi::init::Protocol >(value));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // uint32 port = 6;
      case 6: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(48u /* 48 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &port_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:bwapi.init.ServerResponse)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:bwapi.init.ServerResponse)
  return false;
#undef DO_
}

void ServerResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:bwapi.init.ServerResponse)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint32 apiVersion = 1;
  if (this->apiversion() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->apiversion(), output);
  }

  // string engineType = 2;
  if (this->enginetype().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->enginetype().data(), static_cast<int>(this->enginetype().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "bwapi.init.ServerResponse.engineType");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->enginetype(), output);
  }

  // string engineVersion = 3;
  if (this->engineversion().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->engineversion().data(), static_cast<int>(this->engineversion().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "bwapi.init.ServerResponse.engineVersion");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      3, this->engineversion(), output);
  }

  // .bwapi.error.Error error = 4;
  if (this->has_error()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      4, this->_internal_error(), output);
  }

  // repeated .bwapi.init.Protocol supportedProtocols = 5;
  if (this->supportedprotocols_size() > 0) {
    ::google::protobuf::internal::WireFormatLite::WriteTag(
      5,
      ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED,
      output);
    output->WriteVarint32(
        static_cast< ::google::protobuf::uint32>(_supportedprotocols_cached_byte_size_));
  }
  for (int i = 0, n = this->supportedprotocols_size(); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteEnumNoTag(
      this->supportedprotocols(i), output);
  }

  // uint32 port = 6;
  if (this->port() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(6, this->port(), output);
  }

  output->WriteRaw((::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).data(),
                   static_cast<int>((::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).size()));
  // @@protoc_insertion_point(serialize_end:bwapi.init.ServerResponse)
}

size_t ServerResponse::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:bwapi.init.ServerResponse)
  size_t total_size = 0;

  total_size += (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).size();

  // repeated .bwapi.init.Protocol supportedProtocols = 5;
  {
    size_t data_size = 0;
    unsigned int count = static_cast<unsigned int>(this->supportedprotocols_size());for (unsigned int i = 0; i < count; i++) {
      data_size += ::google::protobuf::internal::WireFormatLite::EnumSize(
        this->supportedprotocols(static_cast<int>(i)));
    }
    if (data_size > 0) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
            static_cast< ::google::protobuf::int32>(data_size));
    }
    int cached_size = ::google::protobuf::internal::ToCachedSize(data_size);
    GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
    _supportedprotocols_cached_byte_size_ = cached_size;
    GOOGLE_SAFE_CONCURRENT_WRITES_END();
    total_size += data_size;
  }

  // string engineType = 2;
  if (this->enginetype().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->enginetype());
  }

  // string engineVersion = 3;
  if (this->engineversion().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->engineversion());
  }

  // .bwapi.error.Error error = 4;
  if (this->has_error()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *error_);
  }

  // uint32 apiVersion = 1;
  if (this->apiversion() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->apiversion());
  }

  // uint32 port = 6;
  if (this->port() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->port());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void ServerResponse::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const ServerResponse*>(&from));
}

void ServerResponse::MergeFrom(const ServerResponse& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:bwapi.init.ServerResponse)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  supportedprotocols_.MergeFrom(from.supportedprotocols_);
  if (from.enginetype().size() > 0) {

    enginetype_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.enginetype_);
  }
  if (from.engineversion().size() > 0) {

    engineversion_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.engineversion_);
  }
  if (from.has_error()) {
    mutable_error()->::bwapi::error::Error::MergeFrom(from.error());
  }
  if (from.apiversion() != 0) {
    set_apiversion(from.apiversion());
  }
  if (from.port() != 0) {
    set_port(from.port());
  }
}

void ServerResponse::CopyFrom(const ServerResponse& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:bwapi.init.ServerResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ServerResponse::IsInitialized() const {
  return true;
}

void ServerResponse::Swap(ServerResponse* other) {
  if (other == this) return;
  InternalSwap(other);
}
void ServerResponse::InternalSwap(ServerResponse* other) {
  using std::swap;
  supportedprotocols_.InternalSwap(&other->supportedprotocols_);
  enginetype_.Swap(&other->enginetype_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  engineversion_.Swap(&other->engineversion_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(error_, other->error_);
  swap(apiversion_, other->apiversion_);
  swap(port_, other->port_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::std::string ServerResponse::GetTypeName() const {
  return "bwapi.init.ServerResponse";
}


// ===================================================================

void ClientResolution::InitAsDefaultInstance() {
  ::bwapi::init::_ClientResolution_default_instance_._instance.get_mutable()->result_ = const_cast< ::bwapi::error::Error*>(
      ::bwapi::error::Error::internal_default_instance());
}
void ClientResolution::clear_result() {
  if (GetArenaNoVirtual() == NULL && result_ != NULL) {
    delete result_;
  }
  result_ = NULL;
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int ClientResolution::kResultFieldNumber;
const int ClientResolution::kProtocolFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

ClientResolution::ClientResolution()
  : ::google::protobuf::MessageLite(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_init_2eproto::scc_info_ClientResolution.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:bwapi.init.ClientResolution)
}
ClientResolution::ClientResolution(const ClientResolution& from)
  : ::google::protobuf::MessageLite(),
      _internal_metadata_(NULL) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  if (from.has_result()) {
    result_ = new ::bwapi::error::Error(*from.result_);
  } else {
    result_ = NULL;
  }
  protocol_ = from.protocol_;
  // @@protoc_insertion_point(copy_constructor:bwapi.init.ClientResolution)
}

void ClientResolution::SharedCtor() {
  ::memset(&result_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&protocol_) -
      reinterpret_cast<char*>(&result_)) + sizeof(protocol_));
}

ClientResolution::~ClientResolution() {
  // @@protoc_insertion_point(destructor:bwapi.init.ClientResolution)
  SharedDtor();
}

void ClientResolution::SharedDtor() {
  if (this != internal_default_instance()) delete result_;
}

void ClientResolution::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ClientResolution& ClientResolution::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_init_2eproto::scc_info_ClientResolution.base);
  return *internal_default_instance();
}


void ClientResolution::Clear() {
// @@protoc_insertion_point(message_clear_start:bwapi.init.ClientResolution)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArenaNoVirtual() == NULL && result_ != NULL) {
    delete result_;
  }
  result_ = NULL;
  protocol_ = 0;
  _internal_metadata_.Clear();
}

bool ClientResolution::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::LiteUnknownFieldSetter unknown_fields_setter(
      &_internal_metadata_);
  ::google::protobuf::io::StringOutputStream unknown_fields_output(
      unknown_fields_setter.buffer());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_output, false);
  // @@protoc_insertion_point(parse_start:bwapi.init.ClientResolution)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // .bwapi.error.Error result = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_result()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .bwapi.init.Protocol protocol = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          set_protocol(static_cast< ::bwapi::init::Protocol >(value));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:bwapi.init.ClientResolution)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:bwapi.init.ClientResolution)
  return false;
#undef DO_
}

void ClientResolution::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:bwapi.init.ClientResolution)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .bwapi.error.Error result = 1;
  if (this->has_result()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      1, this->_internal_result(), output);
  }

  // .bwapi.init.Protocol protocol = 2;
  if (this->protocol() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      2, this->protocol(), output);
  }

  output->WriteRaw((::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).data(),
                   static_cast<int>((::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).size()));
  // @@protoc_insertion_point(serialize_end:bwapi.init.ClientResolution)
}

size_t ClientResolution::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:bwapi.init.ClientResolution)
  size_t total_size = 0;

  total_size += (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).size();

  // .bwapi.error.Error result = 1;
  if (this->has_result()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *result_);
  }

  // .bwapi.init.Protocol protocol = 2;
  if (this->protocol() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->protocol());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void ClientResolution::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const ClientResolution*>(&from));
}

void ClientResolution::MergeFrom(const ClientResolution& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:bwapi.init.ClientResolution)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.has_result()) {
    mutable_result()->::bwapi::error::Error::MergeFrom(from.result());
  }
  if (from.protocol() != 0) {
    set_protocol(from.protocol());
  }
}

void ClientResolution::CopyFrom(const ClientResolution& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:bwapi.init.ClientResolution)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ClientResolution::IsInitialized() const {
  return true;
}

void ClientResolution::Swap(ClientResolution* other) {
  if (other == this) return;
  InternalSwap(other);
}
void ClientResolution::InternalSwap(ClientResolution* other) {
  using std::swap;
  swap(result_, other->result_);
  swap(protocol_, other->protocol_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::std::string ClientResolution::GetTypeName() const {
  return "bwapi.init.ClientResolution";
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace init
}  // namespace bwapi
namespace google {
namespace protobuf {
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::bwapi::init::ClientBroadcast* Arena::CreateMaybeMessage< ::bwapi::init::ClientBroadcast >(Arena* arena) {
  return Arena::CreateInternal< ::bwapi::init::ClientBroadcast >(arena);
}
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::bwapi::init::ServerResponse* Arena::CreateMaybeMessage< ::bwapi::init::ServerResponse >(Arena* arena) {
  return Arena::CreateInternal< ::bwapi::init::ServerResponse >(arena);
}
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::bwapi::init::ClientResolution* Arena::CreateMaybeMessage< ::bwapi::init::ClientResolution >(Arena* arena) {
  return Arena::CreateInternal< ::bwapi::init::ClientResolution >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
