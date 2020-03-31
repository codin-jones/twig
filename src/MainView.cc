#include "MainView.h"

#include <sstream>

#include "MainWindow.h"

#include "spdlog/spdlog.h"

namespace ncurses
{

MainView::MainView (int startX, int startY, int maxX, int maxY)
  : m_startX      (startX),
    m_startY      (startY),
    m_maxX        (maxX),
    m_maxY        (maxY),
    m_curX        (m_startX),
    m_curY        (m_startY),
    m_nodes       (),
    m_activeNode  (),
    m_viewTopNode (),
    m_viewBotNode (),
    m_nodesPerView ((maxY/3)-1) // 3 lines per node and -1 because of head/root node 
{
  // load the view objects from the model data

  // HACK load bogus data for now
  for (int i=0; i < 60; ++i)
  {
    m_nodes.emplace_back (i);
    if (i == 0)
    {
      m_nodes.back().setHead ();
      m_nodes.back().setActive (true);
    }
  }

  m_nodes.back ().setRoot ();

  m_activeNode  = m_nodes.begin ();
  m_viewTopNode = m_nodes.begin ();
  if (m_nodes.size () > m_nodesPerView)
  {
    m_viewBotNode = m_nodes.begin ();
    std::advance (m_viewBotNode, m_nodesPerView);
  }
  else
  {
    m_viewBotNode = m_nodes.end ();
  }

  spdlog::info ("{}:{} m_nodesPerView={}", __FILE__, __LINE__, m_nodesPerView);
  
  // draw initial
  draw (m_viewTopNode);
}

void MainView::arrowDown ()
{
  Nodes::iterator newActive = m_activeNode;
  std::advance (newActive, 1);

  if (newActive == m_nodes.end ())
  {
    spdlog::info ("{}:{} at the very end", __FILE__, __LINE__);
    status ("At the very end");
  }
  else if (newActive == m_viewBotNode)
  {
    spdlog::info ("{}:{} bottom of view, draw next page", __FILE__, __LINE__);
    clearView ();

    m_activeNode->setActive (false);
    m_activeNode = newActive;
    
    m_activeNode->setActive (true);
    m_viewTopNode = m_activeNode;

    if (std::distance (m_viewTopNode, m_nodes.end ()) >= m_nodesPerView)
    {
      m_viewBotNode = m_viewTopNode;
      std::advance (m_viewBotNode, m_nodesPerView);
    }
    else
    {
      m_viewBotNode = m_nodes.end ();
    }
    
    draw (m_viewTopNode);
  }
  else
  {
    // arrow down, same view select next node
    m_activeNode->setActive (false);
    newActive->setActive (true);
    m_activeNode = newActive;
    draw (m_viewTopNode);
  } 
}

void MainView::arrowUp ()
{
  if (m_activeNode == m_viewTopNode && m_activeNode != m_nodes.begin ())
  {
    spdlog::info ("{}:{} top of view, draw prev page", __FILE__, __LINE__);
    clearView ();
    
    m_activeNode->setActive (false);
    m_viewBotNode = m_activeNode;
    
    std::advance (m_activeNode, -1);
    m_activeNode->setActive (true);

    if (std::distance (m_nodes.begin (), m_viewBotNode) >= m_nodesPerView)
    {
      m_viewTopNode = m_viewBotNode;
      std::advance (m_viewTopNode, (m_nodesPerView * -1));
    }
    else
    {
      m_viewTopNode = m_nodes.begin ();
    }
    
    draw (m_viewTopNode);
   }
  else if (m_activeNode == m_nodes.begin ())
  {
    spdlog::warn ("{}:{} at the very beginning", __FILE__, __LINE__);
    status ("At the very beginning");
  }
  else
  {
    // arrow up, same view select previous node
    m_activeNode->setActive (false);
    std::advance (m_activeNode, -1);
    m_activeNode->setActive (true);
    draw (m_viewTopNode);
  } 
}

void MainView::arrowRight ()
{
  status ("There are no commit nodes to the right");
}

void MainView::arrowLeft ()
{
  status ("There are no commit nodes to the left");
}

void MainView::selectActive ()
{
  // Find any curent selection
  auto oldSelected = std::find_if (m_nodes.begin (),
                                   m_nodes.end (),
                                   [] (const Nodes::value_type& v)
                                     {
                                       return v.isSelected ();
                                     });

  // if the user hits 's-elect' on the same node select/deselect it
  if (oldSelected == m_activeNode)
  {
    spdlog::info ("selected same as active");
    m_activeNode->setSelected (false);
  }
  // else they are different nodes and if not the end (aka nothing selected)
  // deselect old, select new
  else if (oldSelected != m_nodes.end ())
  {
    spdlog::info ("selected differs from active");
    oldSelected->setSelected (false);
    m_activeNode->setSelected (true);
  }
  else if (oldSelected == m_nodes.end ())
  {
    spdlog::info ("nothing selected, select active");
    m_activeNode->setSelected (true);
  }
  else
  {
    spdlog::info ("do nothing...because nothing selected");
  }
  draw (m_viewTopNode);
}

void MainView::diffSelectedAndActive ()
{
  // Find any curent selection
  auto selectedIter = std::find_if (m_nodes.begin (),
                                    m_nodes.end (),
                                    [] (const Nodes::value_type& v)
                                      {
                                        return v.isSelected ();
                                      });

  if (selectedIter == m_nodes.end ())
  {
    status ("No object selected to diff against active");
    return;
  }

  if (selectedIter == m_activeNode)
  {
    status ("Selected and active are the same, nothing to diff");
    return;
  }

  std::ostringstream strm;
  strm << "Diff selected=" << selectedIter->commitId() << " vs "
       << "active=" << m_activeNode->commitId ();
  status (strm.str ().c_str ());
}
  
void MainView::draw (NodeIter iter)
{
  spdlog::info ("{}:{} draw iter(top)={} bot={}", __FILE__ ,__LINE__ , iter->commitId(), m_viewBotNode->commitId());
  int xVal = m_curX;
  int yVal = m_curY;

  while (iter != m_viewBotNode)
  {
    if (! iter->isHead ())
    {
      mvprintw (yVal, xVal, "    |");
      ++yVal;
    }

    spdlog::info ("{}:{} yVal={} < m_maxY={}", __FILE__, __LINE__, yVal, m_maxY);
    if (iter->isActive () && iter->isSelected ())
    {
      mvprintw (yVal, xVal, ">[(%s)]", iter->commitId ().c_str ());
    }
    else if (iter->isActive () && ! iter->isSelected ())
    {
      mvprintw (yVal, xVal, "> (%s) ", iter->commitId ().c_str ());
    }
    else if (! iter->isActive () && iter->isSelected ())
    {
      mvprintw (yVal, xVal, " [ %s ]", iter->commitId ().c_str ());
    }
    else
    {
      mvprintw (yVal, xVal, "   %s  ", iter->commitId ().c_str ());
    }
    ++yVal;

    if (! iter->isRoot ())
    {
      mvprintw (yVal, xVal, "    |");
      ++yVal;
    }
    
    ++iter;
  }
}

void MainView::clearView ()
{
  for (int y=m_startY; y < m_maxY; ++y)
  {
    mvprintw (y, 0, "%*s", (m_maxX-1), "");
  }
}
  
} // namespace ncurses
