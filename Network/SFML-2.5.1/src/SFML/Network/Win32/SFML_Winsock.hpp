#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOGDICAPMASKS
#define NOGDICAPMASKS
#endif

#ifndef NOVIRTUALKEYCODES
#define NOVIRTUALKEYCODES
#endif

#ifndef NOWINMESSAGES
#define NOWINMESSAGES
#endif

#ifndef NOWINSTYLES
#define NOWINSTYLES
#endif

#ifndef NOSYSMETRICS
#define NOSYSMETRICS
#endif

#ifndef NOMENUS
#define NOMENUS
#endif

#ifndef NOICONS
#define NOICONS
#endif

#ifndef NOKEYSTATES
#define NOKEYSTATES
#endif

#ifndef NOSYSCOMMANDS
#define NOSYSCOMMANDS
#endif

#ifndef NORASTEROPS
#define NORASTEROPS
#endif

#ifndef NOSHOWWINDOW
#define NOSHOWWINDOW
#endif

#ifndef NOATOM
#define NOATOM
#endif

#ifndef NOCLIPBOARD
#define NOCLIPBOARD
#endif

#ifndef NOCOLOR
#define NOCOLOR
#endif

#ifndef NOCTLMGR
#define NOCTLMGR
#endif

#ifndef NODRAWTEXT
#define NODRAWTEXT
#endif

#ifndef NOGDI
#define NOGDI
#endif

#ifndef NOKERNEL
#define NOKERNEL
#endif

#ifndef NOUSER
#define NOUSER
#endif

#ifndef NONLS
#define NONLS
#endif

#ifndef NOMB
#define NOMB
#endif

#ifndef NOMEMMGR
#define NOMEMMGR
#endif

#ifndef NOMETAFILE
#define NOMETAFILE
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef NOMSG
#define NOMSG
#endif

#ifndef NOOPENFILE
#define NOOPENFILE
#endif

#ifndef NOSCROLL
#define NOSCROLL
#endif

#ifndef NOSERVICE
#define NOSERVICE
#endif

#ifndef NOSOUND
#define NOSOUND
#endif

#ifndef NOTEXTMETRIC
#define NOTEXTMETRIC
#endif

#ifndef NOWH
#define NOWH
#endif

#ifndef NOWINOFFSETS
#define NOWINOFFSETS
#endif

#ifndef NOCOMM
#define NOCOMM
#endif

#ifndef NOKANJI
#define NOKANJI
#endif

#ifndef NOHELP
#define NOHELP
#endif

#ifndef NOPROFILER
#define NOPROFILER
#endif

#ifndef NODEFERWINDOWPOS
#define NODEFERWINDOWPOS
#endif

#ifndef NOMCX
#define NOMCX
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

#ifdef FD_ISSET
#undef FD_ISSET
#endif

#define FD_ISSET(fd, set) __WSAFDIsSet((SOCKET)(fd), (fd_set FAR *)(set))
