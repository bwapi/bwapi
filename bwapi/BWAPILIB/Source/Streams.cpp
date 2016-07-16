#include <BWAPI/Streams.h>
#include <BWAPI/BroodwarOutputDevice.h>

#pragma warning(push)
#pragma warning(disable: 4702) //unreachable code
#include <boost/iostreams/stream.hpp>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 4512) //assignment operator could not be generated
#include <boost/iostreams/tee.hpp>
#pragma warning(pop)


namespace BWAPI
{
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
}
