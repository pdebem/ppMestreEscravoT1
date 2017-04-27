#!/bin/bash
git add README.md mestreEscravo.c *.sh saidas/* masterSlave.c sequential.c
	if [ $# -eq 0 ] 
	then
		git commit -m "Without comment!"
	else
		res='"'
		res+="$@"
		res+='"'
		git commit -m "$res"
	fi
git push origin master
