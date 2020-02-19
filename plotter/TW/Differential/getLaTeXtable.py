import ROOT as r
import varList as vl
import tabulate as tb
import warnings as wr
import sys, os, copy
from collections import OrderedDict
r.gROOT.SetBatch(True)

def saveLaTeXfromhisto(histo, varname, path = "./", outname = "", ty = "folded", errhisto = None):
    table   = [[], []]; headers = []
    usehisto = r.TH1D()

    if errhisto != None:
        usehisto = copy.deepcopy(histo.Clone("usehisto"))
        for bn in range(1, usehisto.GetNbinsX() + 1): usehisto.SetBinError(bn, errhisto.GetBinError(bn))
    else: usehisto = copy.deepcopy(histo.Clone("usehisto"))


    nbins   = usehisto.GetNbinsX()
    limits  = [int(round(usehisto.GetXaxis().GetBinLowEdge(bn), 0)) for bn in range(1, nbins + 1)]

    #### Construction of headers
    headers.append(vl.varList[varname]["printname"])
    if "DPhiLL" not in varname:
        for b in range(1, nbins + 1):
            if b == nbins:
                headers.append("[{inf}, $\\infty$)".format(inf = limits[b - 1]))
            else:
                headers.append("[{inf}, {sup})".format(inf = limits[b - 1], sup = limits[b]))
    else:
        for b in range(1, nbins + 1):
            if b == nbins:
                headers.append("[{inf}, 1]".format(inf = round(usehisto.GetXaxis().GetBinLowEdge(b), 2)))
            else:
                headers.append("[{inf}, {sup})".format(inf = round(usehisto.GetXaxis().GetBinLowEdge(b), 2), sup = round(usehisto.GetXaxis().GetBinLowEdge(b + 1), 2)))


    #### Construction of the table
    table[0].append(vl.TableDict[ty]["prefix"] + vl.varList[varname]["mathprintname"] + "\\right)$ " + vl.TableDict[ty]["dimensions"])
    table[1].append("Uncertainty " + vl.TableDict[ty]["dimensions"])
    for bn in range(1, nbins + 1):
        tmpval, tmpinc = getvalueandincstrings(usehisto, bn)
        table[0].append(tmpval); table[1].append(tmpinc)

    # To assure that we don't remove trailing zeros
    frm = tuple([""] + [".{l}f".format(l = len(table[0][ind].split(".")[1] )) for ind in range(1, len(table[0])) ])

    finaltab = tb.tabulate(table, headers = headers, floatfmt = frm, numalign = "decimal", stralign = "center", colalign=("left",))
    #print(finaltab)

    #print "\n\n"


    finallatextab = tb.tabulate(table, headers = headers, floatfmt = frm, tablefmt = "latex_raw")
    #print(finallatextab)

    substr = "".join(["l"] + ["r" for x in range(nbins)])
    finallatextab = finallatextab.replace(substr, "".join(["l|"] + ["c|" for x in range(nbins - 1)] + ["c"] ))


    #print finallatextab
    if not os.path.isdir(path): os.system("mkdir -p " + path)

    name = outname if outname != "" else (varname + "_" + ty)

    outfile = open(path + "/" + name + ".tex", "w")
    outfile.write(finallatextab)
    outfile.close(); del outfile

    return


def getvalueandincstrings(histo, b):
    valstring = ""; incstring = ""; # NOTE: Python by default uses up to 12 characters when
                                    #       transforming a float number into a string.
    valstring = str(histo.GetBinContent(b))
    incstring = str(histo.GetBinError(b))
    ndeczeros = 0
    numofdecs = 0

    if "e" in (valstring + incstring): raise RuntimeError("Scientific notation detected: case not supported.")

    if "." in incstring:
        incparts = incstring.split(".")

        if incparts[0].replace("0", "") == "": # There is no non-decimal part
            # Count figures that are zero
            if incparts[1][0] == "0":
                selfig = "0"
                while selfig == "0":
                    selfig = incparts[1][ndeczeros + 1]
                    ndeczeros += 1
                    if ndeczeros == len(incparts[1]) and selfig == "0":
                        raise RuntimeError("The uncertainty is completely zero!")

            numofdecs = ndeczeros + 1 # number of total decimals that we will take

            # Check if the first significant figure is a one or not.
            if incparts[1][ndeczeros] == "1":
                numofdecs += 1
        else:
            if   len(incparts[0]) == 1 and incparts[0][0] != 1: numofdecs = 0
            elif len(incparts[0]) == 1:                         numofdecs = 1
            else:
                wr.warn("WARNING: the result is a real number with only integer part and more than one digit. This should be saved with scientific notation, but it is not implemented. The full integer will be returned.")
                numofdecs = 0

        finalvalstring = str(round(histo.GetBinContent(b), numofdecs))
        finalincstring = str(round(histo.GetBinError(b), numofdecs))

        if   len(finalincstring) < len(finalvalstring):
            finalincstring += "0"
        elif len(finalincstring) > len(finalvalstring):
            finalvalstring += "0"

    return finalvalstring, finalincstring


