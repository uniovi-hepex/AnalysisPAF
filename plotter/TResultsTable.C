#include "TResultsTable.h"

#include <iostream>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////////////
// TResultsTableRow
TResultsTableRow::TResultsTableRow(unsigned int ncol):fRow(0), fNColumns(ncol) {
  if (fNColumns)
    fRow = new TResultsTableContent[fNColumns];
}

void TResultsTableRow::SetNColumns(unsigned int ncols) {
  if (ncols) {
    if (fNColumns)
      delete[] fRow;

    fNColumns=ncols;
    fRow = new TResultsTableContent[fNColumns];
  }
  else
    std::cerr << "ERROR: Trying to delete information in TResultsTableRow by setting the number of columns to 0" << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////////
// TResultsTable
TResultsTable::TResultsTable(unsigned int nrows, 
			     unsigned int ncol,
			     bool witherrors,
			     bool automaticerrors):
  fNColumns(ncol),
  fNRows(nrows),
  fWithErrors(witherrors),
  fAutomaticErrors(automaticerrors),
  fRowTitleHeader(""),
  fDrawHLines(false),
  fDrawVLines(false),
  fColumnWidth(11) {

  fRows = new TResultsTableRow[fNRows];

  fRowTitle = new TString[fNRows];
  fColumnTitle = new TString[fNColumns];

  for (unsigned int i = 0; i < fNRows; i++) {
    fRows[i].SetNColumns(fNColumns);
  }

  if (fWithErrors)
    fColumnWidth=2*fColumnWidth+3;

  if (fAutomaticErrors)
    std::cerr << "WARNING: Automatic errors not yet implemented" << std::endl;
}

TString TResultsTable::FixWidth(const TString& s, unsigned int width, bool prepend) const {
    TString val(s);
    if (prepend)
      val.Prepend(' ', width - val.Length());
    else
      val.Append(' ', width - val.Length());
    return val;
}

void TResultsTable::Print(ETResultsTableOutputFormat format, ostream& os) const {

  if (format == kCSV) {
    std::cerr << "WARNING: The CSV format is not yet supported by TResultsTable!" << std::endl;
    return;
  }
  //  else if (format == kLaTeX) {
  //    std::cerr << "WARNING: The LaTeX format is not yet supported by TResultsTable!" << std::endl;
  //    return;
  //  }
    
  // Widths
  unsigned int colwidth = fColumnWidth;
  if (fWithErrors)
    colwidth=(colwidth/2)-3;

  unsigned int firstcolw = GetRowTitleHeader().Length();
  for (unsigned int i = 0; i < fNRows; i++) {
    firstcolw = TMath::Max(firstcolw, (unsigned int)GetRowTitle(i).Length());
  }


  // Tokens
  TString tablestart("");
  TString tableend("");
  TString rowstart("");
  TString rowend("");
  TString cellstart("");
  TString cellend("");
  TString cellcoltitstart("");
  TString cellcoltitend("");
  TString cellrowtitstart("");
  TString cellrowtitend("");
  TString plusminus("");
  TString singleline("");
  TString doubleline("");

  if (format == kPlain) {
    tablestart    = "Printing table... ";
    cellrowtitend = "| ";
    if (fDrawVLines) {
      cellcoltitend = "| ";
      cellend       = "| ";
    }
    else {
      cellcoltitend = "  ";
      cellend       = "  ";
    }      
    plusminus     = " +- ";

    doubleline =  TString().Prepend('=',firstcolw); 
    for (unsigned int i = 0; i < fNColumns; i++) {
      doubleline += "+=";
      doubleline += TString().Prepend('=',fColumnWidth);
    }
    doubleline += "+\n";

    if (fDrawHLines) {
      singleline = TString().Prepend('-',firstcolw); 
      for (unsigned int i = 0; i < fNColumns; i++) {
	singleline += "+-";
	singleline += TString().Prepend('-',fColumnWidth);
      }
    }
    singleline += "+\n";

  }
  else if (format == kHTML) {
    if (fDrawVLines || fDrawHLines)
      tablestart      = "<table border=\"1px\" cellpadding=\"4px\" cellspacing=\"0\">";
    else
      tablestart      = "<table border=\"0px\" cellpadding=\"4px\" cellspacing=\"0\">";
    tableend        = "</table>";
    rowstart        = "<tr>";
    rowend          = "</tr>";
    cellstart       = "<td>";
    cellend         = "</td>";
    cellcoltitstart = "<th bgcolor=\"lightblue\">";
    cellcoltitend   = "</th>";
    cellrowtitstart = "<th align=\"right\" bgcolor=\"lightblue\">";
    cellrowtitend   = "</th>";
    plusminus       = "&plusmn;";
  }
  else if (format == kLaTeX) {
    TString linesep("");
    if (fDrawVLines)
      linesep = " | ";
    tablestart      = TString("\\begin{tabular}{ ") + linesep + "r | ";
    for (unsigned int j = 0; j < fNColumns; j++)
      tablestart+=" l " + linesep;
    tablestart+="}";
    tableend        = "\\end{tabular}";
    rowend          = " \\\\";
    cellend         = " &";
    cellcoltitstart = " {\\bf ";
    cellcoltitend   = " } &";
    cellrowtitstart = " {\\bf ";
    cellrowtitend   = " } &";
    plusminus       = "$\\pm$ ";
    singleline      = " \\hline\n";
    doubleline      = "\\hline\n";
    if (fDrawHLines)
      doubleline    = "\\hline\\hline\n";
  }
  


  //Start table
  os << tablestart << std::endl;


  //Column titles
  os << rowstart << cellcoltitstart << FixWidth(fRowTitleHeader, firstcolw) << cellcoltitend;
  for (unsigned int i = 0; i < fNColumns; i++) {
    os << cellcoltitstart << FixWidth(fColumnTitle[i],fColumnWidth);
    if (format != kLaTeX || i != GetNColumns()-1)
      os << cellcoltitend;
    else
      os << "}"; //Ugly... need to think better
  }
  os << rowend << std::endl;


  os << doubleline;

  //Content
  for (unsigned int i = 0; i < GetNRows(); i++) {
    os << rowstart << cellrowtitstart << FixWidth(fRowTitle[i],firstcolw) << cellrowtitend;
    for (unsigned int j = 0; j < GetNColumns(); j++) {
      os << cellstart;
      os.width(colwidth);
      os << fRows[i][j];

      if (fWithErrors) {
	os.width(colwidth);
	os << plusminus << fRows[i][j].Error();
      }
      
      if (format != kLaTeX || j != GetNColumns()-1)
	os<< cellend;
    }
    os << rowend << std::endl;
    if (fDrawHLines && i != GetNRows()-1)
      os << singleline;
  }

  os << doubleline;

  //Footers
  os << tableend << std::endl;
}

void TResultsTable::SaveAs(const TString& filename) const {
  ofstream os(filename);
  if (filename.EndsWith(".html"))
    Print(kHTML, os);
  else if (filename.EndsWith(".tex"))
    Print(kLaTeX, os);
  else if (filename.EndsWith(".csv"))
    Print(kCSV, os);
  else if (filename.EndsWith(".txt") || filename.EndsWith(".log"))
    Print(kPlain, os);
  else {
    std::cerr << "WARNING: Unknown file type for \"" << filename << "\"." << std::endl;
    std::cerr << "         Saving as plain text." << std::endl;
    Print(kPlain, os);
  }  
}

