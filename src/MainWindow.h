#ifndef NCURSES_MAIN_WINDOW_INCLUDED
#define NCURSES_MAIN_WINDOW_INCLUDED

#include "AutoMemory.h"

namespace ncurses
{

class MainView;
  
void status (const char* s); 
void status (const std::wstring& s); 
  
class MainWindow
{
public:

  MainWindow (); // add a debug mode which writes to 'window' debug area also need a log
  ~MainWindow () = default;

  MainWindow (const MainWindow&) = delete;
  MainWindow& operator= (const MainWindow&) = delete;

  MainWindow (MainWindow&&) = delete;
  MainWindow& operator= (MainWindow&&) = delete;

  shared_Window get ()
    noexcept;

  // Handle keyboard input, until the user inputs
  void runForever ();
  
  inline int maxY () const noexcept;
  inline int maxX () const noexcept;

  inline int startY () const noexcept;
  inline int startX () const noexcept;
  
private:
  
  shared_Window m_window;
  int           m_maxX;
  int           m_maxY;
  int           m_startX;
  int           m_startY;
  std::shared_ptr<MainView> m_mainView;
};


inline int MainWindow::maxY () const noexcept
{ return m_maxY; }

inline int MainWindow::maxX () const noexcept
{ return m_maxX; }
  
inline int MainWindow::startY () const noexcept
{ return m_startY; }

inline int MainWindow::startX () const noexcept
{ return m_startX; }
  
} // namespace ncurses

#endif // #define NCURSES_MAIN_WINDOW_INCLUDED
