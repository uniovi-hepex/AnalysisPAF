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
			     Int_t witherrors):
  fNColumns(ncol),
  fNRows(nrows),
  fWithErrors(witherrors),
  fRowTitleHeader(""),
  fDrawHLines(false),
  fDrawVLines(false),
  fColumnWidth(11) {

  formatNum = TString("%1.2f");
  VSeparations = "";
  //colorRow[20] = {"#0099cc", "#ff9966", "#99ff99", "#ff66ff", "#99cc00", "#0099cc", "#ff9966", "#99ff99", "#ff66ff", "#99cc00", "#0099cc", "#ff9966", "#99ff99", "#ff66ff", "#99cc00", "#0099cc", "#ff9966", "#99ff99", "#ff66ff", "#99cc00"};
  //color[20]    = {"#ccffff", "#ffffcc", "#ccffcc", "#ffccff", "#ccff99", "#ccffff", "#ffffcc", "#ccffcc", "#ffccff", "#ccff99", "#ccffff", "#ffffcc", "#ccffcc", "#ffccff", "#ccff99", "#ccffff", "#ffffcc", "#ccffcc", "#ffccff", "#ccff99"};

  fRows = new TResultsTableRow[fNRows];

  fRowTitle = new TString[fNRows];
  fColumnTitle = new TString[fNColumns];

  for (unsigned int i = 0; i < fNRows; i++) {
    fRows[i].SetNColumns(fNColumns);
  }

  //if (fWithErrors > 0) fColumnWidth=2*fColumnWidth+3;
  if (fWithErrors > 0) fColumnWidth=(1+fWithErrors)*fColumnWidth+3*fWithErrors;
}


TString TResultsTable::FixWidth(const TString& s, unsigned int width, bool prepend) const {
    TString val(s);
    if (prepend)
      val.Prepend(' ', width - val.Length());
    else
      val.Append(' ', width - val.Length());
    return val;
}

void TResultsTable::Print(ETResultsTableOutputFormat format, ostream& os) const{

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
  //if (fWithErrors > 0) colwidth=(colwidth/2)-3;
  if (fWithErrors > 0) colwidth=(colwidth/(1+fWithErrors))-3*fWithErrors;

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
    cellcoltitstart = "<th bgcolor=\"lightcoral\">";
    cellcoltitend   = "</th>";
    cellrowtitstart = "<th align=\"right\" bgcolor=\"blue\">";
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
  iColor = 0;
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
    if(format == kHTML) cellrowtitstart = getCellColor(-1);
    os << rowstart << cellrowtitstart << FixWidth(fRowTitle[i],firstcolw) << cellrowtitend;
    for (unsigned int j = 0; j < GetNColumns(); j++) {
    if(format == kHTML) cellstart = getCellColor(j);
      os << cellstart;
      os.width(colwidth);
      if     (formatNum.Contains("fix") ) os << KeepAllErrorDecimals(fRows[i][j].Content(), fRows[i][j].Error());
      else if(formatNum.Contains("auto")) os << KeepOneDecimal(      fRows[i][j].Content(), fRows[i][j].Error());
      else    os << Form(formatNum,fRows[i][j].Content());

      if (fWithErrors == 1) {
        os.width(colwidth);
        os << plusminus; 
        if     (formatNum.Contains("fix") ) os << KeepAllErrorDecimals(fRows[i][j].Error(), 0);
        else if(formatNum.Contains("auto")) os << KeepOneDecimal(      fRows[i][j].Error(), 0);
        else os << Form(formatNum, fRows[i][j].Error());
      }
      else if (fWithErrors > 1) {
        os.width(colwidth);
        os << plusminus; 
        if     (formatNum.Contains("fix") ) os << KeepAllErrorDecimals(fRows[i][j].StatError(), 0);
        else if(formatNum.Contains("auto")) os << KeepOneDecimal(      fRows[i][j].StatError(), 0);
        else os << Form(formatNum, fRows[i][j].StatError());

        os.width(colwidth);
        os << plusminus; 
        if     (formatNum.Contains("fix") ) os << KeepAllErrorDecimals(fRows[i][j].Error(), 0);
        else if(formatNum.Contains("auto")) os << KeepOneDecimal(      fRows[i][j].Error(), 0);
        else os << Form(formatNum, fRows[i][j].Error());
      }

      if (format != kLaTeX || j != GetNColumns()-1) os << cellend;
    }
    os << rowend << std::endl;
    if(VSeparations.Contains(TString(Form("l%i,", i)))){ os << doubleline; iColor++;}
    else if (fDrawHLines && i != GetNRows()-1) os << singleline;
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

TString TResultsTable::getCellColor(Int_t col) const{
  TString out = "";
  if(col < 0){ // title
    out = TString("<th align=\"right\" bgcolor=\"") + colorRow[iColor] + TString("\">");
  }
  else{ // normal
    out = TString("<td bgcolor=\"") + color[iColor] + TString("\">");
  }
  return out;
}

TString KeepAllErrorDecimals(Float_t number, Float_t error){
  Int_t d = 6;
  TString f;
  if(error == 0) f = Form("%1.6f", number);
  else           f = Form("%1.6f", error);
  for(Int_t i = f.Sizeof()-2; i > f.First('.'); i--){
    if(f[i] == '0') d--;
    else break;
  }
  TString format = TString("%1.") + TString(Form("%i", d)) + TString("f");
  return Form(format, number);
}

TString KeepOneDecimal(Float_t number, Float_t error){
  if(number < 0.000001) return TString(Form("%g", number));
  Int_t d = 1;
  TString f;
  if     (error == 0){
    if(number > 0) return(Form("%1.0f", number));
    else f = Form("%1.6f", number);
  }
  else if(error > 0.) return(Form("%1.0f", number));
  else                f = Form("%1.6f", error);
  for(Int_t i = f.First('.') +1; i < f.Sizeof(); i++){
    if(f[i] == '0') d++;
    else break;
  }
  if(d == 7) d = 0;
  TString format = TString("%1.") + TString(Form("%i", d)) + TString("f");
  return Form(format, number);
}

