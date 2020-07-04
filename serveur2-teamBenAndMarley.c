#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <math.h>


int main(int argc, char *argv[])
{


    /* Vérification des paramètres */
    if (argc != 2){
        printf("Nombre de paramètres incorrects\n");
        exit(-1);
    }

    //Nouveau port :
    int Numero_port_redirection=8000;
    char nouveau_port[5];    
    sprintf(nouveau_port, "%d", Numero_port_redirection);
   

    //***************************************************************************// SOCKET 1  POUR CONNEXION //******************************************************************
    int socket_serveur_1;
    int sock_len = sizeof(struct sockaddr_in);
    int reuse = 1;
    socket_serveur_1=socket(AF_INET,SOCK_DGRAM,0);

    

    /* Remise à zéro de struct sockaddr_in avant son initialisation */
    struct sockaddr_in my_addr_1; //     
    memset((char*)&my_addr_1, 0, sizeof(my_addr_1));


    // Structure Pour accueillir le client UDP et ses donnees
    struct sockaddr_in structure_reception_1; 
    memset((char*)&structure_reception_1, 0, sizeof(structure_reception_1));

    /* INITIALISATION UDP */
    /* UDP - Init des champs de sockaddr_in */
    my_addr_1.sin_family = AF_INET;
    my_addr_1.sin_port = htons(atoi(argv[1])); //Ici, le port UDP est fixé
    my_addr_1.sin_addr.s_addr = htonl(INADDR_ANY);
    


    /* Creation de la socket UDP */
    if (socket_serveur_1 == -1)
    {
        perror("CreationError : Création de socket UDP impossible\n"); //Affiche le message d'erreur (sans arrêter l'exécution du process)
        return -1; //Arrête l'exécution de la fonction par un retour
    }
    else
    {
        //printf("AFFECTATION SOCKET CORRECTE : DESCRIPTEUR DE FICHIER ASSOCIE N ° < %d > \n", socket_serveur_1);
    }

    
    setsockopt(socket_serveur_1, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    /* BIND ENTRE LA SOCKET ET LA STRUCTURE POUR DONNER UNE SOCKET OPERATIONNELLE */
    int bind_ok_udp;
    bind_ok_udp = bind(socket_serveur_1, (struct sockaddr*)(&my_addr_1), sock_len);
    if (bind_ok_udp < 0){
        perror("BindError : Impossible de lier la socket DATA-UDP\n");
        return(-1);
    }
    else{
        //printf("BIND VALIDE < %d > || SOCKET 1 OPERATIONNELLE ! \n",bind_ok_udp);
    }

    socklen_t len = sizeof(struct sockaddr_in);

    //**************************************************************************// FIN CREATION SOCKET 1//*******************************************************************************


    
    
    
    //************************************************************************** Creations des differents buffers *************************************************************************
    

    char three_way_handshake_support[50];
    memset(three_way_handshake_support, 0, sizeof(three_way_handshake_support));

    char three_way_handshake_support_2[50];
    memset(three_way_handshake_support_2, 0, sizeof(three_way_handshake_support_2));    

    char numero_seq[7];    // j'ai mis une taille de 7 meme si c'est demandé 6 octet sinon je pouvais pas faire la comparaison des string pour les aquitements 
    memset(numero_seq, 0, sizeof(numero_seq));

    char aquitement[10];
    memset(aquitement, 0, sizeof(aquitement));

    char numero_aquitement[7]; // idem ici 
    memset(numero_aquitement, 0, sizeof(numero_aquitement));

    char choix_fichier[100];
    memset(choix_fichier, 0, sizeof(choix_fichier));

    char reception_SYN[]="SYN";
    char emission_SYNACK[] = "SYN-ACK";
    char reception_ACK[]="ACK";
    char FIN_TRANSMISSION[]="FIN";    
    
    //******************************************************************************** FIN DE CREATION DES BUFFERS  ************************************************************************
    
    
    while(1){
        
        
       
        
        //***********************************************************************************
        

        

        
        // Attente de reception du "SYN" de la part du client 
        recvfrom(socket_serveur_1, (char *)three_way_handshake_support,sizeof(three_way_handshake_support),MSG_WAITALL, ( struct sockaddr *) &structure_reception_1,&len);

        if (strcmp(three_way_handshake_support,reception_SYN)==0)
        {
            
          

            //INCREMENTATION DU NUMERO DE PORT:----------------------------------------------------------------------------------------------------------------------------------------
            memset(nouveau_port,0,sizeof(nouveau_port));
            Numero_port_redirection += 1;            
            sprintf(nouveau_port, "%d", Numero_port_redirection);
            //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            //CREATION DE LA SOCKET 2 -------------------------------------------------------------------------------------------------------------------------------------------------

            //**************************************************************************// CREATION SOCKET 2 POUR ECHANGE DE DONNEES //************************************************************

            int socket_serveur_2;
            int sock_len_2 = sizeof(struct sockaddr_in);
            socket_serveur_2=socket(AF_INET,SOCK_DGRAM,0);
            struct sockaddr_in my_addr_2; //     
            memset((char*)&my_addr_2, 0, sizeof(my_addr_2));

            //initialisation structure 2:
            my_addr_2.sin_family = AF_INET;
            my_addr_2.sin_port = htons(atoi(nouveau_port)); 
            my_addr_2.sin_addr.s_addr = htonl(INADDR_ANY);

            /* Creation de la socket UDP */
            if (socket_serveur_2 == -1)
            {
                perror("CreationError : Création de socket UDP impossible\n"); //Affiche le message d'erreur (sans arrêter l'exécution du process)
                return -1; //Arrête l'exécution de la fonction par un retour
            }
            else
            {
                //printf("AFFECTATION SOCKET CORRECTE : DESCRIPTEUR DE FICHIER ASSOCIE N ° < %d > \n", socket_serveur_2);
            }
            setsockopt(socket_serveur_2, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
            
            /* BIND ENTRE LA SOCKET ET LA STRUCTURE POUR DONNER UNE SOCKET OPERATIONNELLE */
            int bind_2;
            bind_2 = bind(socket_serveur_2, (struct sockaddr*)(&my_addr_2), sock_len_2);
            if (bind_2 < 0){
                perror("BindError : Impossible de lier la socket DATA-UDP\n");
                return(-1);
            }
            else{
                //printf("BIND VALIDE < %d > || SOCKET 2 OPERATIONNELLE ! \n \n",bind_2);
            }
            //**************************************************************************// FIN CREATION SOCKET 2 //*********************************************************************************
    
            memset(three_way_handshake_support, 0, sizeof(three_way_handshake_support)); // on vide notre buffer 
            //On va envoyer notre SYN-ACK suivi du numero de port 
            memcpy(three_way_handshake_support,emission_SYNACK, (sizeof(emission_SYNACK)-1)); // on copie dedans la premiere partie syn-ack
            memcpy(three_way_handshake_support+(sizeof(emission_SYNACK)-1),nouveau_port, sizeof(nouveau_port));
            //printf(" %s \n", three_way_handshake_support);
                   
            sendto(socket_serveur_1, (const char *)three_way_handshake_support, sizeof(three_way_handshake_support),MSG_CONFIRM, (const struct sockaddr *) &structure_reception_1,sizeof(structure_reception_1));
            
            


            pid_t pid_fils = fork();           /* On duplique le processus père. */
            if (pid_fils == -1) {              /* Erreur de duplication. */
                perror("fork");           /* Affiche l'erreur sur stderr. */
                exit(EXIT_FAILURE);       /* On quitte dans le processus père. */

            }

            if (pid_fils == 0){  //Si je suis le fils

                close(socket_serveur_1);
                
                 
               
            
            
                // NOTE : JE N'AI PAS DE NOUVELLE STRUCTURE CLIENT POUR LA DEUXIEMME SOCKET : JE GARDE LA PREMIERE SINON L4ENVOI SERVEUR CLIENT NE FONCTIONNE PAS !


            
                
                
                // ...................................................................... ATTENTE DE RECEPTION DU NOM DU FICHIER ..............................................................................
                //printf("Attente du nom de fichier \n ");
                recvfrom(socket_serveur_2,  (char *)choix_fichier,sizeof(choix_fichier),MSG_WAITALL, ( struct sockaddr *) &structure_reception_1,&len);    
                //printf("LANCEMENT DU PROCESSUS D'ENVOI DU FICHIER \n\n");
                //.............................................................................................................................................................................................

                
                
                
                
                // ********************************************************************* OUVERTURE DU FICHIER A ENVOYER *****************************************************************************************
                FILE *descripteur_file;
                // descripteur_file = fopen("image.jpg", "r");
                descripteur_file=fopen(choix_fichier,"rb");
                // descripteur_file=fopen("woods.jpeg","rb");
                if (descripteur_file == NULL) 
                { 
                    printf("Ouverture du fichier impossible  < %s >\n",strerror(errno)); 
                    sendto(socket_serveur_2, (const char *)FIN_TRANSMISSION, sizeof(FIN_TRANSMISSION),MSG_CONFIRM, (const struct sockaddr *)  &structure_reception_1,sizeof(structure_reception_1));
                    exit(0); 
                }
                else
                {
                    //printf("OUVERTURE REUSSIE | LECTURE EN COURS ... \n \n");
                    memset(choix_fichier, 0, sizeof(choix_fichier));
                }
                //************************************************************************************************************************************************************************************************
                
                
               

                //...............................................................BLOC POUR CONNAITRE LA TAILLE DU FICHIER A ENVOYER ..............................................
                int size_file;
                fseek(descripteur_file, 0L, SEEK_END); // calcule la taille de mopn fichier 
                size_file=ftell(descripteur_file);
                fseek(descripteur_file, 0L, SEEK_SET);
                //.................................................................................................................................................................


                //...............................BLOC POUR CALCULER LE NOMBRE DE PAQUETS QU'ON VA DEVOIR ENVOYER EN FONCTION DE LA TAILLE DE DATA UTILE PAR PAQUET......................
                int DATA_SIZE=1466;    
                int NUMBER_OF_PACKETS=(size_file/DATA_SIZE)+1; //nombre de paquet à envoyer 
                int size_of_last_packet=(size_file%DATA_SIZE); // taille du dernier paquet
                //......................................................................................................................................................................
                
                



                // ********************************** CREATION D'UN BUFFER AVEC UN MALLOC POUR S'ADAPTER AUTOMATIQUEMENT A LA TAILLE DU FICHIER A CHARGER  ********************************        
                int i_read;
                char* LECTURE_FICHIER=NULL;
                LECTURE_FICHIER = (char*)malloc(size_file*sizeof(char));

                if (LECTURE_FICHIER == NULL) { 
                    printf("Memory not allocated.\n"); 
                    exit(0); 
                }
                else{
                    memset(LECTURE_FICHIER,0,size_file*sizeof(char));
                    //printf("BUFFER DE LECTURE CREE ET INITIALIZE !! \n");
                }   

                i_read =fread(LECTURE_FICHIER,size_file,1, descripteur_file); // CHARGEMENT DU FICHIER COMPLET EN MEMOIRE 

                //***************************************************************************************************************************************************************************************

                
                
                
                //************************************************* PREPARATION DU TABLEAU DE TABLEAU QUI VA CONTENIR TOUS LES PAQUETS PRET POUR L'ENVOI : CHAQUE CASE FAIT 1024 Bytes !!!!!!****************
                int it;
                char **final_sending_buffer = (char **)calloc(NUMBER_OF_PACKETS ,sizeof(char *));
                if (final_sending_buffer== NULL) { 
                    printf("Memory not allocated with malloc 2.1 .\n"); 
                    exit(0); 
                }
                else{

                    for (it=0; it<NUMBER_OF_PACKETS; it++){
                        final_sending_buffer[it] = (char *)calloc(15000, sizeof(char));
                        if (final_sending_buffer[it]== NULL) { 
                            printf("Memory not allocated with malloc 2.2 .\n"); 
                            exit(0); 
                        }
                    }
                    
                }
                // ************************************************* CHARGEMENT du tableau d'envoi contenant tous les pasquets à envoyer déjà prets avec leur numéro de sequence ******************************
                //******************************************************************************************************************************************************************************************
                int numero_sequence = 0;
                int i;
                int decalage=0;
                for(i=0;i<NUMBER_OF_PACKETS;i++){
                    numero_sequence += 1;
                    sprintf(numero_seq, "%06d", numero_sequence);
                    memcpy(final_sending_buffer[i],numero_seq, 6);
                    memcpy(final_sending_buffer[i]+6,LECTURE_FICHIER+decalage,DATA_SIZE);
                    //printf("[ %d ]> \n",i);
                    
                    decalage=decalage+DATA_SIZE;  // avant j'avais mis +1000 : regardons si ça fonctionne comme ça
                    //TOUT DOUX:rajouter un memset a zero pour le numero_seq!!
                }
                //******************************************************************************************************************************************************************************************
                //****************************************************************************************************************************************************************************************** 
                
                
                free(LECTURE_FICHIER); // On libère le premier tableau qui a servit à tout lire    
                
                //************************************************************************ TIMEOUT *************************************************************
                struct timeval TimeOUT; // creation de la valeur du timeout 
                TimeOUT.tv_sec=0;
                TimeOUT.tv_usec=38000;


                setsockopt(socket_serveur_2,SOL_SOCKET,SO_RCVTIMEO,&TimeOUT,sizeof(TimeOUT));
                

            
                //*************************************************************************
                
                // estimation du RTT : creation des points de depart et d'arrivee !
                struct timespec tstart, tend;
                long int RTT_test_sec;
                long int RTT_test_msec;





                int size_of_sliding_wnd=70;                                           //////////////////////////////C EST ICI SI ON VEUT MODIFIER LA TAILLE DE LA FENETRE GLISSANTE !!!!
                int *pointeur_size_of_sliding_wnd=&size_of_sliding_wnd;
                
                int indice_debut_fenetre=0;
                int *pointeur_indice_debut=&indice_debut_fenetre;

                int indice_fin_fenetre= *pointeur_size_of_sliding_wnd - 1;
                int *pointeur_indice_fin_fenetre=&indice_fin_fenetre;

                int indice_lecture=0;
                int *pointeur_indice_lecture=&indice_lecture;

                int last_ack_received =0;
                int *pointeur_last_ack_received=&last_ack_received;

                int valeur_ack=0;
                int *pointeur_valeur_ack=&valeur_ack;

                //printf("NOUVELLE VALEUR < %d > \n ",indice_debut_fenetre);
                
                int compteur_fast_retransmit=0;
                int *pointeur_compteur_fast_retransmit=&compteur_fast_retransmit;

                int compteur_last_end=0;
                int *pointeur_compteur_last_end=&compteur_last_end;

                int compteur_de_timeout_1=0;
                int *pointeur_compteur_de_timeout_1=&compteur_de_timeout_1;

                int compteur_de_timeout_2=1;
                int *pointeur_compteur_de_timeout_2_max=&compteur_de_timeout_2;

                int compteur_de_timeout_3=0;
                int *pointeur_compteur_de_timeout_3=&compteur_de_timeout_3;

                int compteur_de_FastRT=0;
                int *pointeur_compteur_de_FastRT=&compteur_de_FastRT;

                int variation_FastRT=3;
                int *pointeur_variation_FastRT=&variation_FastRT;

                int seuil_fastRT=(*pointeur_size_of_sliding_wnd-(*pointeur_size_of_sliding_wnd%3))/3;
                int *pointeur_seuil_fastRT=&seuil_fastRT;
                
                int dernier_decalage=0;
                int *pointeur_dernire_decalage=&dernier_decalage;

                int RTT_TIME=0;
                int *pointeur_RTT_TIME=&RTT_TIME;
               
                while (last_ack_received != NUMBER_OF_PACKETS) 
                {
                
                    //printf("\n\n\n PROCHAINE SALVE ENVOI : FENETRE DE [ < %d > ; < %d > ] ET ENVOI REEL DE [ < %d > ; < %d > ] \n",(*pointeur_indice_debut+1),(*pointeur_indice_fin_fenetre+1),(*pointeur_indice_lecture+1),(*pointeur_indice_fin_fenetre+1));
                    while(indice_lecture<=indice_fin_fenetre && *pointeur_dernire_decalage<1){            
                             
                        
                        
                        if(*pointeur_indice_lecture == (NUMBER_OF_PACKETS - 1)){
                            sendto(socket_serveur_2, (const char *)final_sending_buffer[indice_lecture], size_of_last_packet+sizeof(numero_seq)-1,MSG_CONFIRM, (const struct sockaddr *)  &structure_reception_1,sizeof(structure_reception_1));
                            *pointeur_dernire_decalage=*pointeur_dernire_decalage+1;
                        }
                        else{
                            sendto(socket_serveur_2, (const char *)final_sending_buffer[indice_lecture], DATA_SIZE+sizeof(numero_seq)-1,MSG_CONFIRM, (const struct sockaddr *)  &structure_reception_1,sizeof(structure_reception_1));
                            if(*pointeur_indice_lecture==7){
                                clock_gettime(CLOCK_REALTIME, &tstart); //attrape le temps de depart 
                            }

                        }
                        
                        *pointeur_indice_lecture= *pointeur_indice_lecture+1;
                    }

                    

                    // ****************************************************************************** INITIALISATION DU SELECT ********************************************************************************
                    

                    //*****************************************************************************************************************************************************************************************

                    
                    
                    
                    while(*pointeur_last_ack_received >= *pointeur_valeur_ack){

                        
                        
                        while(recvfrom(socket_serveur_2,  (char *)aquitement,sizeof(aquitement),MSG_WAITALL, ( struct sockaddr *) &structure_reception_1,&len)<0){
                            //printf("< RETRANSMISSION SELECT INCORRECT > \n \n");                
                            sendto(socket_serveur_2, (const char *)final_sending_buffer[*pointeur_last_ack_received], DATA_SIZE+sizeof(numero_seq)-1,MSG_CONFIRM, (const struct sockaddr *)  &structure_reception_1,sizeof(structure_reception_1));
                            //sendto(socket_serveur_2, (const char *)final_sending_buffer[*pointeur_last_ack_received +1], DATA_SIZE+sizeof(numero_seq)-1,MSG_CONFIRM, (const struct sockaddr *)  &structure_reception_1,sizeof(structure_reception_1));
                            *pointeur_compteur_de_timeout_1=*pointeur_compteur_de_timeout_1+1;
                            *pointeur_compteur_de_timeout_3=*pointeur_compteur_de_timeout_3+1;
                            

                        }
                        if(*pointeur_compteur_de_timeout_2_max < *pointeur_compteur_de_timeout_1){
                            *pointeur_compteur_de_timeout_2_max=*pointeur_compteur_de_timeout_1;
                        }
                        *pointeur_compteur_de_timeout_1=0;
                        //TimeOUT.tv_usec=400*(*pointeur_compteur_de_timeout_2_max);
                        /*if(TimeOUT.tv_usec>8000){
                            TimeOUT.tv_usec=1000;
                        }*/

                        //compteur_de_timeout_1=1;       
                        memcpy(numero_aquitement,aquitement+sizeof(reception_ACK)-1, sizeof(numero_aquitement));  //* retrait du -1 pour le /0 + une taille de 7 pour le buffer             
                        *pointeur_valeur_ack=atoi(numero_aquitement);
                        //printf("VALEUR DE ACK RECU : < %d > \n ",*pointeur_valeur_ack);
                        memset(numero_aquitement,0,sizeof(numero_aquitement));
                        memset(aquitement,0,sizeof(aquitement));

                        if(*pointeur_valeur_ack==7){
                            clock_gettime(CLOCK_REALTIME, &tend); //attrape le temps d'arrivée
                            //printf("RTT = < %ld > ns || < %ld > s \n ",(tend.tv_nsec - tstart.tv_nsec),(tend.tv_sec - tstart.tv_sec)); // difference des deux temps , estimation du rtt en nanosecondes.
                            *pointeur_RTT_TIME=((tend.tv_sec - tstart.tv_sec)*pow(10,6))+(abs((tend.tv_nsec - tstart.tv_nsec)))*pow(10,-3);
                            TimeOUT.tv_usec=*pointeur_RTT_TIME;
                        }
                        
                        if(*pointeur_last_ack_received == *pointeur_valeur_ack ){
                            *pointeur_compteur_fast_retransmit=*pointeur_compteur_fast_retransmit+1;
                        }
                        //printf("VALEUR FAST_RT:  %d/3 \n",*pointeur_compteur_fast_retransmit);
                        if(*pointeur_compteur_fast_retransmit >=3){
                            //printf("FAST RETRANSMIT NECESSAIRE ! \n");
                            sendto(socket_serveur_2, (const char *)final_sending_buffer[*pointeur_last_ack_received], DATA_SIZE+sizeof(numero_seq)-1,MSG_CONFIRM, (const struct sockaddr *)  &structure_reception_1,sizeof(structure_reception_1));
                            *pointeur_compteur_fast_retransmit=0;   // une fois que le fast retransmit est réalisé on reinitialise sa valeur    

                            if (*pointeur_variation_FastRT >= *pointeur_seuil_fastRT){
                                *pointeur_variation_FastRT =0;
                            }
                            *pointeur_variation_FastRT=*pointeur_variation_FastRT+1;
                            *pointeur_compteur_de_FastRT=*pointeur_compteur_de_FastRT+1;        
                        }
                        
                        
                    }
                    
                    *pointeur_variation_FastRT = 3;

                    *pointeur_last_ack_received=*pointeur_valeur_ack;
                    *pointeur_compteur_fast_retransmit=0; // on annule pour pas qu'on ai des additions entre valeurs intermediaires et donc rajouter des fastrt pour rien
                    //printf("==> LAST ACK RECEIVED < %d > \n", last_ack_received);

                    *pointeur_indice_debut=last_ack_received;
                    *pointeur_indice_lecture=*pointeur_indice_fin_fenetre + 1; // l'indice de lecture se positionne tjs 1 indice plus loin de lu l'indice du dernier paquet envoyé 
                    *pointeur_indice_fin_fenetre= last_ack_received + (*pointeur_size_of_sliding_wnd - 1);

                    if(*pointeur_indice_fin_fenetre >  (NUMBER_OF_PACKETS - 1)){
                        *pointeur_indice_fin_fenetre=NUMBER_OF_PACKETS-1;
                        
                    }
                    if(*pointeur_indice_lecture >=(NUMBER_OF_PACKETS - 1)){
                        *pointeur_indice_lecture=(NUMBER_OF_PACKETS - 1);
                    }

                    memset(numero_seq, 0, sizeof(numero_seq));
                    memset(aquitement, 0, sizeof(aquitement));
                    memset(numero_aquitement, 0, sizeof(numero_aquitement));

                    //sleep(1);               

                }
                
                    
                fclose(descripteur_file);
                //printf("TRANSFERT FICHIER COMPLET ! OPERATION TERMINEE \n\n");
                free(final_sending_buffer);
                //printf("FREE COMPLETE\n");
                sendto(socket_serveur_2, (const char *)FIN_TRANSMISSION, sizeof(FIN_TRANSMISSION),MSG_CONFIRM, (const struct sockaddr *)  &structure_reception_1,sizeof(structure_reception_1));
                
                
                //printf("taille du fichier < %d > \n",size_file);    
                //printf("nb de paquet à envoyer < %d > \n", NUMBER_OF_PACKETS);
                //printf("La taille de chaque paquet = %d \n", DATA_SIZE);
                //printf("La taille du dernier paquet est  < %d > \n", size_of_last_packet);
                //printf("Le nb de TIMEOUT_max < %d >\n\n",*pointeur_compteur_de_timeout_2_max);
                
                
                //printf("NB DE FAST RETRANSMIT < %d > \n",*pointeur_compteur_de_FastRT);
                //printf("Le nb de TIMEOUT_TOTAL < %d >\n",*pointeur_compteur_de_timeout_3);
                //printf(" TIMEOUT = <%ld> µs \n",TimeOUT.tv_usec);
                close(socket_serveur_2);
                exit(0);  // pour arreter le process quand il a terminé 

            }
            else{ //SI JE SUIS LE PERE 

                // On va attendre maintenant la reception de l'ack du client:
                recvfrom(socket_serveur_1, (char *)three_way_handshake_support_2, sizeof(three_way_handshake_support_2),MSG_WAITALL, ( struct sockaddr *) &structure_reception_1,&len);
                 
                
                
                //on va maintenant regarder si on a bien recu le ACK du client:
                if (strcmp(three_way_handshake_support_2,reception_ACK)==0)
                {
                    //printf("THREE WAY HANDSHAKE REUSSI | < CONNEXION VALIDE > |\n\n");
                    memset(three_way_handshake_support_2, 0, sizeof three_way_handshake_support_2);
                    memset(three_way_handshake_support, 0, sizeof(three_way_handshake_support)); // pas très utile mais bon
                }
                else
                {
                    printf(" THREE WAY HANDSHAKE REFUSE | RAISON ( ACK NON RECU )\n");
                    kill(pid_fils, SIGKILL);
                }
                

                close(socket_serveur_2);               
                
            }

        }
        else
        {
            printf(" THREE WAY HANDSHAKE REFUSE ! RAISON : PAS DE < SYN > RECU \n ");
            memset(three_way_handshake_support, 0, sizeof(three_way_handshake_support));
        }
        
        
    }
    return(0);
}