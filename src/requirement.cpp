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

#include "memberlist.h"
#include "classlist.h"
#include "filedef.h"
#include "doxygen.h"
#include "memberdef.h"
#include "classdef.h"
#include "namespacedef.h"
#include "util.h"
#include "language.h"
#include "outputlist.h"
#include "dot.h"
#include "message.h"
#include "docparser.h"
#include "searchindex.h"
#include "htags.h"
#include "parserintf.h"
#include "portable.h"
#include "vhdldocgen.h"
#include "debug.h"
#include "layout.h"
#include "groupdef.h"
#include "filename.h"
#include "membergroup.h"
#include "dirdef.h"
#include "config.h"
#include "definition.h"
#include "definitionimpl.h"
#include "index.h"
#include "requirement.h"

RequirementMain::RequirementMain(const char *f, int l, const char *n, const char *d, const char *t)
  : DefinitionImpl(f, l, 1, n), m_title(t)
{
  setDocumentation(d, f, l);
  m_pageScope = 0;
  m_showToc = FALSE;
}

RequirementMain::~RequirementMain()
{
}

QCString RequirementMain::getOutputFileBase() const
{
  return m_fileName;
}

void RequirementMain::writeDocumentation(OutputList &ol) {

  static bool generateTreeView = Config_getBool(GENERATE_TREEVIEW);

  startFile(ol, "requirements", 0, theTranslator->trRequirementsMain(), HLI_RequirementsMain);

  ol.startContents();

  //---------------------------------------- start flexible part -------------------------------

  SrcLangExt lang = getLanguage();
  QListIterator<LayoutDocEntry> eli(
    LayoutDocManager::instance().docEntries(LayoutDocManager::File));
  LayoutDocEntry *lde;
  for (eli.toFirst(); (lde = eli.current()); ++eli)
  {
    switch (lde->kind())
    {
    case LayoutDocEntry::BriefDesc:
    {
      if (!briefDescription().isEmpty() && Config_getBool(BRIEF_MEMBER_DESC))
      {
        ol.startParagraph();
        ol.generateDoc(briefFile(), briefLine(), this, 0, briefDescription(), TRUE, FALSE, 0, TRUE, FALSE);
        ol.pushGeneratorState();
        ol.disable(OutputGenerator::RTF);
        ol.writeString(" \n");
        ol.enable(OutputGenerator::RTF);

        if (Config_getBool(REPEAT_BRIEF) ||
          !documentation().isEmpty()
          )
        {
          ol.disableAllBut(OutputGenerator::Html);
          ol.startTextLink(0, "details");
          ol.parseText(theTranslator->trMore());
          ol.endTextLink();
        }
        ol.popGeneratorState();
        ol.endParagraph();

      }
      ol.writeSynopsis();
      break;
    }
    case LayoutDocEntry::MemberDeclStart:
      //        startMemberDeclarations(ol);
      break;
    case LayoutDocEntry::FileIncludes:
      //        writeIncludeFiles(ol);
      break;
    case LayoutDocEntry::FileIncludeGraph:
      //        writeIncludeGraph(ol);
      break;
    case LayoutDocEntry::FileIncludedByGraph:
      //        writeIncludedByGraph(ol);
      break;
    case LayoutDocEntry::FileSourceLink:
      //        writeSourceLink(ol);
      break;
    case LayoutDocEntry::FileClasses:
    {
      //          LayoutDocEntrySection *ls = (LayoutDocEntrySection*)lde;
      //          writeClassDeclarations(ol,ls->title(lang));
    }
      break;
    case LayoutDocEntry::FileNamespaces:
    {
      //          LayoutDocEntrySection *ls = (LayoutDocEntrySection*)lde;
      //          writeNamespaceDeclarations(ol,ls->title(lang));
    }
      break;
    case LayoutDocEntry::MemberGroups:
      //        writeMemberGroups(ol);
      break;
    case LayoutDocEntry::MemberDecl:
    {
      //          LayoutDocEntryMemberDecl *lmd = (LayoutDocEntryMemberDecl*)lde;
      //          writeMemberDeclarations(ol,lmd->type,lmd->title(lang));
    }
      break;
    case LayoutDocEntry::MemberDeclEnd:
      //        endMemberDeclarations(ol);
      break;
    case LayoutDocEntry::DetailedDesc:
    {
      LayoutDocEntrySection *ls = (LayoutDocEntrySection*)lde;
      if ((!briefDescription().isEmpty() && Config_getBool(REPEAT_BRIEF)) || !documentation().isEmpty())
      {
        ol.pushGeneratorState();
        ol.disable(OutputGenerator::Html);
        ol.writeRuler();
        ol.popGeneratorState();
        ol.pushGeneratorState();
        ol.disableAllBut(OutputGenerator::Html);
        ol.writeAnchor(0, "details");
        ol.popGeneratorState();
        //dont want to created a detailed description section
        //          ol.startGroupHeader();
        //          ol.parseText(ls->title(lang));
        //          ol.endGroupHeader();

        ol.startTextBlock();
        /*
        if (!briefDescription().isEmpty() && Config_getBool(REPEAT_BRIEF))
        {
        ol.parseDoc(briefFile(),briefLine(),this,0,briefDescription(),FALSE,FALSE);
        }
        if (!briefDescription().isEmpty() && Config_getBool(REPEAT_BRIEF) && !documentation().isEmpty())
        {
        ol.pushGeneratorState();
        ol.disable(OutputGenerator::Man);
        ol.disable(OutputGenerator::RTF);
        //ol.newParagraph(); // FIXME:PARA
        ol.enableAll();
        ol.disableAllBut(OutputGenerator::Man);
        ol.writeString("\n\n");
        ol.popGeneratorState();
        }
        */
        if (!documentation().isEmpty())
        {
          ol.generateDoc(docFile(), docLine(), this, 0, documentation() + "\n", TRUE, FALSE);
        }
        ol.endTextBlock();
      }
    }
      break;
    case LayoutDocEntry::MemberDefStart:
      //        startMemberDocumentation(ol);
      break;
    case LayoutDocEntry::FileInlineClasses:
      //        writeInlineClasses(ol);
      break;
    case LayoutDocEntry::MemberDef:
    {
      //          LayoutDocEntryMemberDef *lmd = (LayoutDocEntryMemberDef*)lde;
      //          writeMemberDocumentation(ol,lmd->type,lmd->title(lang));
    }
      break;
    case LayoutDocEntry::MemberDefEnd:
      //        endMemberDocumentation(ol);
      break;
    case LayoutDocEntry::ClassIncludes:
    case LayoutDocEntry::ClassInheritanceGraph:
    case LayoutDocEntry::ClassNestedClasses:
    case LayoutDocEntry::ClassCollaborationGraph:
    case LayoutDocEntry::ClassAllMembersLink:
    case LayoutDocEntry::ClassUsedFiles:
    case LayoutDocEntry::ClassInlineClasses:
    case LayoutDocEntry::NamespaceNestedNamespaces:
    case LayoutDocEntry::NamespaceClasses:
    case LayoutDocEntry::NamespaceInlineClasses:
    case LayoutDocEntry::GroupClasses:
    case LayoutDocEntry::GroupInlineClasses:
    case LayoutDocEntry::GroupNamespaces:
    case LayoutDocEntry::GroupDirs:
    case LayoutDocEntry::GroupNestedGroups:
    case LayoutDocEntry::GroupFiles:
    case LayoutDocEntry::GroupGraph:
    case LayoutDocEntry::GroupPageDocs:
    case LayoutDocEntry::DirSubDirs:
    case LayoutDocEntry::DirFiles:
    case LayoutDocEntry::DirGraph:
      err("Internal inconsistency: member %d should not be part of "
        "LayoutDocManager::File entry list\n", lde->kind());
      break;
    }
  }

  //---------------------------------------- end flexible part -------------------------------

  ol.endContents();

  return;
}


