/***************************************************************
 *
 * file: pss.h
 *
 * @Author  Nikolaos Vasilikopoulos (nvasilik@csd.uoc.gr)
 * @Version 20-10-2020
 *
 * @e-mail       hy240-list@csd.uoc.gr
 *
 * @brief   Implementation of the "pss.h" header file for the Public Subscribe System,
 * function definitions
 *
 *
 ***************************************************************
 */
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "pss.h"


/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 0 on success
 *         1 on failure
 */
int initialize(void){
    return EXIT_SUCCESS;
}

/*fucntion inserts node in sorted double linked list */
void DL_Insert(struct Info **I,int iId,int itm , int igp[MG]){
    struct Info *current;
    struct Info *new_Info = (struct Info*)malloc(sizeof(struct Info));
    current = *I;
    /*initialize new_info*/
    new_Info->iId = iId;
    new_Info->itm = itm;
    for(int i=0;i<MG;i++){
        new_Info->igp[i] = igp[i];
    }
    /*if list is empty or we want to insert before first node*/
    if(current == NULL || (current)->itm >= new_Info->itm){
        *I = new_Info;
        if(current==NULL){
            new_Info->inext = NULL;
        }else{
            new_Info->inext = current;
            current->iprev = new_Info;
        }
        new_Info->iprev = NULL;

    }else{
        
        while(current->inext != NULL && current->inext->itm < new_Info->itm){
            current = current->inext;
        }
        new_Info->inext = current->inext;
        new_Info->iprev = current;
        current->inext = new_Info;
        if(new_Info->inext!=NULL){
            new_Info->inext->iprev = new_Info;
        }
    }
    return;
}

