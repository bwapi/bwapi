#include <BWAPI/Streams.h>
#include <BWAPI/BroodwarOutputDevice.h>
#include <iostream>

#if 0
#pragma warning(push)
#pragma warning(disable: 4702) //unreachable code
#include <boost/iostreams/stream.hpp>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 4512) //assignment operator could not be generated
#include <boost/iostreams/tee.hpp>
#pragma warning(pop)
#endif // 0


namespace BWAPI
{
#if 0
  namespace io = boost::iostreams;
  using bwstream = io::stream<BroodwarOutputDevice>;
  using teestream = io::stream<io::tee_device<std::ostream, std::ostream>>;

  BroodwarOutputDevice bwout_device;
  BroodwarOutputDevice bwerr_device(Text::BrightRed);
  bwstream bwout_stream(bwout_device);
  bwstream bwerr_stream(bwerr_device);
  std::ostream& bwout = bwout_stream;
  std::ostream& bwerr = bwerr_stream;
  teestream out_stream(io::tee(std::cout, bwout));
  teestream err_stream(io::tee(std::cerr, bwerr));
  std::ostream& out = out_stream;
  std::ostream& err = err_stream;
#endif // 0

  std::ostream& bwout = std::cout;
  std::ostream& bwerr = std::cerr;
  std::ostream& out = std::cout;
  std::ostream& err = std::cerr;
}