int RequirementSDict::compareItems(QCollection::Item item1, QCollection::Item item2)
{
  return qstricmp(((Requirement *)item1)->name(), ((Requirement *)item2)->name());
}

int Dot_GraphSDict::compareItems(QCollection::Item item1, QCollection::Item item2)
{
  return qstricmp(((Dot_Graph *)item1)->name(), ((Dot_Graph *)item2)->name());
}


Requirement::Requirement(const char *f, int l, const char *n,
  const char *d, const char *t)
  : DefinitionImpl(f, l, 1, n), m_title(t)
{
  std::string tmp_str(n);
  m_name__ = tmp_str;
  m_outOfScope = false;
  m_pageScope = 0;
  m_nestingLevel = -1;
  m_showToc = FALSE;

  setDocumentation(d, f, l);
  //  m_subPageDict = new PageSDict(7);

  QCString tmp_arg = t;
  if (tmp_arg.contains("out_of_scope")) {
    m_outOfScope = true;
  }

  mp_parentReq = 0;
  m_req_type = Requirement::safety;
}

Requirement::~Requirement()
{
  //  delete m_subPageDict;
}

bool Requirement::addChildReq(Requirement& a_req) {
  bool tmp_retVal = true;

  int tmp_index = m_childsReq.count();
  if (!m_childsReq.resize(tmp_index + 1))		// no memory
  {
    tmp_retVal = false;
  }
  m_childsReq.insert(tmp_index, &(a_req));

  a_req.mp_parentReq = this;
  if (this->m_nestingLevel >= 1) {
    a_req.m_nestingLevel = 1;
  }
  else {
    a_req.m_nestingLevel = (this->m_nestingLevel + 1);
  }

  return tmp_retVal;
}

