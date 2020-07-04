all: serveur1-teamBenAndMarley serveur2-teamBenAndMarley serveur3-teamBenAndMarley

serveur1-teamBenAndMarley: serveur1-teamBenAndMarley.c 
	gcc serveur1-teamBenAndMarley.c -o serveur1-teamBenAndMarley

serveur2-teamBenAndMarley: serveur2-teamBenAndMarley.c 
	gcc serveur2-teamBenAndMarley.c -o serveur2-teamBenAndMarley

serveur3-teamBenAndMarley: serveur3-teamBenAndMarley.c 
	gcc serveur3-teamBenAndMarley.c -o serveur3-teamBenAndMarley

clean:
	rm -f serveur1-teamBenAndMarley 
	rm -f serveur2-teamBenAndMarley
	rm -f serveur3-teamBenAndMarley