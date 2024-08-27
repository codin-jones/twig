#ifndef NCURSES_MAIN_VIEW_INCLUDED
#define NCURSES_MAIN_VIEW_INCLUDED

#include <vector>

#include "AutoMemory.h"
#include "CommitNode.h"

namespace ncurses
{

class MainView
{
public:

  MainView (int startX, int startY, int maxX, int maxY);

  void arrowDown ();
  void arrowUp ();
  void arrowRight ();
  void arrowLeft ();
  
  // page down
  // page up

  void selectActive ();
  void diffSelectedAndActive ();


private:

  using Nodes = std::vector<CommitNode>;
  using NodeIter = Nodes::iterator;

  void draw (NodeIter);
  void clearView ();
  
  int m_startX;
  int m_startY;
  int m_maxX;
  int m_maxY;
  int m_curX;
  int m_curY;


  Nodes m_nodes;
  NodeIter m_activeNode;
  NodeIter m_viewTopNode;
  NodeIter m_viewBotNode;
  uint16_t m_nodesPerView;
};

} // namepsace ncurses
#endif // #ifndef NCURSES_MAIN_VIEW_INCLUDED