QCString Requirement::getOutputFileBase() const
{
  return m_fileName;
}

void Requirement::writeDocumentation(OutputList &ol) {
  static bool generateTreeView = Config_getBool(GENERATE_TREEVIEW);

  std::string tmp_blabla(this->m_name__);
  if (tmp_blabla.compare("UC-73.1") == 0) {
    int iiii = 0;
  }

  HighlightedItem tmp_HLI;
  switch (this->m_req_type) {
  case Requirement::safety:
  {
    tmp_HLI = HLI_Req_safety;
    break;
  }
  case Requirement::safety_critical:
  {
    tmp_HLI = HLI_Req_safety_critical;
    break;
  }
  case Requirement::use_case:
  {
    tmp_HLI = HLI_Req_use_case;
    break;
  }
  case Requirement::test_case:
  {
    tmp_HLI = HLI_Req_test_case;
    break;
  }
  default:
  {
    tmp_HLI = HLI_RequirementsList;
    break;
  }
  }
  
  startFile(ol, getOutputFileBase(), name(), name(), tmp_HLI);
  ol.startContents();
  
  ol.pushGeneratorState();
  ol.disableAllBut(OutputGenerator::Latex);
  if (this->m_nestingLevel < 0) {  //patch because startGroupHeader is same thing at level 0 or -1
    ol.startTitle();
  }
  else {
    ol.startGroupHeader(this->m_nestingLevel);
  }
  ol.popGeneratorState();
  ol.pushGeneratorState();
  ol.disable(OutputGenerator::Latex);
  ol.startGroupHeader();
  ol.popGeneratorState();
  
  ol.parseText(this->name());

  ol.pushGeneratorState();
  ol.disableAllBut(OutputGenerator::Latex);
  if (this->m_nestingLevel < 0) {  //patch because endGroupHeader is same thing at level 0 or -1
    ol.endTitle();
  }
  else {
    ol.endGroupHeader(this->m_nestingLevel);
  }
  ol.popGeneratorState();
  ol.pushGeneratorState();
  ol.disable(OutputGenerator::Latex);
  ol.endGroupHeader();
  ol.popGeneratorState();

  //---------------------------------------- start flexible part -------------------------------
  
  SrcLangExt lang = getLanguage();
  QListIterator<LayoutDocEntry> eli(LayoutDocManager::instance().docEntries(LayoutDocManager::File));
  LayoutDocEntry *lde;
  for (eli.toFirst(); (lde = eli.current()); ++eli)
  {
    switch (lde->kind())
    {
    case LayoutDocEntry::BriefDesc:
      writeBriefDescription(ol);
      break;
    case LayoutDocEntry::DetailedDesc:
    {
      LayoutDocEntrySection *ls = (LayoutDocEntrySection*)lde;
      writeDetailedDescription(ol, name());
    }
      break;
    }
  }
  
  //---------------------------------------- end flexible part -------------------------------

  ol.pushGeneratorState();
  ol.startSubsection();
  ol.parseText(theTranslator->trRequirementSatisfiedBy());
  ol.endSubsection();
  ol.startItemList();

  if (this->m_outOfScope == true) {
    ol.startItemListItem();
    ol.writeObjectLink(0, "requirementsunsatisfied", "out_of_scope", "out of scope");
    ol.endItemListItem();
  }
  else {
    RequirementSatisfied* tmpp_reqSat;
    bool tmp_satisfied = false;


    if (this->m_req_type == Requirement::test_case) {
      tmp_satisfied = true;  //test_case dont need to be satisfied (with req_sat), only tested (with req_test)
    }
    else {
      for (uint tmp_ii = 0; tmp_ii < Doxygen::reqSatList->size(); tmp_ii++) {
        tmpp_reqSat = Doxygen::reqSatList->at(tmp_ii);

        std::string tmp_str(this->getOutputFileBase());
        if (tmpp_reqSat->reqID == tmp_str) {
          DocInternalRef *ref = new DocInternalRef(0, tmpp_reqSat->refItem->name);
          if (ref)
          {
            ol.startItemListItem();
            ol.writeObjectLink(0, ref->file(), ref->anchor(), tmpp_reqSat->refItem->title);
            ol.endItemListItem();
            tmp_satisfied = true;
          }
        }
      }
    }

    if (tmp_satisfied == false) {
      Doxygen::requirementUnsatisfiedSDict->inSort(this->name(), this);
      ol.writeObjectLink(0, "requirementsunsatisfied", "in_scope", "unsatisfied");
    }
  }

  ol.endItemList();
  ol.popGeneratorState();

  ol.pushGeneratorState();
  ol.startSubsection();
  ol.parseText(theTranslator->trRequirementTestedBy());
  ol.endSubsection();
  ol.startItemList();

  RequirementTested* tmpp_reqTest;
  bool tmp_tested = false;

  for (uint tmp_ii = 0; tmp_ii < Doxygen::reqTestList->count(); tmp_ii++) {
    tmpp_reqTest = Doxygen::reqTestList->at(tmp_ii);

    if (tmpp_reqTest->reqID == this->getOutputFileBase()) {
      DocInternalRef *ref = new DocInternalRef(0, tmpp_reqTest->refItem->name);
      if (ref)
      {
        ol.startItemListItem();
        ol.writeObjectLink(0, ref->file(), ref->anchor(), tmpp_reqTest->refItem->title);
        ol.endItemListItem();
        tmp_tested = true;
      }
    }
  }

  if (tmp_tested == false) {
    Doxygen::requirementUntestedSDict->inSort(this->name(), this);
    ol.writeObjectLink(0, "requirementsuntested", "all", "untested");
  }

  ol.endItemList();
  ol.popGeneratorState();

  if (this->mp_parentReq != 0) {
    ol.pushGeneratorState();
    ol.disableAllBut(OutputGenerator::Html);

    ol.startSubsection();
    ol.parseText(theTranslator->trRequirementExtend());
    ol.endSubsection();

    ol.startItemList();
    DocInternalRef *ref = new DocInternalRef(0, mp_parentReq->getOutputFileBase());
    if (ref)
    {
      ol.startItemListItem();
      ol.writeObjectLink(0, ref->file(), ref->anchor(), mp_parentReq->name());
      ol.endItemListItem();
    }
    ol.endItemList();

    ol.popGeneratorState();
  }

  if (this->m_childsReq.count() > 0) {
    Requirement* tmp_childReq;

    ol.pushGeneratorState();
    ol.disableAllBut(OutputGenerator::Html);

    ol.startGroupHeader();
    ol.parseText(theTranslator->trRequirementExtendedBy());
    ol.endGroupHeader();
    ol.startItemList();

    for (uint tmp_ii = 0; tmp_ii < this->m_childsReq.count(); tmp_ii++) {
      tmp_childReq = this->m_childsReq.at(tmp_ii);
      DocInternalRef *ref = new DocInternalRef(0, tmp_childReq->getOutputFileBase());
      if (ref)
      {
        ol.startItemListItem();
        ol.writeObjectLink(0, ref->file(), ref->anchor(), tmp_childReq->name());
        ol.endItemListItem();
      }
    }

    ol.endItemList();

    ol.popGeneratorState();
    ol.pushGeneratorState();
    ol.disableAllBut(OutputGenerator::Latex);

    for (uint tmp_ii = 0; tmp_ii < this->m_childsReq.count(); tmp_ii++) {
      tmp_childReq = this->m_childsReq.at(tmp_ii);
      DocInternalRef *ref = new DocInternalRef(0, tmp_childReq->getOutputFileBase());
      if (ref)
      {
        ol.writePageLink(tmp_childReq->getOutputFileBase(), false);
      }
    }

    ol.popGeneratorState();
  }
  
  ol.endContents();
  ol.endFile();

  return;
}

