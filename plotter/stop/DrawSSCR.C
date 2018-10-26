#include "SSCR.C"

void DrawSSCR(){
  SSCR* s = new SSCR();
  s->DrawSSCR("SSCRplot");
  s->PrintSStable("SSCRtable");
  delete s;
  return;
}
