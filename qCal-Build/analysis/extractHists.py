import ROOT as r
from string import Template
from array import array
import matplotlib.pyplot as plt
import numpy as np
import os
import sys


# sys.argv[0] is the name of the file 
# $1 -> particle name (string), e.g. "pions", "muons", "electrons"
# $2 -> energy (integer), e.g 1, 2, 3, 50 100, etc. 
# $3 -> units (string), e.g. MeV, GeV
# $4 -> run (integer), e.g. 1, 2, 3, 10

particle = sys.argv[1]
energy   = sys.argv[2]
units    = sys.argv[3]

hists = []
for i in range(1, 11):
    newHist =  r.TH1D("PhotonCountHist" + str(i), "PhotonCountsPerEvent", 100*100, 0., 100*100*1000)
    hists.append(newHist)

newFiles = []

for i in range(1, 11):
    outF = r.TFile(particle + "_" + str(energy) + "-" + units + "_1000_" + str(i) + "x.root","RECREATE")   # name of the extracted file 
    newFiles.append(outF)

fileNames = []

for i in range(1, 11):
    name = particle + "_" + str(energy) + "-" + units + "_1000_" + str(i) + ".root"   # name of the original file
    print("Filename is: ", name)
    fileNames.append(name)

for i in range(0, 10):
    f = fileNames[i]
    processLine = '.x analyzeCal.C+("{0}")'.format(f) 
    r.gROOT.ProcessLine(processLine)  
    hists[i] =  r.gROOT.FindObject("PhotonCountHist")      
    hists[i].SetDirectory(0)
    newFiles[i].cd()                        
    hists[i].Write()    # After running, analyzeCal.C returns a TH1F
    newFiles[i].Write()
    newFiles[i].Close()

#raw_input()  # This keeps the plots on the screen until you hit enter.

