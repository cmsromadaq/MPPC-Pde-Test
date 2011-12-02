#! /bin/bash
# Simple script to measure MPPC
#
#	by Rene Rios rrios@elo.utfsm.cl
#

#voltage steps
steps=0.1
range=1
initial_delay=300
delay=120
measures=20
Nmed=1
voltage=0.0
events=10000
dir=../photoelectron

#data file
mppc_data=mppc_voltages.dat
mppc=(`awk '{print $1}' $mppc_data`)
voltages=(`awk '{print $2}' $mppc_data`)

function Measure {
	for i in $(seq 1 $measures); do
		echo measurement number $i of voltage $voltage
		../ksm/ksm -v $voltage -p 1 &
		for j in $(seq 1 $Nmed); do
			if [ $j -eq 1 ] && [ $i -eq 1 ] ; then
				./spectra $events $dir/test_mppc_$mppc"_"$voltage"_"$j.dat $initial_delay
			else
				./spectra $events $dir/test_mppc_$mppc"_"$voltage"_"$j.dat $delay
			fi
		done
		voltage=`echo $voltage + $steps|bc`
	done
}


echo "Measurement of MPPC will Start if ON is Selected"
OPTIONS="START OFF EXIT"
indice=0
select opt in $OPTIONS; do
	if [ "$opt" = "START" ]; then
		for i in ${mppc[*]} ; do
		voltage=`echo ${voltages[$indice]} - $range|bc`
		echo starting measurements of mppc $i voltage $voltage
			Measure
			indice=`expr $indice+1`
			echo now insert mppc ${mppc[$indice]}
			read -p "Press any key to continue measurement after installing MPPC..." 
		done
	elif [ "$opt" = "OFF" ]; then
		echo turning OFF Source Meter
		../ksm/ksm -p 0
	elif [ "$opt" = "EXIT" ]; then
		clear		
		echo Bye.
		exit
	else
		clear
		echo Bad option, 1=START , 2=OFF , 3=EXIT
	fi
done




