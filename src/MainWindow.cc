#include "MainWindow.h"

#include <sstream>
#include <stdexcept>

#include <chrono>
#include <thread>

// hack
#include <iostream>

#include "MainView.h"


namespace
{
const wchar_t* up_arrow    = L"\u25b2";
const wchar_t* down_arrow  = L"\u25bc";
const wchar_t* left_arrow  = L"\u25c0";
const wchar_t* right_arrow = L"\u25b6";

int l_maxX = 0;
}

namespace ncurses
{

void status (const char* s)
{
  if (s == nullptr)
    return;
  
  std::wstringstream strm;
  strm << s;
  status (strm.str ());
}
  
void status (const std::wstring& s)
{
  if (l_maxX > 0)
  {
    // Clear the 'status' line
    mvprintw (0, 0, "%*s", (l_maxX-1), "");
  }
  mvprintw (0, 0, "%ls", s.c_str ());
}
  
MainWindow::MainWindow ()
  : m_window   (initscr(), [] (WINDOW*) { endwin(); }),
    m_maxX     (0),
    m_maxY     (0),
    m_startX   (0),
    m_startY   (0),
    m_mainView ()
{
  if (! m_window)
  {
    throw std::runtime_error ("Null WINDOW returned from initscr");
  }

  if (! has_colors ())
  {
    throw std::runtime_error ("Your terminal does not support colors");
  }
  
  start_color ();
  use_default_colors();
  //
  // Initialize color pairs 1-255 with background as default (-1) and foreground as [0-254]
  //
  for (int i(0); i < 255; ++i)
  {
    init_pair (i+1, i, -1);
  }

  getmaxyx (m_window.get (), m_maxY, m_maxX);

  if (m_maxX < 80)
  {
    throw std::runtime_error ("Please make the window at least 80 columns wide.");
  }

  l_maxX = m_maxX;
  
  std::wostringstream strm;
  strm << "Use " << down_arrow << ' ' << up_arrow << ' '  << left_arrow << ' ' << right_arrow << " to navigate. S - Select, D - Diff, Q - quit";

  mvprintw (m_maxY-1, 0, "%ls", strm.str().c_str());

  // Leave a row on the top an bottom of the window so MaxY is really max-2
  // Start Y will be 1 also to accomodate the top row for 'menu'
  m_maxY  -= 2;
  m_startY = 1;

  m_mainView.reset (new MainView (m_startX, m_startY, m_maxX, m_maxY));
  
  // register we want to listen for keyboard input
  keypad (m_window.get (), TRUE);
}


void MainWindow::runForever ()
{
  bool done = false;
  do
  {
    // Get a single character
    int ch = wgetch (m_window.get ());
    
    // Clear the 'status' line
    mvprintw (0, 0, "%*s", (m_maxX-1), "");

    // update view
    
    
    switch (ch)
    {
    case KEY_DOWN:  m_mainView->arrowDown ();  break;
    case KEY_UP:    m_mainView->arrowUp ();    break;
    case KEY_RIGHT: m_mainView->arrowRight (); break;
    case KEY_LEFT:  m_mainView->arrowLeft ();  break;

    case 'd': // fall through
    case 'D':
    {
      m_mainView->diffSelectedAndActive (); break;
    }
    break;

    case 's': // fall through
    case 'S':
    {
      m_mainView->selectActive (); break;
    }
    break;
      
    case 'q': // fall through
    case 'Q':
    {
      status ("INFO: Quit key presssed");
      done = true;
    }
    break;
    
    default:
    {
      std::wostringstream strm;
      strm << "WARN: Unrecognized key char=" << (char)ch << " int=" << ch;
      status (strm.str ());
    }
    } // switch
  } while (! done);
}
  
} // namespace ncurses
