#pragma once

#include <throwless/Error.hpp>

namespace ayan::cfg::err {

// mnemonically convenient names for errors:
using ConfigErr = tmn::err::AnyErr;
using ConfigParamNotFoundErr = tmn::err::AnyErr;
using InvalidConfigFileErr = tmn::err::AnyErr;
using ConfigParamConversionErr = tmn::err::AnyErr;

} // namespace ayan::cfg::err;
