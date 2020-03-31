#ifndef NCURSES_AUTO_MEMORY_INCLUDED
#define NCURSES_AUTO_MEMORY_INCLUDED

extern "C"
{
#include <ncursesw/ncurses.h>
}

#include <memory>

namespace ncurses
{

using shared_Window = std::shared_ptr<WINDOW>;

shared_Window make_shared_Window (WINDOW*)
  noexcept;


} // namespace ncurses

#endif // #define NCURSES_MAIN_WINDOW_INCLUDED
