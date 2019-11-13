/******************************************************************************
*
* 
*
* Copyright (C) 1997-2013 by Dimitri van Heesch.
*
* Permission to use, copy, modify, and distribute this software and its
* documentation under the terms of the GNU General Public License is hereby 
* granted. No representations are made about the suitability of this software 
* for any purpose. It is provided "as is" without express or implied warranty.
* See the GNU General Public License for more details.
*
* Documents produced by Doxygen are derivative works derived from the
* input used in their production; they are not affected by this license.
*
*/

#ifndef REQUIREMENT_H
#define REQUIREMENT_H

#include <qcstring.h>
#include <qregexp.h>
#include <qvector.h>
#include "sortdict.h"
#include "entry.h"
#include "reflist.h"
#include "definition.h"
#include "definitionimpl.h"


class ClassDef;
class MemberName;

class RequirementMain : public DefinitionImpl
{
public:
  RequirementMain(const char *f,int l,const char *n,const char *d,const char *t);
  ~RequirementMain();

  // setters
  void setFileName(const char *name) {
    m_fileName = name;
    return;
  }

  // getters
  DefType definitionType() const { return TypeClass; }
  bool isLinkableInProject() const 
  { 
    return /*hasDocumentation() &&*/ !isReference();
  }
  bool isLinkable() const 
  {
    return isLinkableInProject() || isReference();
  } 

  // functions to get a uniform interface with Definitions
  QCString getOutputFileBase() const;
  QCString anchor() const { return QCString(); }
  bool showToc() const { return m_showToc; }
  void setPageScope(Definition *d){ m_pageScope = d; }
  Definition *getPageScope() const { return m_pageScope; }
  QCString displayName(bool=TRUE) const { return name(); }

  void writeDocumentation(OutputList &ol);

private:
  QCString m_fileName;
  QCString m_title;
  Definition *m_pageScope;
  bool m_showToc;
};

class RequirementSatisfied {
public:
  std::string reqID;
  RefItem* refItem;

  RequirementSatisfied(const std::string &a_reqID, RefItem &a_refItem) {
    reqID = a_reqID;
    refItem = &(a_refItem);
    return;
  }
};

class RequirementSatisfiedList : public std::vector<RequirementSatisfied*>
{
public:
  RequirementSatisfiedList() : std::vector<RequirementSatisfied*>() {}
  ~RequirementSatisfiedList() {}

  void f_addFromRefItem(RefItem &a_refItem);
};


class RequirementTested {
public:
  QCString reqID;
  RefItem* refItem;

  RequirementTested(const QCString &a_reqID, RefItem &a_refItem) {
    reqID = a_reqID.mid(0);
    refItem = &(a_refItem);
    return;
  }
};

class RequirementTestedList : public QList<RequirementTested>
{
public:
  RequirementTestedList() : QList() {}
  ~RequirementTestedList() {}

  void f_addFromRefItem(RefItem &a_refItem);
};

class Requirement : public DefinitionImpl
{
public:
  enum req_type {
    undef_type = 0,
    safety = 1,
    safety_critical = 2,
    use_case = 3,
    test_case = 4
  };

  QVector<Requirement> m_childsReq;
  Requirement* mp_parentReq;
  int m_nestingLevel;
  req_type m_req_type;

  std::string m_name__;

  //RequirementSatisfiedList m_reqSatList;
  bool m_outOfScope;

  Requirement(const char *f,int l,const char *n,const char *d,const char *t);
  ~Requirement();

  bool addChildReq(Requirement &a_req);

  // setters
  void setFileName(const char *name) {
    m_fileName = name;
    return;
  }

  // getters
  DefType definitionType() const { return TypeClass; }
  bool isLinkableInProject() const 
  { 
    return /*hasDocumentation() &&*/ !isReference();
  }
  bool isLinkable() const 
  {
    return isLinkableInProject() || isReference();
  } 

  // functions to get a uniform interface with Definitions
  QCString getOutputFileBase() const;
  QCString anchor() const { return QCString(); }
  bool showToc() const { return m_showToc; }
  void setPageScope(Definition *d){ m_pageScope = d; }
  Definition *getPageScope() const { return m_pageScope; }
  QCString displayName(bool=TRUE) const { return name(); }

  void writeDocumentation(OutputList &ol);
  void writeBriefDescription(OutputList &ol);
  void writeDetailedDescription(OutputList &ol,const QCString &title);

  void addListReferences();

private:
  QCString m_fileName;
  QCString m_title;
  Definition *m_pageScope;
  bool m_showToc;
};

/** A sorted dictionary of requirement objects. */
class RequirementSDict : public SDict<Requirement>
{
public:
  RequirementSDict(int size=17) : SDict<Requirement>(size) {}
  ~RequirementSDict() {}
  int RequirementSDict::compareItems(QCollection::Item item1,QCollection::Item item2);
};

class Dot_Graph : public DefinitionImpl
{
public:
  Dot_Graph(const char *f,int l,const char *n,const char *d,const char *t);
  ~Dot_Graph();

  // setters
  void setFileName(const char *name) {
    m_fileName = name;
    return;
  }

  // getters
  DefType definitionType() const { return TypeClass; }
  bool isLinkableInProject() const
  { 
    return /*hasDocumentation() &&*/ !isReference();
  }
  bool isLinkable() const
  {
    return isLinkableInProject() || isReference();
  } 

  // functions to get a uniform interface with Definitions
  QCString getOutputFileBase() const;
  QCString anchor() const { return QCString(); }
  bool showToc() const { return m_showToc; }
  void setPageScope(Definition *d){ m_pageScope = d; }
  Definition *getPageScope() const { return m_pageScope; }
  QCString displayName(bool=TRUE) const { return name(); }

  void writeDocumentation(OutputList &ol);
  void writeBriefDescription(OutputList &ol);
  void writeDetailedDescription(OutputList &ol,const QCString &title);

  void addListReferences();

private:
  QCString m_fileName;
  QCString m_title;
  Definition *m_pageScope;
  bool m_showToc;
};

/** A sorted dictionary of requirement_graph objects. */
class Dot_GraphSDict : public SDict<Dot_Graph>
{
public:
  Dot_GraphSDict(int size=17) : SDict<Dot_Graph>(size) {}
  ~Dot_GraphSDict() {}
  int Dot_GraphSDict::compareItems(QCollection::Item item1,QCollection::Item item2);
};
#endif
