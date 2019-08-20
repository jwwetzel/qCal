import ROOT 
import os 
import matplotlib.pyplot as plt
import numpy as np

# move histogram creation here

# Dictionary/List definitions
d_muonEnergies = {}
d_tfiles = {}
d_ttrees = {}
d_runs = {}
l_runNames = [""]
d_muonCounts = {}
d_muonCountTags = {}
# Store Muon Energies from the Nth macro file, key "RN": 
l_runNames[0] = "R00"
d_muonEnergies["R00"] = [1, 25, 120]
d_muonCountTags["R00"] = "10k"
d_muonCounts["R00"] = 10e4 # tags should probably look like this 
# Getting all root TFiles and their "qCal" TTrees

# The for loop below is doing roughly the same thing as the constructor
# in the root makeClasses for C
for s_runName in l_runNames:
    for i_GeV in d_muonEnergies[s_runName]:

        s_GeV = str(i_GeV)
        #print("SRUNNAME IS : ", s_runName)
        s_count = d_muonCountTags[s_runName]

        # get file e.g. muons_120-GeV_10k.root (consider using os.path.join)
        s_currentFileName = "../muons_"+s_GeV+"-GeV_"+s_count+".root"
        #s_currentFileName = "../qCalOutputFile.root"
        #s_currentFileName = "../myTest.root"
        #print "Current file name is: ", s_currentFileName  
        d_tfiles[s_runName, i_GeV] = ROOT.TFile(s_currentFileName)
        currentFile = d_tfiles[s_runName, i_GeV]
        #print "Current file is",  currentFile
        # store tree in dictionary
        d_ttrees[s_runName, i_GeV] = ROOT.TTree()
        currentFile.GetObject("qCal", d_ttrees[s_runName, i_GeV])

def GetMainBranch(t_qCalTree):
    s_branchName  = "PhotonCounts"
    return t_qCalTree.GetBranch(s_branchName)

def GetSingleEventHits(t_qCalTree, i_event):
    b_branch = GetMainBranch(t_qCalTree)
    v_photonCounts = ROOT.std.vector('double')(27)
    t_qCalTree.GetEntry(i_event)
    t_qCalTree.SetBranchAddress('PhotonCounts', ROOT.AddressOf(v_photonCounts))
    i_detectorVolume = v_photonCounts.size()

    totalHits = 0

    for SiPMNumber in range(0, i_detectorVolume):
        totalHits += v_photonCounts.at(SiPMNumber)

    return totalHits 

def GetAllEventHits(t_qCalTree):
    b_branch = GetMainBranch(t_qCalTree)
    i_eventCount = b_branch.GetEntries()
    a_eventHits = np.zeros(i_eventCount)

    print "Event Count: ", i_eventCount

    for event in range(0, i_eventCount):
        i_currentHits = GetSingleEventHits(t_qCalTree, event)
        a_eventHits[event] = i_currentHits
    print "Total Hits: ", a_eventHits
    return a_eventHits

def makeAndPlotHistogram(a_input):
    a_nonzero = a_input[a_input > 0.]
    
    plt.hist(a_nonzero, bins= 200)
    plt.show()
    return 0

def makeThreeHistograms(a1, a2, a3):
    a1 = a1[a1 > 0.]
    print "Total Hits: ", a1
    a2 = a2[a2 > 0.]
    print "Total Hits: ", a1
    a3 = a3[a3 > 0.]
    print "Total Hits: ", a1
    
    plt.title("Photon hits per Event (-x 11 -y 11 -z 11 -w 1 -a Fe, 500 events, 125 bins) ")
    bn = 125
    al = 0.5
    plt.xlim(0, 5000)
    plt.gca().set_yscale("log")
    plt.hist(a1, bins= bn, alpha=al, label='1 GeV', normed = True)
    plt.hist(a2, bins= bn,alpha=al, label='25 GeV', normed = True)
    plt.hist(a3, bins= bn,alpha=al, label='120 GeV', normed = True)
    plt.legend(loc='upper right')
    plt.show()

"""
# testing functions 

d_arr = {1: np.array([]), 25: np.array([]), 120: np.array([])}

for i_GeV in d_muonEnergies["R00"]:
    myTree = d_ttrees["R00", i_GeV]
    d_arr[i_GeV] = GetAllEventHits(myTree)
    
    #makeAndPlotHistogram(myHits)

makeThreeHistograms(d_arr[1], d_arr[25], d_arr[120])

"""

