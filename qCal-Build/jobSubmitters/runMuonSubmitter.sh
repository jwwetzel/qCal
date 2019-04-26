### Run Jobs for qCal
### Contact: james-wetzel@uiowa.edu
###
### $1 = number of events

for E in 1 2 3 4 5 6 7 8 9 10 15 20 25 30 40 50 60 70 80 90 100 150 200 250 300 350 400 450 500 750 1000
do
	if [ "$E" -eq 30 ] || [ "$E" -eq 40 ] || [ "$E" -eq 70 ]  || [ "$E" -eq 90 ]  || [ "$E" -eq 200 ] || [ "$E" -eq 400 ]  || [ "$E" -eq 15 ]  || [ "$E" -eq 1000 ]
	then
		./muonSubmitter.sh muons $E /Users/jwwetzel/geant4Simulations/qCal/qCal-Build $1
		#continue
	fi
#	./muonSubmitter.sh muons $E /Users/jwwetzel/geant4Simulations/qCal/qCal-Build $1
done