void Requirement::writeBriefDescription(OutputList &ol)
{
  if (!briefDescription().isEmpty() && Config_getBool(BRIEF_MEMBER_DESC))
  {
    ol.startParagraph();
    ol.generateDoc(briefFile(), briefLine(), this, 0, briefDescription(), TRUE, FALSE, 0, TRUE, FALSE);
    ol.pushGeneratorState();
    ol.disable(OutputGenerator::RTF);
    ol.writeString(" \n");
    ol.enable(OutputGenerator::RTF);

    if (Config_getBool(REPEAT_BRIEF) ||
      !documentation().isEmpty()
      )
    {
      ol.disableAllBut(OutputGenerator::Html);
      ol.startTextLink(0, "details");
      ol.parseText(theTranslator->trMore());
      ol.endTextLink();
    }
    ol.popGeneratorState();
    ol.endParagraph();
  }
  ol.writeSynopsis();
}

void Requirement::writeDetailedDescription(OutputList &ol, const QCString &title)
{
  if ((!briefDescription().isEmpty() && Config_getBool(REPEAT_BRIEF)) || !documentation().isEmpty())
  {
    ol.pushGeneratorState();
    ol.disable(OutputGenerator::Html);
    ol.writeRuler();
    ol.popGeneratorState();
    ol.pushGeneratorState();
    ol.disableAllBut(OutputGenerator::Html);
    ol.writeAnchor(0, "details");
    ol.popGeneratorState();

    ol.startTextBlock();

    if (!documentation().isEmpty())
    {
      ol.generateDoc(docFile(), docLine(), this, 0, documentation() + "\n", TRUE, FALSE);
    }
    ol.endTextBlock();
  }
}

