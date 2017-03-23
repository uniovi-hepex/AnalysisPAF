#ifndef TRESULTSTABLE_H
#define TRESULTSTABLE_H

#include <iostream>
#include "TString.h"

enum ETResultsTableOutputFormat {kPlain, kLaTeX, kHTML, kCSV};

class TResultsTableContent {
 public:
  TResultsTableContent(double content = 0) {fContent = content;}

  double Content() const {return fContent;}
  double& Content() {return fContent;}
  double Error() const {return fError;}
  double& Error() {return fError;}

  void SetContent(double c) {fContent = c;}
  void SetError(double e) {fError = e;}

  operator double() const {return fContent;}
  operator double() {return fContent;}

 protected:
  double fContent;
  double fError;

};

class TResultsTableRow {
 public:
  TResultsTableRow(unsigned int ncol=0);
  ~TResultsTableRow() {if (fRow) delete[] fRow;}

  TResultsTableContent& operator[](unsigned int i) {return fRow[i];}
  TResultsTableContent  operator[](unsigned int i) const {return fRow[i];}

  

  unsigned int GetNColumns() const {return fNColumns;}
  void SetNColumns(unsigned int ncols);

 protected:
  TResultsTableContent*  fRow;
  unsigned int fNColumns;
};


class TResultsTable {
 public:
  TResultsTable(unsigned int nrows, unsigned int ncols, bool witherrors = false, bool automaticerrors=true);
  ~TResultsTable() {if (fRows) delete[] fRows;}

  ////////////////////////////////////////////////////////////////////////////
  //
  // Operator []
  // These will allow to recover a row and from there a cell to access the
  // content. Examples:
  // + sometable[i][j] will return the value stored in cell_ij
  // + sometable[i][j].Content() is equivalente to the previous line
  // + sometable[i][j].Error() will return the error stored in cell_ij
  //
  TResultsTableRow& operator[](unsigned int i) {return fRows[i];}
  TResultsTableRow  operator[](unsigned int i) const {return fRows[i];}
  //
  ////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////
  //
  // Some options
  //

  // Set it to true if your want all lines to be drawn (horizontal or vertical)
  // Default value is false
  void SetDrawHLines(bool draw = false) {fDrawHLines = draw;}
  void SetDrawVLines(bool draw = false) {fDrawVLines = draw;}
  // Set the width of each content column
  void SetPlainTextColumnWidth(unsigned int w) {fColumnWidth = w;}
  //
  ////////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////////////////////////
  //
  // Saving and printing
  //
  void SaveAs(const TString& filename) const;  
  void Print(ETResultsTableOutputFormat format = kPlain, ostream& os=std::cout) const;
  //
  ////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////
  //
  // Some getters and setters
  //
  void SetContent(unsigned int i, unsigned int j, double value) {fRows[i][j] = value;}
  void SetRowTitle(unsigned int i, TString title) {fRowTitle[i] = title;}
  void SetColumnTitle(unsigned int j, TString title) {fColumnTitle[j] = title;}
  void SetRowTitleHeader(const char* str) { fRowTitleHeader = str;}
 

  unsigned int GetNColumns() const { return fNColumns;}
  unsigned int GetNRows() const { return fNRows;}

  double GetContent(unsigned int i, unsigned int j) const {return fRows[i][j];}
  TString GetRowTitle(unsigned int i) const {return fRowTitle[i];}
  TString GetColumnTitle(unsigned int j) const {return fColumnTitle[j];}
  TString GetRowTitleHeader() const { return fRowTitleHeader;}
  //
  ////////////////////////////////////////////////////////////////////////////


 protected:
  TString FixWidth(const TString& s, unsigned int width=11, bool prepend = true) const;


  unsigned int fNColumns; //Number of columns
  unsigned int fNRows;    //Number of rows

  bool fWithErrors;
  bool fAutomaticErrors;

  TResultsTableRow* fRows;    //The table rows (content)

  TString     fRowTitleHeader; //String to put on top of row titles

  TString*    fRowTitle;    //Each row title
  TString*    fColumnTitle; //Each columnt title

  //Options
  bool fDrawHLines;
  bool fDrawVLines;
  unsigned int fColumnWidth;
};


#endif
