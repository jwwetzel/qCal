### Run Jobs for qCal
### Contact: james-wetzel@uiowa.edu
###
<<<<<<< HEAD
### $1 = number of events


for E in 1 2 3 4 5 6 7 8 9 10 15 20 25 30 40 50 60 70 80 90 100 150 200 250 300 350 400 450 500 750 1000
do
#	if [ "$E" -eq 30 ] || [ "$E" -eq 40 ] || [ "$E" -eq 70 ]  || [ "$E" -eq 90 ]  || [ "$E" -eq 200 ] || [ "$E" -eq 400 ]  || [ "$E" -eq 15 ]  || [ "$E" -eq 1000 ]
#	then
#		./muonSubmitter.sh muons $E /Users/jwwetzel/geant4Simulations/qCal/qCal-Build $1
#		continue
#	fi

	if [ ! -d /nfsscratch/Users/jwwetzel/$E ]
	then
    		mkdir /nfsscratch/Users/jwwetzel/$E
        	wait
	fi

	qsub -V -j y -q INFORMATICS muonSubmitter.job muons $E /Users/jwwetzel/geant4Simulations/qCal/qCal-Build $1
=======
### $1 = number of events per 
### $2 = number of times repeated 
for E in 1 2 3 4 5 6 7 8 9 10 15 20 25 30 40 50 60 70 80 90 100 150 200 250 300 350 400 450 500 750 1000
	for seed in {1..$2}
		do
			if [ "$E" -eq 2 ] || [ "$E" -eq 400 ] || [ "$E" -eq 60 ]
			then
				continue
			fi
			./muonSubmitter.sh muons $E /Users/jwwetzel/geant4Simulations/qCal/qCal-Build $1 $2
>>>>>>> f20554b5f8d70e6a6ea2c9645c43eef1b602d5d6
done