void Requirement::addListReferences()
{
  {
    const std::vector<ListItemInfo> &xrefItems = xrefListItems();
    addRefItem(
      xrefItems,
      qualifiedName(),
      getLanguage() == SrcLangExt_Fortran ? theTranslator->trModule(TRUE, TRUE) : theTranslator->trNamespace(TRUE, TRUE),
      getOutputFileBase(),
      displayName(),
      0,
      0
      );
  }

  return;
}










Dot_Graph::Dot_Graph(const char *f, int l, const char *n,
  const char *d, const char *t)
  : DefinitionImpl(f, l, 1, n), m_title(t)
{
  setDocumentation(d, f, l);
  //  m_subPageDict = new PageSDict(7);
  m_pageScope = 0;
  m_showToc = FALSE;
}

Dot_Graph::~Dot_Graph()
{
  //  delete m_subPageDict;
}

QCString Dot_Graph::getOutputFileBase() const
{
  return m_fileName;
}

void Dot_Graph::writeDocumentation(OutputList &ol) {

  static bool generateTreeView = Config_getBool(GENERATE_TREEVIEW);

  startFile(ol, getOutputFileBase(), name(), name(), HLI_RequirementsGraphs);
  ol.startContents();
  //---------------------------------------- start flexible part -------------------------------

  SrcLangExt lang = getLanguage();
  QListIterator<LayoutDocEntry> eli(LayoutDocManager::instance().docEntries(LayoutDocManager::File));
  LayoutDocEntry *lde;
  for (eli.toFirst(); (lde = eli.current()); ++eli)
  {
    switch (lde->kind())
    {
    case LayoutDocEntry::BriefDesc:
      writeBriefDescription(ol);
      break;
    case LayoutDocEntry::MemberDeclStart:
      //        startMemberDeclarations(ol);
      break;
    case LayoutDocEntry::FileIncludes:
      //        writeIncludeFiles(ol);
      break;
    case LayoutDocEntry::FileIncludeGraph:
      //        writeIncludeGraph(ol);
      break;
    case LayoutDocEntry::FileIncludedByGraph:
      //        writeIncludedByGraph(ol);
      break;
    case LayoutDocEntry::FileSourceLink:
      //        writeSourceLink(ol);
      break;
    case LayoutDocEntry::FileClasses:
    {
      //          LayoutDocEntrySection *ls = (LayoutDocEntrySection*)lde;
      //          writeClassDeclarations(ol,ls->title(lang));
    }
      break;
    case LayoutDocEntry::FileNamespaces:
    {
      //          LayoutDocEntrySection *ls = (LayoutDocEntrySection*)lde;
      //          writeNamespaceDeclarations(ol,ls->title(lang));
    }
      break;
    case LayoutDocEntry::MemberGroups:
      //        writeMemberGroups(ol);
      break;
    case LayoutDocEntry::MemberDecl:
    {
      //          LayoutDocEntryMemberDecl *lmd = (LayoutDocEntryMemberDecl*)lde;
      //          writeMemberDeclarations(ol,lmd->type,lmd->title(lang));
    }
      break;
    case LayoutDocEntry::MemberDeclEnd:
      //        endMemberDeclarations(ol);
      break;
    case LayoutDocEntry::DetailedDesc:
    {
      LayoutDocEntrySection *ls = (LayoutDocEntrySection*)lde;
      writeDetailedDescription(ol, ls->title(lang));
    }
      break;
    case LayoutDocEntry::MemberDefStart:
      //        startMemberDocumentation(ol);
      break;
    case LayoutDocEntry::FileInlineClasses:
      //        writeInlineClasses(ol);
      break;
    case LayoutDocEntry::MemberDef:
    {
      //          LayoutDocEntryMemberDef *lmd = (LayoutDocEntryMemberDef*)lde;
      //          writeMemberDocumentation(ol,lmd->type,lmd->title(lang));
    }
      break;
    case LayoutDocEntry::MemberDefEnd:
      //        endMemberDocumentation(ol);
      break;
    case LayoutDocEntry::ClassIncludes:
    case LayoutDocEntry::ClassInheritanceGraph:
    case LayoutDocEntry::ClassNestedClasses:
    case LayoutDocEntry::ClassCollaborationGraph:
    case LayoutDocEntry::ClassAllMembersLink:
    case LayoutDocEntry::ClassUsedFiles:
    case LayoutDocEntry::ClassInlineClasses:
    case LayoutDocEntry::NamespaceNestedNamespaces:
    case LayoutDocEntry::NamespaceClasses:
    case LayoutDocEntry::NamespaceInlineClasses:
    case LayoutDocEntry::GroupClasses:
    case LayoutDocEntry::GroupInlineClasses:
    case LayoutDocEntry::GroupNamespaces:
    case LayoutDocEntry::GroupDirs:
    case LayoutDocEntry::GroupNestedGroups:
    case LayoutDocEntry::GroupFiles:
    case LayoutDocEntry::GroupGraph:
    case LayoutDocEntry::GroupPageDocs:
    case LayoutDocEntry::DirSubDirs:
    case LayoutDocEntry::DirFiles:
    case LayoutDocEntry::DirGraph:
      err("Internal inconsistency: member %d should not be part of "
        "LayoutDocManager::File entry list\n", lde->kind());
      break;
    }
  }

  //---------------------------------------- end flexible part -------------------------------

  return;
}

