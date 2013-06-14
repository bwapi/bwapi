#include "AbstractReplayReader.h"
#include "StrUtil.h"

START_REPLAY_TOOL

template<class TParam>
struct ParamReaderTraits {};

template<>
struct ParamReaderTraits<BYTE>
{
  static BYTE read(AbstractReplayReader& reader) { return reader.readBYTE(); }
};

template<>
struct ParamReaderTraits<WORD>
{
  static WORD read(AbstractReplayReader& reader) { return reader.readWORD(); }
};

template<>
struct ParamReaderTraits<DWORD>
{
  static DWORD read(AbstractReplayReader& reader) { return reader.readDWORD(); }
};

template<>
struct ParamReaderTraits<BWAPI::TechType>
{
  static BWAPI::TechType read(AbstractReplayReader& reader) { return reader.readTechType(); }
};

template<>
struct ParamReaderTraits<BWAPI::UpgradeType>
{
  static BWAPI::UpgradeType read(AbstractReplayReader& reader) { return reader.readUpgradeType(); }
};

template<>
struct ParamReaderTraits<BWAPI::UnitType>
{
  static BWAPI::UnitType read(AbstractReplayReader& reader) { return reader.readUnitType(); }
};

template<>
struct ParamReaderTraits<BWAPI::Order>
{
  static BWAPI::Order read(AbstractReplayReader& reader) { return reader.readOrder(); }
};

template<>
struct ParamReaderTraits<std::string>
{
  static std::string read(AbstractReplayReader& reader) { return reader.readCString(); }
};

template<class TParam>
struct ParamToStringTraits
{
  static std::string toString(const TParam& param) { return param.c_str(); }
};

template<>
struct ParamToStringTraits<DWORD>
{
  static std::string toString(const DWORD& param) { return StrUtil::format("%d", param); }
};

template<class TParam>
struct ParamTraits
{
  static std::string toString(const TParam& param) { return ParamToStringTraits<TParam>::toString(param); }
  static TParam read(AbstractReplayReader& reader) { return ParamReaderTraits<TParam>::read(reader); }
};

END_REPLAY_TOOL