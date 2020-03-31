#ifndef NCURSES_COMMIT_NODE_INCLUDED
#define NCURSES_COMMIT_NODE_INCLUDED

#include <string>

namespace ncurses
{

class CommitNode // maybe CommitNodeView that owns a CommitNode
{
public:

  CommitNode (int commitId);

  std::string commitId () const noexcept;

  bool isHead () const noexcept;
  bool isRoot () const noexcept;
  
  void setHead (); // aka top of the tree
  void setRoot (); // aka bottom of the tree

  bool isActive () const noexcept;
  bool isSelected () const noexcept;
  
  void setActive (bool b);
  void setSelected (bool b);

private:

  std::string m_commitId;
  std::string m_description;
  //std::string m_date;

  bool m_head;
  bool m_root;
  bool m_active;
  bool m_selected;
};

inline std::string CommitNode::commitId () const noexcept
{ return m_commitId; }

inline bool CommitNode::isHead () const noexcept
{ return m_head; }

inline bool CommitNode::isRoot () const noexcept
{ return m_root; }

inline bool CommitNode::isActive () const noexcept
{ return m_active; }

inline bool CommitNode::isSelected () const noexcept
{ return m_selected; }

} // namespace ncurses

#endif // #ifndef NCURSES_COMMIT_NODE_INCLUDED