void Dot_Graph::writeBriefDescription(OutputList &ol)
{
  if (!briefDescription().isEmpty() && Config_getBool(BRIEF_MEMBER_DESC))
  {
    ol.startParagraph();
    ol.generateDoc(briefFile(), briefLine(), this, 0, briefDescription(), TRUE, FALSE, 0, TRUE, FALSE);
    ol.pushGeneratorState();
    ol.disable(OutputGenerator::RTF);
    ol.writeString(" \n");
    ol.enable(OutputGenerator::RTF);

    if (Config_getBool(REPEAT_BRIEF) ||
      !documentation().isEmpty()
      )
    {
      ol.disableAllBut(OutputGenerator::Html);
      ol.startTextLink(0, "details");
      ol.parseText(theTranslator->trMore());
      ol.endTextLink();
    }
    ol.popGeneratorState();
    ol.endParagraph();

    //ol.pushGeneratorState();
    //ol.disable(OutputGenerator::RTF);
    //ol.newParagraph();
    //ol.popGeneratorState();
  }
  ol.writeSynopsis();
}

void Dot_Graph::writeDetailedDescription(OutputList &ol, const QCString &title)
{
  if ((!briefDescription().isEmpty() && Config_getBool(REPEAT_BRIEF)) || !documentation().isEmpty())
  {
    ol.pushGeneratorState();
    ol.disable(OutputGenerator::Html);
    ol.writeRuler();
    ol.popGeneratorState();
    ol.pushGeneratorState();
    ol.disableAllBut(OutputGenerator::Html);
    ol.writeAnchor(0, "details");
    ol.popGeneratorState();
    ol.startGroupHeader();
    ol.parseText(title.upper());
    ol.endGroupHeader();

    ol.startTextBlock();
    /*
    if (!briefDescription().isEmpty() && Config_getBool(REPEAT_BRIEF))
    {
    ol.parseDoc(briefFile(),briefLine(),this,0,briefDescription(),FALSE,FALSE);
    }
    if (!briefDescription().isEmpty() && Config_getBool(REPEAT_BRIEF) && !documentation().isEmpty())
    {
    ol.pushGeneratorState();
    ol.disable(OutputGenerator::Man);
    ol.disable(OutputGenerator::RTF);
    //ol.newParagraph(); // FIXME:PARA
    ol.enableAll();
    ol.disableAllBut(OutputGenerator::Man);
    ol.writeString("\n\n");
    ol.popGeneratorState();
    }
    */
    if (!documentation().isEmpty())
    {
      ol.generateDoc(docFile(), docLine(), this, 0, documentation() + "\n", TRUE, FALSE);
    }
    ol.endTextBlock();
  }
}

