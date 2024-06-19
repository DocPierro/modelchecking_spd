idiot:
	cp controleur_idiot.h controleur.h

simple:
	cp controleur_simple.h controleur.h

esperance:
	Cosmos -g moded_CoSimulation.grml esperance.lha --width 50 --use-setdomain-impl --njob 6 --batch 20 --max-run 2000000

propcoll:
	Cosmos -g moded_CoSimulation.grml propCollision.lha --width 0.1 --use-setdomain-impl --njob 6 --batch 20 --max-run 2000000

esperanceI: idiot esperance

esperanceS: simple esperance

propcollI: idiot propcoll

propcollS: simple propcoll
	
