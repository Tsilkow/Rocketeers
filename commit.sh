#!/bin/bash

projectName="Rockteers";
projectColor="\E[1m\E[91m\E[47m";

itemName[1]="tom";
attributes[1]="\E[1m\E[94m";

itemName[2]="arrow";
attributes[2]="\E[1m\E[35m";

itemName[3]="actor";
attributes[3]="\E[1m\E[93m";

itemName[4]="signal";
attributes[4]="\E[1m\E[97m";

itemName[5]="ray";
attributes[5]="\E[1m\E[91m";

itemName[6]="vessel";
attributes[6]="\E[1m\E[96m";

itemName[7]="planet";
attributes[7]="\E[1m\E[33m";

itemName[8]="init";
attributes[8]="\E[1m\E[97m";


function commitLibrary
{
	 echo -e "${attributes[$1]}${itemName[$1]}\E[0m";
	 git add "${itemName[$1]}.cpp";
	 git add "${itemName[$1]}.hpp";
}

clear
echo -ne "\e[3J"

len=${#itemName[@]}+1

for (( i=0; i<$len; i++ ))
do
	 commitLibrary $i;
done

echo -e "\E[1mbuild.sh\E[0m";
git add build.sh;

echo -e "\E[1mcommit.sh\E[0m";
git add commit.sh;

echo -e "$projectColor""main.cpp\E[0m";
git add main.cpp;

echo -e "\E[1m\E[37m\E[105mCommiting.\E[0m";
git commit;

echo -e "\E[1m\E[97m\E[42m$projectName commited successfully.\E[0m";
