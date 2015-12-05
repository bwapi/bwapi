#include <BWAPI/Streams.h>
#include <BWAPI/BroodwarOutputDevice.h>

#pragma warning(push)
#pragma warning(disable: 4127) //conditional expression is constant
#pragma warning(disable: 4512) //assignment operator could not be generated
#pragma warning(disable: 4702) //unreachable code
#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#pragma warning(pop)

//for openConsole()
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <tchar.h>


namespace BWAPI
{
  namespace io = boost::iostreams;

  class OpenConsoleFilter : public io::multichar_dual_use_filter
  {
    bool attach;
    bool alloc;
  public:
    OpenConsoleFilter(bool attach, bool alloc)
      : attach(attach)
      , alloc(alloc)
    {}

    template<typename Sink>
    std::streamsize write(Sink& snk, const char* s, std::streamsize n) const
    {
      openConsole(attach, alloc);
      return io::write(snk, s, n);
    }

    template<typename Source>
    std::streamsize read(Source& src, char* s, std::streamsize n) const
    {
      openConsole(attach, alloc);
      return io::read(src, s, n);
    }
  };
  BOOST_IOSTREAMS_PIPABLE(OpenConsoleFilter, 0)



  bool openConsole(bool attach, bool alloc)
  {
    if ((attach && AttachConsole(ATTACH_PARENT_PROCESS)) || (alloc && AllocConsole()))
    {
      *stdin  = *_tfdopen(_open_osfhandle(reinterpret_cast<intptr_t>(::GetStdHandle(STD_INPUT_HANDLE)), _O_RDONLY), _T("r"));
      *stdout = *_tfdopen(_open_osfhandle(reinterpret_cast<intptr_t>(::GetStdHandle(STD_OUTPUT_HANDLE)), _O_WRONLY), _T("a"));
      *stderr = *_tfdopen(_open_osfhandle(reinterpret_cast<intptr_t>(::GetStdHandle(STD_ERROR_HANDLE)), _O_WRONLY), _T("a"));

      //make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
      std::ios::sync_with_stdio(true);
      return true;
    }
    else
      return false;
  }

  void autoOpenConsole(bool attach, bool alloc)
  {
    // putting cin in a filtering_ostreambuf makes it stop responding directly on Enter keypresses, and instead
    // you need to press Ctrl+C the same amount of times as the number of elements in the filter chain.
    //static io::filtering_istreambuf auto_cin_buf(OpenConsoleFilter(attach, alloc) | boost::ref(*std::cin.rdbuf()));
    static io::filtering_ostreambuf auto_cout_buf(OpenConsoleFilter(attach, alloc) | boost::ref(*std::cout.rdbuf()));
    static io::filtering_ostreambuf auto_cerr_buf(OpenConsoleFilter(attach, alloc) | boost::ref(*std::cerr.rdbuf()));
    //std::cin.rdbuf(&auto_opening_cin_buf);
    std::cout.rdbuf(&auto_cout_buf);
    std::cerr.rdbuf(&auto_cerr_buf);
  }



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
