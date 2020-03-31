#include "CommitNode.h"

#include <iomanip>
#include <sstream>

namespace ncurses
{

CommitNode::CommitNode (int commitId)
  : m_commitId    (),
    m_description (),
    m_head        (false),
    m_root        (false),
    m_active      (false),
    m_selected    (false)
{
  std::ostringstream strm;
  strm << std::setfill ('0') << std::setw (3) << commitId;
  m_commitId = strm.str ();

  std::ostringstream descStrm;
  descStrm << "Description for commit-id " << m_commitId;
  m_description = descStrm.str ();
}

void CommitNode::setHead ()
{
  m_head = true;
}

void CommitNode::setRoot ()
{
  m_root = true;
}

void CommitNode::setActive (bool b)
{
  m_active = b;
}

void CommitNode::setSelected (bool b)
{
  m_selected = b;
}
  
} // namespace ncurses