def getyieldsLaTeXtable(xuanpath, outname = "yields", path = "./results/tables"):
    xuanchivo = open(xuanpath, "r")

    recordthings = False
    procs = []; table = [[]];

    procs.append("Source")
    table[0].append("Amount")

    for line in xuanchivo.readlines():
        if "====" in line: recordthings = True
        if recordthings and "|" in line:
            sublines = line.replace(" ", "").split("|")
            procs.append(sublines[0].replace("Background", "") if not sublines[0] in vl.ProcessNameTranslator else vl.ProcessNameTranslator[sublines[0]])
            table[0].append(round(float(sublines[1].split("+-")[0]), 0))
    xuanchivo.close()

    allbkg = 0
    for ind in range(1, len(procs)):
        #print(procs[ind].lower())
        if ("signal"  not in procs[ind].lower() and
            "total"   not in procs[ind].lower() and
            "data"    not in procs[ind].lower()):
            allbkg += table[0][ind]

    for ind in range(len(procs)):
        if "signal" in procs[ind].lower():
            procs.insert(ind, "Total background")
            table[0].insert(ind, round(float(allbkg), 0))
            break

    finallatextab = tb.tabulate(table, headers = procs, tablefmt = "latex_raw")

    substr = "".join(["l"] + ["r" for x in range(len(table[0]) - 1)])
    finallatextab = finallatextab.replace(substr, "".join(["l|"] + ["c|" for x in range(len(table[0]) - 2)] + ["c"] ))

    #print finallatextab
    if not os.path.isdir(path): os.system("mkdir -p " + path)

    name = outname if outname != "" else (varname + "_" + ty)

    outfile = open(path + "/" + outname + ".tex", "w")
    outfile.write(finallatextab)
    outfile.close(); del outfile
    return


def getcondnumLaTeXtable(variables, path = "./results/tables", txttablespath = "./", outname = "", addasimov = False):
    table   = []; headers = []

    if len(variables) == 0: raise RuntimeError("No variables given.")

    # First, obtain the info from the txts
    listofsysts  = []
    listoftuples = []
    filledsysts  = False
    for var in variables:
        tmptupleofcontents = (vl.varList[var]["printname"], [])
        tmpfile = open(txttablespath + "/" + var + ".txt", "r")
        for line in tmpfile.readlines():
            if "Condition" in line: continue
            sublines = line.split("          ")  # TODO: enhance this shit
            if len(sublines) > 1 and not filledsysts:
                thegoodname = sublines[1].replace("\n", "").replace("Up", "").replace("Down", "").replace(" ", "")
                print thegoodname
                if addasimov:
                    listofsysts.append(vl.PrintSysNameTranslator[thegoodname])
                    if thegoodname + "Down" not in vl.varList["Names"]["specialSysts"] and thegoodname != "":
                        listofsysts[-1] += " up" if "Up" in sublines[1] else " down"
                elif "asimov" not in sublines[1]:
                    listofsysts.append(vl.PrintSysNameTranslator[thegoodname])
                    if thegoodname + "Down" not in vl.varList["Names"]["specialSysts"] and thegoodname != "":
                        listofsysts[-1] += " up" if "Up" in sublines[1] else " down"

            if not addasimov and "asimov" not in sublines[1]: tmptupleofcontents[1].append(float(sublines[0]))
            elif addasimov:                                   tmptupleofcontents[1].append(float(sublines[0]))

        listoftuples.append(tmptupleofcontents)
        filledsysts = True
        tmpfile.close();

    listoftuples.insert(0, ("Syst./Variable", listofsysts))

    thedict = OrderedDict(listoftuples)


    #sys.exit()

    finallatextab = tb.tabulate(thedict, headers = "keys", floatfmt = ".2f", tablefmt = "latex_raw")

    substr = "".join(["l"] + ["r" for x in range(len(variables))])
    finallatextab = finallatextab.replace(substr, "".join(["l|"] + ["c|" for x in range(len(variables) - 1)] + ["c"] ))


    print finallatextab
    if not os.path.isdir(path): os.system("mkdir -p " + path)

    name = outname if outname != "" else "condnum"

    outfile = open(path + "/" + name + ".tex", "w")
    outfile.write(finallatextab)
    outfile.close(); del outfile
    return


