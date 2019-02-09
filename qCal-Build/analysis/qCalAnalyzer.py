import ROOT 
import os 
import matplotlib.pyplot as plt
import numpy as np
# GOAL: Under James' request, actually move histogram creation here

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
# in the root makeClasses 
for s_runName in l_runNames:
    for i_GeV in d_muonEnergies[s_runName]:

        s_GeV = str(i_GeV)
        #print("SRUNNAME IS : ", s_runName)
        s_count = d_muonCountTags[s_runName]

        # get file e.g. muons_120-GeV_10k.root (consider using os.path.join)
        s_currentFileName = "../muons_"+s_GeV+"-GeV_"+s_count+".root"
        #print "Current file name is: ", s_currentFileName  
        d_tfiles[s_runName, i_GeV] = ROOT.TFile(s_currentFileName)
        currentFile = d_tfiles[s_runName, i_GeV]
        #print "Current file is",  currentFile
        # store tree in dictionary
        d_ttrees[s_runName, i_GeV] = ROOT.TTree()
        currentFile.GetObject("qCal", d_ttrees[s_runName, i_GeV])

def testPrintPhotons(qCalTree):
    if not qCalTree:
        return "ERROR: No tree passed to to your function"
    i_centry = qCalTree.LoadTree
    return 0
######################################################################
# Okay so we have the qCal trees
# qCal Trees have an event number as the Row, then...
# SiPMNumberCount ...= photonCountCount and...
# SiPmNumbers, and photonCounts (vectors)
# the photon counts are not directly important, we really need
# the total number of hits which is the sum of the photonCounts in 
# a given event 
# here's where we start wanting NUMPY
#######################################################################


# WHAT WE ARE ACTUALLY PLOTTING: 
# Number of Hits VS. Number of Events (Normalized to unity, log scale)
# So we must iterate through each event, sum the photonCounts to get
# the total hits for that event. Then get a new array, I'll call it 
# a_eventHits and the index is the event!
# then we simply need to convert this array into a histogram



    # pcounts = bra.GetLeaf("PhotonCounts")

def GetMainBranch(t_qCalTree):
    return t_qCalTree.GetBranch("row_wise_branch")

def GetSingleEventHits(b_branch , i_event):
    # Sets the branch to chosen event: 
    b_branch.GetEntry(i_event)
    i_photonCounts = b_branch.GetLeaf("PhotonCounts")
    i_detectorVolume = i_photonCounts.GetLen()

    totalHits = 0
    for SiPMNumber in range(0, i_detectorVolume):
        totalHits += i_photonCounts.GetValue(SiPMNumber)

    return totalHits 

def GetAllEventHits(b_branch):
    i_eventCount = b_branch.GetEntries()
    a_eventHits = np.zeros(i_eventCount)

    for event in range(0, i_eventCount):
        i_currentHits = GetSingleEventHits(b_branch, event)
        a_eventHits[event] = i_currentHits
    
    return a_eventHits

def invertEventsVsHits(a_eventsVsHits):
    a_nonzeroCopy = a_eventsVsHits[a_eventsVsHits > 0]
    #print "nonzercopy is: ", a_nzeroCopy
    i_outputSize = int(np.amax(a_nonzeroCopy) - np.amin(a_nonzeroCopy))
    #print "i_outputSize is: ", i_outputSize
    a_hitsVsEvents = np.zeros(i_outputSize)

    for N in range(0, i_outputSize):
        a_hasNHits = a_nonzeroCopy[a_nonzeroCopy == N]
        a_hitsVsEvents[N] = a_hasNHits.size

    return a_hitsVsEvents

def plotHistContour(a_input):
    #xData = np.linspace(0, a_input.size, 1)
    plt.plot(a_input)
    plt.show()
    return 0


# testing functions 
myTree = d_ttrees["R00", 1]
myBranch = GetMainBranch(myTree)
myEvent = 0
event0HitsFor1GeV = GetSingleEventHits(myBranch, myEvent)
alleventhitsfor1gev = GetAllEventHits(myBranch)

preHistogram = invertEventsVsHits(alleventhitsfor1gev)

#print "THE PREHISTOGRAM FOR 1GEV TAKES THE FORM: ", preHistogram

#plotHistContour(preHistogram)
# OKAY NOW that we have all the total hits in every event this 
# needs to be transformed into...
# number of events at every hit number
# now the size of the array just depends on the min and max hits over all events 

# Now we can make histograms if needed



# From the hists we can make plots OR we can start from the ntuples and skip hists: 



# Then a plot summarizing the run must be created, like in the DHCal paper: 
# these plots have many details...
# For one the formatting is complicated
# Secondly they must accurately represent the simulation data