void Dot_Graph::addListReferences()
{
  //bool fortranOpt = Config_getBool(OPTIMIZE_FOR_FORTRAN);
  {

    const std::vector<ListItemInfo> &xrefItems = xrefListItems();
    addRefItem(
      xrefItems,
      qualifiedName(),
      getLanguage() == SrcLangExt_Fortran ? theTranslator->trModule(TRUE, TRUE) : theTranslator->trNamespace(TRUE, TRUE),
      getOutputFileBase(), displayName(),
      0,
      0
      );
  }

  return;
}



void RequirementSatisfiedList::f_addFromRefItem(RefItem &a_refItem)
{
  QRegExp tmp_regx1("\\\\ref\\s+");
  QRegExp tmp_regx2("[A-Za-z0-9_]+");
  int tmp_start = 0;
  int tmp_len = 0;
  int tmp_st = 0;
  QCString tmp_req_name;

  QCString tmp_text = a_refItem.text;

  while (tmp_st >= 0) {
    tmp_st = tmp_regx1.match(tmp_text, tmp_start, &tmp_len);
    if (tmp_st >= 0) {
      tmp_start = (tmp_st + tmp_len);
      tmp_st = tmp_regx2.match(tmp_text, tmp_start, &tmp_len);
      if (tmp_st >= 0) {
        tmp_start = (tmp_st + tmp_len);
        tmp_req_name = tmp_text.mid(tmp_st, tmp_len);

        std::string tmp_req_name2(tmp_req_name);
        RequirementSatisfied* rs = new RequirementSatisfied(tmp_req_name2, a_refItem);
        this->push_back(rs);
      }
    }
  }

  return;
}

void RequirementTestedList::f_addFromRefItem(RefItem &a_refItem)
{
  QRegExp tmp_regx1("\\\\ref\\s+");
  QRegExp tmp_regx2("[A-Za-z0-9_]+");
  int tmp_start = 0;
  int tmp_len = 0;
  int tmp_st = 0;
  QCString tmp_req_name;

  QCString tmp_text = a_refItem.text;

  while (tmp_st >= 0) {
    tmp_st = tmp_regx1.match(tmp_text, tmp_start, &tmp_len);
    if (tmp_st >= 0) {
      tmp_start = (tmp_st + tmp_len);
      tmp_st = tmp_regx2.match(tmp_text, tmp_start, &tmp_len);
      if (tmp_st >= 0) {
        tmp_start = (tmp_st + tmp_len);
        tmp_req_name = tmp_text.mid(tmp_st, tmp_len);

        RequirementTested* rs = new RequirementTested(tmp_req_name, a_refItem);
        this->append(rs);
      }
    }
  }

  return;
}