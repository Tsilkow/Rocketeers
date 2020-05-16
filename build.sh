#!/bin/bash

projectName="Rockteers";
projectColor="\E[1m\E[91m\E[47m";
compilatorAndFlags="g++ -std=c++17";
gdbFlag="-g -ggdb -gstabs";
inputAndOutput="2>&1";
finalCompilation="main.cpp -o Rockteers libraries.a -lsfml-graphics -lsfml-system -lsfml-window -lsfml-audio";

itemName[1]="tom";
attributes[1]="\E[1m\E[94m";
itemCompilation[1]="-c tom.cpp -lsfml-graphics -lsfml-system -lsfml-window -lsfml-audio";

itemName[2]="actor";
attributes[2]="\E[1m\E[93m";
itemCompilation[2]="-c actor.cpp -lsfml-graphics -lsfml-system -lsfml-window -lsfml-audio";

itemName[3]="signal";
attributes[3]="\E[1m\E[97m";
itemCompilation[3]="-c signal.cpp -lsfml-graphics -lsfml-system -lsfml-window -lsfml-audio";

itemName[4]="ray";
attributes[4]="\E[1m\E[91m";
itemCompilation[4]="-c ray.cpp -lsfml-graphics -lsfml-system -lsfml-window -lsfml-audio";

itemName[5]="vessel";
attributes[5]="\E[1m\E[96m";
itemCompilation[5]="-c vessel.cpp -lsfml-graphics -lsfml-system -lsfml-window -lsfml-audio";

itemName[6]="init";
attributes[6]="\E[1m\E[97m";
itemCompilation[6]="-c init.cpp -lsfml-graphics -lsfml-system -lsfml-window -lsfml-audio"

function compileLibrary
{
	 echo -e "${attributes[$1]}${itemName[$1]}\E[0m";
	 if ! eval $compilatorAndFlags ${itemCompilation[$1]} $inputAndOutput ; then exit; fi
	 ar rvs "libraries.a" "${itemName[$1]}.o";
}

clear
echo -ne "\e[3J"

for var in "$@"
do
	 if [ $var -eq -1 ]; then
		  compilatorAndFlags="$compilatorAndFlags $gdbFlag";
	 elif [ $var -eq -2 ]; then
		  echo -e "\E[1m\E[97m\E[46mStarting full compilation:\E[0m";
		  len=${#itemName[@]}

		  for (( i=1; i<=$len; i++ ))
		  do
				compileLibrary $i;
		  done

		  echo -e "\E[1m\E[97m\E[42mAll dependencies have been compiled succesfully.\E[0m";
		  
	 elif [ $var -gt 0 ]; then
		  compileLibrary $var;
	 fi
done
echo -e "\E[1m\E[97m\E[42mAll requested dependencies have been compiled succesfully.\E[0m";
	 
echo -e "\E[1m\E[97m\E[44mStarting compilation of $projectName.\E[0m";
if eval $compilatorAndFlags $finalCompilation $inputAndOutput; then
	 echo -e "\E[1m\E[97m\E[42m$projectName compiled successfully.\E[0m";
	 echo -e "$projectColor""Launching $projectName.\E[0m";
	 eval "./$projectName";
fi