def getcondnumLaTeXtable(variables, path = "./results/tables", txttablespath = "./", outname = "", addasimov = False):
    table   = []; headers = []

    if len(variables) == 0: raise RuntimeError("No variables given.")

    # First, obtain the info from the txts
    listofsysts  = []
    listoftuples = []
    filledsysts  = False
    for var in variables:
        tmptupleofcontents = (vl.varList[var]["printname"], [])
        tmpfile = open(txttablespath + "/" + var + ".txt", "r")
        for line in tmpfile.readlines():
            if "Condition" in line: continue
            sublines = line.split("          ")  # TODO: enhance this shit
            if len(sublines) > 1 and not filledsysts:
                thegoodname = sublines[1].replace("\n", "").replace("Up", "").replace("Down", "").replace(" ", "")
                print thegoodname
                if addasimov:
                    listofsysts.append(vl.PrintSysNameTranslator[thegoodname])
                    if thegoodname + "Down" not in vl.varList["Names"]["specialSysts"] and thegoodname != "":
                        listofsysts[-1] += " up" if "Up" in sublines[1] else " down"
                elif "asimov" not in sublines[1]:
                    listofsysts.append(vl.PrintSysNameTranslator[thegoodname])
                    if thegoodname + "Down" not in vl.varList["Names"]["specialSysts"] and thegoodname != "":
                        listofsysts[-1] += " up" if "Up" in sublines[1] else " down"

            if not addasimov and "asimov" not in sublines[1]: tmptupleofcontents[1].append(float(sublines[0]))
            elif addasimov:                                   tmptupleofcontents[1].append(float(sublines[0]))

        listoftuples.append(tmptupleofcontents)
        filledsysts = True
        tmpfile.close();

    listoftuples.insert(0, ("Syst./Variable", listofsysts))

    thedict = OrderedDict(listoftuples)


    #sys.exit()

    finallatextab = tb.tabulate(thedict, headers = "keys", floatfmt = ".2f", tablefmt = "latex_raw")

    substr = "".join(["l"] + ["r" for x in range(len(variables))])
    finallatextab = finallatextab.replace(substr, "".join(["l|"] + ["c|" for x in range(len(variables) - 1)] + ["c"] ))


    print finallatextab
    if not os.path.isdir(path): os.system("mkdir -p " + path)

    name = outname if outname != "" else "condnum"

    outfile = open(path + "/" + name + ".tex", "w")
    outfile.write(finallatextab)
    outfile.close(); del outfile
    return


def getgoftestsLaTeXtable(variables, path = "./results/tables", txttablespath = "./", outname = "", ty = "unfolding"):
    table   = []; headers = []

    if len(variables) == 0: raise RuntimeError("No variables given.")

    # First, obtain the info from the txts
    listofsamples = []
    filledsamples = False
    for var in variables:
        tmplistofcontents = [ vl.varList[var]["printnamenodim"] ]
        tmpfile = open(txttablespath + "/" + var + "_goftests_" + ty + ".txt", "r")
        tmpsample = ""
        for line in tmpfile.readlines(): # 1 p-value, 2 test statistic value
            if "/" not in line: continue
            sublines = line.replace(" ", "").split("/")
            if "p-value" in sublines[1]:
                tmplistofcontents.append( str( round(float(sublines[1].split(":")[1]), 3) ) )
                if not filledsamples: listofsamples.append(vl.GOFTranslator[sublines[0]])
            else:
                tmplistofcontents[-1] += " | " + str( round( float(sublines[1].split(":")[1]), 3) )

        table.append(tmplistofcontents)
        filledsamples = True
        tmpfile.close();


    listofsamples.insert(0, "Variable/MC sample")
    #sys.exit()

    finallatextab = tb.tabulate(table, headers = listofsamples, tablefmt = "latex_raw")

    substr = "".join(["l"] + ["l" for x in range(len(listofsamples) - 1)])
    finallatextab = finallatextab.replace(substr, "".join(["l|"] + ["c|" for x in range(len(listofsamples) - 2)] + ["c"] ))


    print finallatextab
    if not os.path.isdir(path): os.system("mkdir -p " + path)

    name = outname if outname != "" else "goftests"

    outfile = open(path + "/" + name +"_" + ty + ".tex", "w")
    outfile.write(finallatextab)
    outfile.close(); del outfile
    return

