#include "AutoMemory.h"

namespace ncurses
{

shared_Window make_shared_Window (WINDOW* win)
  noexcept
{
  return shared_Window (win, [] (WINDOW* w) { });
}

  
} // namespace ncurses
