#include "Console.h"
#include <iostream>

//#pragma warning(push)
//#pragma warning(disable: 4127) //conditional expression is constant
//#pragma warning(disable: 4702) //unreachable code
//#include <boost/iostreams/filtering_streambuf.hpp>
//#pragma warning(pop)

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <tchar.h>


namespace BWAPI
{
#if 0
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
#endif

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
    return false;
  }

  void autoOpenConsole(bool attach, bool alloc)
  {
    // putting cin in a filtering_ostreambuf makes it stop responding directly on Enter keypresses, and instead
    // you need to press Ctrl+C the same amount of times as the number of elements in the filter chain.
    /*
    static std::streambuf& orig_cout_buf(*std::cout.rdbuf());
    static std::streambuf& orig_cerr_buf(*std::cerr.rdbuf());
    static std::unique_ptr<io::filtering_ostreambuf> auto_cout_buf;
    static std::unique_ptr<io::filtering_ostreambuf> auto_cerr_buf;
    auto_cout_buf = std::make_unique<io::filtering_ostreambuf>(OpenConsoleFilter(attach, alloc) | boost::ref(orig_cout_buf));
    auto_cerr_buf = std::make_unique<io::filtering_ostreambuf>(OpenConsoleFilter(attach, alloc) | boost::ref(orig_cerr_buf));
    std::cout.rdbuf(auto_cout_buf.get());
    std::cerr.rdbuf(auto_cerr_buf.get());
    */
  }
}
