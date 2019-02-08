
import ROOT
import os

print "Setting ROOT options"
ROOT.gROOT.SetBatch()
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(10)
ROOT.gStyle.SetOptFit(111111)
ROOT.gStyle.SetOptTitle(1)
ROOT.gStyle.SetPalette(1)
ROOT.gStyle.SetNdivisions(405,"x");
ROOT.gStyle.SetEndErrorSize(0.)
ROOT.gStyle.SetErrorX(0.001)

##########################
# Dictionary Definitions #
##########################
d_massPoints = {}
d_hists = {}
d_tfiles = {}
d_canv = {}
d_f_scale = {}
d_pad = {}

rootFile = ROOT.TFile("../qCalOutputFile.root")

wavelengthHists = {}

# replace with more general form later:
for i in range(0, 8):
    wavelengthHists[i] = ROOT.TH1D()
    histName = "WavelengthPerHit_ID" + str(i)
    print "Histogram name: ", histName
    rootFile.GetObject(histName, wavelengthHists[i])

n = 0
idStr = "id"

for n in range(0, 8):
    d_canv[n] = ROOT.TCanvas("canvasStr1"+ str(n), "canvasStr2"+ str(n), 500, 500)
    d_pad[n] = d_canv[n].GetPad(0)
    d_pad[n].SetLeftMargin(0.15)

    wavelengthHists[n].SetTitle("Wavelength Per Hit: number" + str(n))
    #wavelengthHists[i].Rebin(25) # What the hell

    wavelengthHists[n].SetFillColor(4)
    wavelengthHists[n].SetFillStyle(3002) 

    #d_f_scale[n] = 1/wavelengthHists[n].GetEntries()
    #wavelengthHists[n].Scale(d_f_scale[n])

    wavelengthHists[n].GetXaxis().SetRangeUser(0, 1000)
    wavelengthHists[n].GetXaxis().SetTitle("x-Axis title")
    wavelengthHists[n].GetXaxis().SetTitleSize(0.06)
    wavelengthHists[n].GetXaxis().SetTitleOffset(0.5)

    wavelengthHists[n].GetYaxis().SetRangeUser(0, 100)
    wavelengthHists[n].GetYaxis().SetTitle("y-Axis title")
    #wavelengthHists[n].GetYaxis().SetTitleSize
    wavelengthHists[n].GetYaxis().SetTitleOffset(1.7)

    wavelengthHists[n].DrawCopy()
    d_canv[n].SaveAs("Hists/waveLengthPerHitHists/hist" + str(n) + ".png")