/*fucntion Deletes a node of a sorted double linked list*/
void DL_Delete(struct Info **I,int iId){
    struct Info *current;
    struct Info *prev ;
    prev=*I;
    current = *I;
    while(current!=NULL){
        /*deletion from first node*/
        if(current->iId==iId && current==*I){
            *I = current->inext;
            if(*I!=NULL){
            current->inext->iprev=*I;
            }
            free(current);
            return;
        }else if(current->iId==iId){ /*deletion from any other node*/
            prev->inext = current->inext;
            if(current->inext!=NULL){ //only if its not last node change inext
                current->inext->iprev=prev;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->inext;
    }
    return;
}

/*search for info with specific iId*/
struct Info *DL_LookUp(struct Info **I,int iId){
    struct Info *current;
    current = *I;
    while(current!=NULL){
        if(current->iId==iId){
            return current;
        }
        current = current->inext;
    }
    return NULL;
}
/*print values of a double linked list*/
void DL_Print(struct Info *I){
    struct Info *current;
    current = I;
    printf("INFOLIST = ");
    if(current==NULL){
        printf("Empty  ");
    }
    while(current!=NULL){
        printf(" %d " , current->iId);
        current = current->inext;
    }
    return;
}

/*function inserts node at the start of linked list*/
void L_Insert(struct Subscription **S,int sId){

    struct Subscription *new_sub = (struct Subscription*)malloc(sizeof(struct Subscription));
    /*initialize new_sub*/
    new_sub->sId = sId;
    new_sub->snext = *S;
    *S = new_sub;
    return;
}

/*function deletes node with node.sId==sId from linked list*/
void L_Delete(struct Subscription **S,int sId){
    struct Subscription *current;
    struct Subscription *prev = (struct Subscription*)malloc(sizeof(struct Subscription));
    prev=*S;
    current = *S;
    while(current!=NULL){
        /*deletion from first node*/
        if(current->sId==sId && current==*S){
            *S = current->snext;
            free(current);
            return;
        }else if(current->sId==sId){
            prev->snext = current->snext;
            free(current);
            return;
        }
        prev = current;
        current = current->snext;
    }
    return;
}
/*function searches for node with given sId*/
struct Subscription *L_LookUp(struct Subscription **S,int sId){
    struct Subscription *current;
    current = *S;
    while(current!=NULL){
        if(current->sId==sId){
            return current;
        }
        current = current->snext;
    }
    return NULL;
}
/*function prints out linked list*/
void L_Print(struct Subscription *S){
    struct Subscription *current;
    current = S;
    printf(",SUBLIST = ");
    if(current==NULL){
        printf("Empty ");
    }
    
    while(current!=NULL){
        printf(" %d " , current->sId);
        current = current->snext;
    }
    return;
}

/*function inserts node in sorted linked list*/
void SL_Insert(struct SubInfo **S,int sId,int stm , struct Info *sgp[MG]){
    struct SubInfo *current=*S;
    struct SubInfo *new_SubInfo = (struct SubInfo*)malloc(sizeof(struct SubInfo));
    /*initialize new_SubInfo*/
    new_SubInfo->sId = sId;
    new_SubInfo->stm = stm;
    for(int i=0;i<MG;i++){
        new_SubInfo->sgp[i] = sgp[i];
    }
    
    if(current == NULL || (current)->stm >= new_SubInfo->stm){     
        /*insert node at the start of list*/ 
        *S = new_SubInfo;
        if(current==NULL){
            new_SubInfo->snext = NULL;
        }else{
            new_SubInfo->snext = current;
        }
    }else{
        
        while(current->snext != NULL && current->snext->stm < new_SubInfo->stm){
            current = current->snext;
        }
       
        new_SubInfo->snext = current->snext;
        current->snext = new_SubInfo;
    }
    return;
}

/*function deletes a node with given sId from sorted linked list*/
void SL_Delete(struct SubInfo **S,int sId){
    struct SubInfo *current;
    struct SubInfo *prev = (struct SubInfo*)malloc(sizeof(struct SubInfo));
    prev=*S;
    current = *S;
    while(current!=NULL){
        if(current->sId==sId && current==*S){
            /*delete node from the start*/
            *S = current->snext;
            free(current);
            return;
        }else if(current->sId==sId){
            prev->snext = current->snext;
            free(current);
            return;
        }
        prev = current;
        current = current->snext;
    }
    return;
}

/*function searches for node with given sId in sorted linked list*/
struct SubInfo *SL_LookUp(struct SubInfo *S,int sId){
    struct SubInfo *current;
    current = S;
    while(current!=NULL){
        if(current->sId==sId){
            return current;
        }
        current = current->snext;
    }
    return NULL;
}

/*function prints out sorted linked list*/
void SL_Print(struct SubInfo *S,struct Group G[MG]){
    struct SubInfo *current= S;
    printf("SUBSCRIBERLIST = ");
    if(current==NULL){
        printf("Empty \n");
        return;
    }
    while(current!=NULL){
        printf("%d " , current->sId);
        current = current->snext;
    }
    printf("\n");
    current = S;
    while(current!=NULL){
        printf("SUBSCRIBERID = %d,GROUPLIST = " , current->sId );
        for(int i=0;i<MG;i++){
            if(current->sgp[i]!=1){
                printf("%d " , G[i].gId);
            }
        }
        printf("\n");
        current = current->snext;
    }
    return;
}
/**
 * @brief Subsriber Registration
 *
 * @param sTM Timestamp of arrival
 * @param sId Identifier of subscriber
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Subscriber_Registration(struct SubInfo **S,struct Group G[MG],int sTM,int sId,int* gids_arr,int size_of_gids_arr){
    if(SL_LookUp(*S,sId)!=NULL){ //if susbscriber with that Id exists then exit with 1 
        return 1;
    }

    struct Info *sgp[MG];
    for(int i=0;i<MG;i++){
        sgp[i] = (struct Info*)1; //initialize with value 1 in case it doesnt change later
    }
    for(int i=0;i<size_of_gids_arr-1;i++){
        /*initialize sgp using gids_arr*/
        sgp[gids_arr[i]] = G[gids_arr[i]].gfirst;
        /*insert node in subscription list*/
        L_Insert(&G[gids_arr[i]].ggsub,sId);
    }
    /*insert node in subscriber list*/
    SL_Insert(S,sId,sTM,sgp);

    printf("SUBSCRIBER LIST = ");
    struct SubInfo *current = *S;
    while(current!=NULL){
        printf("%d " , current->sId );
        current=current->snext;
    }
    printf("\n");
    for(int i=0;i<size_of_gids_arr-1;i++){
        printf("GROUPID = %d,SUBLIST = " , gids_arr[i]);
        struct Subscription *curr = G[gids_arr[i]].ggsub;
        while(curr!=NULL){
            printf("%d " , curr->sId);
            curr= curr->snext;
        }
        printf("\n");
    }

    return 0;
}
/**
 * @brief Insert info
 *
 * @param iTM Timestamp of arrival
 * @param iId Identifier of information
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Insert_Info(struct Group G[MG],int iTM,int iId,int* gids_arr,int size_of_gids_arr){
    int igp[MG];
    /*initialize igp*/
    for(int i=0;i<MG;i++){
        if(DL_LookUp(&G[i].gfirst,iId)!=NULL){
            return 1;
        }
        igp[i] = 0;
    }
    for(int i=0;i<size_of_gids_arr-1;i++){
        igp[gids_arr[i]] = 1;
    }
    for(int i=0;i<size_of_gids_arr-1;i++){
        DL_Insert(&G[gids_arr[i]].gfirst,iId,iTM,igp);
        struct Info *glast = G[gids_arr[i]].gfirst;
        while(glast->inext!=NULL){
            glast=glast->inext;
        }
        G[gids_arr[i]].glast = glast;

    }
    for(int i=0;i<size_of_gids_arr-1;i++){
        printf("GROUPID= %d,INFOLIST= " ,gids_arr[i]);
        struct Info *curr = G[gids_arr[i]].gfirst;
        while(curr!=NULL){
            printf("%d ", curr->iId);
            curr = curr->inext;
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}
/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(struct SubInfo **S,struct Group G[MG],int sId){
    struct SubInfo *current = *S;
    if(SL_LookUp(*S,sId)==NULL){
        return 1;
    }
    while(current!=NULL){
        if(current->sId==sId){
            for(int i=0;i<MG;i++){
                if(current->sgp[i]!=1 ){  /*sgp must be !=1 to be able to read from that groups info*/
                    printf("GROUPID= %d ,", i);
                    printf("INFOLIST= ");
                    if(current->sgp[i]==NULL && G[i].glast!=NULL){ /*if info was entered after subscriber registration , update sgp and then read info*/
                        current->sgp[i] = G[i].glast;
                    }else if(current->sgp[i]==NULL && G[i].glast==NULL){ /*if sgp is null and info is null then there is nothing to read */
                        //return 1;
                        break;
                    }
                    while(current->sgp[i]->iprev!=NULL){
                        printf("%d " , current->sgp[i]->iId);
                        current->sgp[i] = current->sgp[i]->iprev;
                    }
                    printf("%d ,NEWSGP= %d \n" , current->sgp[i]->iId , current->sgp[i]->iId );
                }
            }

        }
        current = current->snext;
    }
    printf("\n");
    return EXIT_SUCCESS;
}

/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(struct SubInfo *S,struct Group G[MG]){
    int no_groups=0,no_subs=0;
    for(int i=0;i<MG;i++){
        if(G[i].gfirst!=NULL){
            no_groups+=1; /*count no_groups*/
        }
        printf("GROUP ID = %d, " , G[i].gId);
        DL_Print(G[i].gfirst);
        L_Print(G[i].ggsub);
        printf("\n");
    }
    
    SL_Print(S,G);
    while(S!=NULL){
        no_subs+=1; /*count no_subscribers*/
        S = S->snext;
    }
    printf("NO_GROUPS= %d , NO_SUBSCRIBERS = %d \n" , no_groups , no_subs);

    return EXIT_SUCCESS;
}

/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Delete_Subscriber(struct SubInfo **S,struct Group G[MG],int sId){
    /*chech if subscriber with given sId exists*/
    if(SL_LookUp(*S,sId)==NULL){
        return 1;
    }
    /*first delete subscriber's with given sId subscriptions*/
    for(int i=0;i<MG;i++){
        L_Delete(&G[i].ggsub,sId);
    }
    struct SubInfo *curr = *S;
    printf("SUBSCRIBERLIST= ");
    while(curr!=NULL){
        if(curr->sId!=sId){
            printf("%d " , curr->sId);
        }
        curr = curr->snext;
    }
    printf("\n");
    curr = SL_LookUp(*S,sId);
    for(int i=0;i<MG;i++){
        if(curr->sgp[i]!=1){
            printf("GROUPID= %d" , i);
            L_Print(G[i].ggsub);
            printf("\n");
        }

    }
    /*delete given sId from subscribers list*/
    SL_Delete(S,sId);
    return EXIT_SUCCESS;
}

/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */

int free_all(struct SubInfo **S,struct Group G[MG]){
    struct SubInfo *current = *S;
    while(current!=NULL){
        /*SL_Delete fucntion uses free() for all nodes in subscriber list*/
        SL_Delete(&current,current->sId);
    }
    *S=NULL;
    for(int i=0;i<MG;i++){
        while(G[i].ggsub!=NULL){
            /*L_Delete function uses free() for all nodes in each groups subscription list*/
            L_Delete(&G[i].ggsub,G[i].ggsub->sId);
        }
        while(G[i].gfirst!=NULL){
            /*DL_Delete function uses free() for all nodes in each groups info list*/
            DL_Delete(&G[i].gfirst,G[i].gfirst->iId);
        }
    }
    return EXIT_SUCCESS;
}


int main(){
    struct SubInfo *S = NULL;
	struct Info *I;
	struct Group G[MG];
	int igp[MG];
	for(int i=0;i<MG;i++){
		igp[i]=i;
	}

	for(int i=0;i<MG;i++){
		G[i].ggsub=NULL;
		G[i].gId=i;
		G[i].gfirst = NULL;
		G[i].glast = NULL;
	}

    int gids_arr[5] = {1,2,3,4,-1};

    Subscriber_Registration(&S,G,1,1,gids_arr,5);
    Insert_Info(G,4000,5,gids_arr,5);
    Insert_Info(G,3500,2,gids_arr,5);
    Insert_Info(G,3000,7,gids_arr,5);
    Insert_Info(G,2800,4,gids_arr,5);
    DL_Delete(&G[1].gfirst,4000);
    DL_Delete(&G[1].gfirst,3500);
    DL_Delete(&G[1].gfirst,3000);
    DL_Print(G[0].gfirst);

    Consume(&S,G,1);
}