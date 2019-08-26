# This script is placed in the folder of the root files, with 'extractHists.py', 'analyzeCal.py', 'analyzeCal.C'
# It extracts the photon count data from root histograms into a csv file

# Args: 
# $1 -> particle name (string), e.g. "pions", "muons", "electrons"
# $2 -> units (string), e.g. MeV, GeV

# EXAMPLES: ./analyzeCal.sh muons GeV
#           ./analyzeCal.sh mu GeV
#           ./analyzeCal.sh pions MeV

rm *10k.root* # remove any already combined files 
rm *.csv      # remove output csv file 

for i in 1 2 3 4 5 6 7 8 9 10 15 20 25 30 40 50 60 70 80 90 100 150 200 250 300 350 400 450 500 750 1000   # Enter energies to search for here: 
do
	 python extractHists.py $1 $i $2	# Extract the photon count histograms from the original root files
	 hadd $1_$i-$2_10k.root $1_$i-$2_1000_*x.root  # Add the histograms from the extracted files into a single file
	 rm *x.root # remove the extracted files
done 

python analyzeCal.py $1 $2 # create the csv file from the added histograms


