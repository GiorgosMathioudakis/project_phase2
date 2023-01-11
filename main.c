/***************************************************************
 *
 * file: pss.h
 *
 * @Author  Nikolaos Vasilikopoulos (nvasilik@csd.uoc.gr), John Petropoulos (johnpetr@csd.uoc.gr)
 * @Version 30-11-2022
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
#include <ctype.h>
#include "pss.h"
#define BUFFER_SIZE 1024 /* Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints
 * or comment to disable it */
#define DEBUG
#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */


int a,b,p,m;
struct SubInfo* Hash_arr[10];
struct Group G[MG];

int Isleaf(struct TreeInfo *root){
    if(root->tlc==NULL && root->trc==NULL){
        return 1;
    }
    return 0;
}

/*function inserts node in sorted linked list*/
void SL_Insert(struct SubInfo **S,int sId,int stm , struct TreeInfo *sgp[MG],struct TreeInfo *tgp[MG]){
    struct SubInfo *current=*S;
    struct SubInfo *new_SubInfo = (struct SubInfo*)malloc(sizeof(struct SubInfo));
    /*initialize new_SubInfo*/
    new_SubInfo->sId = sId;
    new_SubInfo->stm = stm;
    for(int i=0;i<MG;i++){
        new_SubInfo->sgp[i] = sgp[i];
        new_SubInfo->tgp[i] = tgp[i];
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

void LO_BST_Print(struct TreeInfo* root){
    while(root->tlc!=NULL){
        root=root->tlc;
    }
    while(root!=NULL){
        printf("%d " , root->tId);
        root = root->next;
    }
}

/*no_null is a integer that counts how many of the hash table's cells are empty */
int no_null =0;
/*function prints out sorted linked list*/
void SL_Print(struct SubInfo *S,struct Group G[MG]){
    struct SubInfo *current= S;
    //printf("SUBSCRIBERLIST = ");
    if(current==NULL){
        no_null++;
        if(no_null==10){
            printf("Empty \n");
        }
        return;
    }
    while(current!=NULL){
        printf("%d " , current->sId);
        current = current->snext;
    }
    printf("\n");
    current = S;
    while(current!=NULL){
        printf("SUBSCRIBERID = %d,GROUPLIST = \n" , current->sId );
        for(int i=0;i<MG;i++){
            if(current->tgp[i]!=1){
                printf("%d ,TREELIST= " , G[i].gId);
                LO_BST_Print(S->tgp[i]);
            }
        }
        printf("\n");
        current = current->snext;
    }
    return;
}

int Universal_Hash_Function(int key,int m,int p){
    return (((a*key)^b)%p)%m; 
}

void HT_Insert(int sId,int stm,struct TreeInfo* sgp[MG],struct TreeInfo* tgp[MG]){
    int pos = Universal_Hash_Function(sId,m,p);
    SL_Insert(&Hash_arr[pos],sId,stm,tgp,sgp);
}

void HT_Delete(int sId){
    int pos = Universal_Hash_Function(sId,m,p);
    SL_Delete(&Hash_arr[pos],sId);
}

struct SubInfo *HT_LookUp(int sId){
    int pos = Universal_Hash_Function(sId,m,p);
    SL_LookUp(Hash_arr[pos],sId);
}

void HT_Print(){
    printf("SUBSCRIBERLIST= ");
    for(int i=0;i<m;i++){
        //SL_Print(Hash_arr[i],G);
        struct SubInfo* head = Hash_arr[i];
        while(head!=NULL){
            printf("%d " , head->sId);
            head = head->snext;
        }
    }
    for(int i=0;i<m;i++){
        SL_Print(Hash_arr[i],G);
    }
    printf("\n");

}


/*this fucntion returns the previous node of the given node in the linked list */
struct TreeInfo* prevLeaf(struct TreeInfo *node){
    struct TreeInfo *temp = node;
    while(temp->tp!=NULL && temp==temp->tp->tlc){
        temp = temp->tp;
    }
    if(temp->tp==NULL){
        return NULL; /*there is no left subtree compared to the node we got*/
    }
    temp = temp->tp;
    temp = temp->tlc;
    while(temp->trc!=NULL){
        temp = temp->trc;
    }
    return temp;
}

void LO_BST_Insert(struct TreeInfo **root,int tId,int ttm,struct TreeInfo *tp){
    if(*root==NULL){ /*empty tree*/
        struct TreeInfo *new_Info = (struct TreeInfo*)malloc(sizeof(struct TreeInfo)) ;
        new_Info->tId = tId;
        new_Info->ttm = ttm;
        new_Info->tp = tp;
        new_Info->tlc = NULL;
        new_Info->trc = NULL;
        new_Info->next = NULL;
        *root = new_Info;
        return;
    }
    struct TreeInfo *current=*root;        
    if(current->tlc==NULL && current->trc==NULL){ /*not empty tree and leaf */
        struct TreeInfo *new_Info = (struct TreeInfo*)malloc(sizeof(struct TreeInfo)) ;
        new_Info->tId = tId;
        new_Info->ttm = ttm;
        new_Info->tlc = NULL;
        new_Info->trc = NULL;
        new_Info->next = NULL;
        if(current->ttm>ttm){ /*node is min of (current,node)*/
            struct TreeInfo* temp = (struct TreeInfo*)malloc(sizeof(struct TreeInfo));
            temp->tId = current->tId;
            temp->ttm = current->ttm;
            temp->tp = current;
            temp->next = NULL;
            temp->tlc = NULL;
            temp->trc = NULL;
            current->tId = tId;
            current->ttm = ttm;
            current->tlc = new_Info;
            new_Info->tp = current;
            current->trc = temp;
            current->tlc->next = current->trc;
            current->trc->next = current->next;
            current->next = NULL;
            if(prevLeaf(current->tlc)!=NULL){
                prevLeaf(current->tlc)->next = current->tlc;
            }
            return;
        }else if(current->ttm<ttm){ /*current is min of (current,node)*/
            struct TreeInfo *new = (struct TreeInfo*)malloc(sizeof(struct TreeInfo));
            new->tp = current;
            new->tId = current->tId;
            new->ttm = current->ttm;
            new->tlc = NULL;
            new->trc = NULL;
            current->tlc = new;
            current->trc = new_Info;
            new_Info->tp= current;
            current->tlc->next = current->trc;
            current->trc->next = current->next;
            current->next= NULL;
            if(prevLeaf(current->tlc)!=NULL){
                prevLeaf(current->tlc)->next = current->tlc;
            }
            return;
        }
        return;
    }   
    if(current->ttm<ttm){
        LO_BST_Insert(&current->trc,tId,ttm,current);
    }else if(current->ttm>ttm){
        LO_BST_Insert(&current->tlc,tId,ttm,current);
    }

}

struct TreeInfo *LO_BST_LookUp(struct TreeInfo *root,int ttm){
    /*root is NULL or root is the node with given itm*/
    if(root==NULL || root->ttm==ttm){
        return root;
    }
    /*itm is greater than root's itm*/
    if(root->ttm<ttm){
        return LO_BST_LookUp(root->trc,ttm);
    }
    /*itm is less than root's itm*/
    if(root->ttm>ttm){
        return LO_BST_LookUp(root->tlc,ttm);
    }
}

void BST_Insert(struct Info **root,int iId,int itm,int igp[MG],struct Info* parent){
 
    if(*root== NULL){
        struct Info *new_Info = (struct Info*)malloc(sizeof(struct Info));
        for(int i=0;i<MG;i++){
            new_Info->igp[i] = igp[i];
        }
        new_Info->iId = iId;
        new_Info->itm = itm;
        new_Info->ip = parent;
        new_Info->ilc = NULL;
        new_Info->irc = NULL;
        *root = new_Info;
        return;
    }
    struct Info *current=*root;   
    if(current->iId<iId){
        BST_Insert(&current->irc,iId,itm,igp,current);
    }else if(current->iId>iId){
        BST_Insert(&current->ilc,iId,itm,igp,current);
    }

}

struct Info* minValueNode(struct Info* node)
{
    struct Info* current = node;
    /* loop down to find the leftmost leaf */
    while (current!=NULL && current->ilc != NULL){
        current = current->ilc;
    }

    return current;
}

struct Info *BST_LookUp(struct Info *root,int iId){
    /*root is NULL or root is the node with given itm*/
    if(root==NULL || root->iId==iId){
        return root;
    }
    /*itm is greater than root's itm*/
    if(root->iId<iId){
        return BST_LookUp(root->irc,iId);
    }
    /*itm is less than root's itm*/
    if(root->iId>iId){
        return BST_LookUp(root->ilc,iId);
    }
}

void BST_Delete(struct Info **root,int iId){
    if(BST_LookUp(*root,iId)==NULL){
        return;
    }

    struct Info* current=BST_LookUp(*root,iId);
     /*node with same itm found*/
    if(current->ilc==NULL && current->irc==NULL){ /*leaf*/
        /*we have to make parent's pointer to leaf equal to NULL */
        if(current->ip==NULL){ /*check if node is root*/
            *root=NULL;
        }else if(current->ip->ilc==current){ /*check if node is left child */
            current->ip->ilc=NULL;
            free(current);
        }else if(current->ip->irc==current){ /*check if node is right child*/
            current->ip->irc=NULL;
            free(current);
        }
        return;
    /*if node has only one child , copy child to node and delete child*/
    }else if(current->ilc==NULL){ /*node has only right child*/      
        struct Info *temp = current->irc;
        current->itm = temp->itm;
        current->iId = temp->iId;
        current->ilc = temp->ilc;
        current->irc = temp->irc;
    
        free(current->irc);
    }else if(current->irc==NULL){ /*node has only left child*/
        struct Info *temp = current->ilc;
        current->itm = temp->itm;
        current->iId = temp->iId;
        current->ilc = temp->ilc;
        current->irc = temp->irc;
        free(current->ilc);
        return;
    }else{ /*node has two children*/
        /*node needs to be replaced with smaller itm node of right subtree*/
        struct Info *temp = minValueNode(current->irc);
        int temp_itm = temp->itm;
        int temp_iId = temp->iId;
        BST_Delete(root,temp->iId);
        current->itm = temp_itm;
        current->iId = temp_iId; 
        return;
    }
    
    return;
}




void BST_Print(struct Info *root){
    if(root==NULL){
        return;
    }
    BST_Print(root->ilc);   
    printf("%d " , root->iId);
    BST_Print(root->irc);

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
    printf(",SUBLIST= ");
    if(current==NULL){
        printf("Empty ");
    }
    
    while(current!=NULL){
        printf("%d " , current->sId);
        current = current->snext;
    }
    return;
}

/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @param m Size of the hash table.
 * @param p Prime number for the universal hash functions.
 *
 * @return 0 on success
 *         1 on failure
 */
int initialize(int hash_table_size,int prime_number){
    m = hash_table_size;
    p = prime_number;
    for(int i=0;i<10;i++){
        Hash_arr[i] = NULL;
    }
    srand(time(NULL));
    a = ((rand() % (p-1 - 1 + 1)) + 1);
    b = ((rand() % (p-1 + 1)) + 0);
    return EXIT_SUCCESS;
}

/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
int free_all(void){
    return EXIT_SUCCESS;
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
int Insert_Info(struct Group G[MG] ,int iTM,int iId,int* gids_arr,int size_of_gids_arr){
    int igp[MG]; 
    /*initialize igp*/
    for(int i=0;i<MG;i++){
        if(BST_LookUp(G[i].gr,iTM)!=NULL){
            printf("I FAILED\n");
            return 1;
        }
        igp[i] = 0;
    }
    for(int i=0;i<size_of_gids_arr-1;i++){
        if(gids_arr[i]>=0 && gids_arr[i]<=63){
            igp[gids_arr[i]] = 1;
        }else{
            printf("I FAILED\n");
            return 1;
        }
    }
    for(int i=0;i<size_of_gids_arr-1;i++){
        BST_Insert(&G[gids_arr[i]].gr,iId,iTM,igp,NULL);
    }
    for(int i=0;i<size_of_gids_arr-1;i++){
        printf("GROUPID= %d,INFOLIST= " ,gids_arr[i]);
        BST_Print(G[gids_arr[i]].gr);
        printf("\n");
    }
    return EXIT_SUCCESS;
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
int Subscriber_Registration(int sTM,int sId,int* gids_arr,int size_of_gids_arr){
    if(HT_LookUp(sId)!=NULL){ //if susbscriber with that Id exists then exit with 1 
        return 1;
    }

    struct TreeInfo *sgp[MG];
    struct TreeInfo *tgp[MG];
    for(int i=0;i<MG;i++){
        tgp[i] = (struct TreeInfo*)1;//initialize with value 1 in case it doesnt change later
        sgp[i] = (struct TreeInfo*)1;
    }
    for(int i=0;i<size_of_gids_arr-1;i++){
        /*initialize Tgp using gids_arr*/
        if(gids_arr[i]<0 || gids_arr[i]>63){
            return 1;
        }
        tgp[gids_arr[i]] = NULL;
        sgp[gids_arr[i]] = NULL;
        /*insert node in subscription list*/
        L_Insert(&G[gids_arr[i]].gsub,sId);
    }
    /*insert node in subscriber list*/
    HT_Insert(sId,sTM,sgp,tgp);
    
    printf("SUBSCRIBER LIST = ");
    for(int i=0;i<m;i++){
        struct SubInfo *current = Hash_arr[i];
        while(current!=NULL){
            printf("%d " , current->sId );
            current=current->snext;
        }
    }
    printf("\n");
    for(int i=0;i<size_of_gids_arr-1;i++){
        printf("GROUPID = %d,SUBLIST = " , gids_arr[i]);
        struct Subscription *curr = G[gids_arr[i]].gsub;
        while(curr!=NULL){
            printf("%d " , curr->sId);
            curr= curr->snext;
        }
        printf("\n");
    }

    return 0;
}
/*Prune_Copy function copies info from G[].gr tree to S.tgp tree
info with timestamp <= given tm only 
*/
void Prune_Copy(struct Info *root,struct SubInfo **S,int tm,int i){
    if(root==NULL){
        return;
    }
    struct SubInfo *head = *S;
    Prune_Copy(root->ilc,&head,tm,i);
    Prune_Copy(root->irc,&head,tm,i);
    if(root->itm<=tm){
        while(head!=NULL){
            if(head->tgp[i]!=1){
                LO_BST_Insert(&head->tgp[i],root->iId,root->itm,NULL);
                // if(Isleaf(head->sgp[i])==0){ /*if after insertion sgp is no longer a leaf that means we have to update it to the same itm leaf in the linked list*/
                //     printf("...\n");
                //     if(head->sgp[i]->tlc->ttm==head->sgp[i]->ttm){
                //         head->sgp[i] = head->sgp[i]->tlc; 
                //     }else{
                //         head->sgp[i] = head->sgp[i]->trc;
                //     }
                // }
            }
            head=head->snext;
        }
    }
}

/*Prune_Delete function deletes info from G[].gr tree 
info with timestamp <= given tm 
*/  
void Prune_Delete(struct Info **root,int tm){
    if(*root==NULL){
        return;
    }

    struct Info* head = *root;
    Prune_Delete(&head->ilc,tm);
    Prune_Delete(&head->irc,tm);
    if(head->itm<=tm && head->ip==NULL){
        BST_Delete(root,head->itm);
        
    }else if(head->itm<=tm){
        BST_Delete(&head,head->itm);
    }
}

/**
 * @brief Prune Information from server and forward it to client
 *
 * @param tm Information timestamp of arrival
 * @return 0 on success
 *          1 on failure
 */
int Prune(struct Group G[MG],struct SubInfo **S,int tm){
    printf("\nR %d DONE  \n" , tm);
    for(int i=0;i<6;i++){
        Prune_Copy(G[i].gr,S,tm,i);
        Prune_Delete(&G[i].gr,tm);
        printf("GROUPID = <%d> , INFOLIST =<" , i);
        BST_Print(G[i].gr);
        printf(">");
        L_Print(G[i].gsub);
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
/* KANW CONSUME KAI PREPEI NA KANW PRINT OLA TA STOIXEIA TIS LISTAS TWN FULLWN PROS TA DE3IA MEXRI KAI TO TELEUTAIO 
CASES: AN O SGP EINAI NULL KAI O TGP EINAI NULL TOTE DEN UPARXOUN STOIXEIA KAI KANW EXIT 
       AN O SGP EINAI NULL KAI O TGP EXEI STOIXEIA TOTE KANW PRINT OLI TIN LISTA KAI VAZW TON SGP NA DEIXNEI STO TELOS
       AN O SGP DEN EINAI NULL TOTE PAW STIN LISTA ,VRISKW TO STOIXEIO POU NA EXEI IDIO TTM ME TON SGP KAI KANW PRINT APO TO EPOMENO PROS TO TELOS 
    */
int Consume(int sId){
    if(HT_LookUp(sId)==NULL){
        printf("C FAILED\n ");
        return 1;
    }
    struct SubInfo *current = HT_LookUp(sId);
    for(int i=0;i<MG;i++){
        if(current->tgp[i]==1 || current->tgp[i]==NULL){
            break;
        }
        printf("GROUPID= %d ," , i);
        printf("TREELIST= ");
        if(current->sgp[i]==NULL){ /*sgp null but tgp not null*/
            current->sgp[i]=current->tgp[i];
            while(Isleaf(current->sgp[i])==0){  /*find first node of list*/
                current->sgp[i]=current->sgp[i]->tlc;
            }
            while(current->sgp[i]->next!=NULL){ /*move through whole list*/
                printf("%d " , current->sgp[i]->tId);
                current->sgp[i] = current->sgp[i]->next;
            }
            printf("%d ,NEWSGP = %d \n" , current->sgp[i]->tId , current->sgp[i]->tId);
        }
        

    }
    return EXIT_SUCCESS;
}
/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Delete_Subscriber(int sId){
    /*chech if subscriber with given sId exists*/
    if(HT_LookUp(sId)==NULL){
        return 1;
    }
    /*first delete subscriber's with given sId subscriptions*/
    for(int i=0;i<MG;i++){
        L_Delete(&G[i].gsub,sId);
    }
    struct SubInfo *curr = Hash_arr[Universal_Hash_Function(sId,m,p)];
    printf("SUBSCRIBERLIST= ");
    while(curr!=NULL){
        if(curr->sId!=sId){
            printf("%d " , curr->sId);
        }
        curr = curr->snext;
    }
    printf("\n");
    curr = HT_LookUp(sId);
    for(int i=0;i<MG;i++){
        if(curr->sgp[i]!=1){
            printf("GROUPID= %d" , i);
            L_Print(G[i].gsub);
            printf("\n");
        }

    }
    /*delete given sId from subscribers list*/
    HT_Delete(sId);
    return EXIT_SUCCESS;
}
/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(void){
    int no_groups=0,no_subs=0;
    for(int i=0;i<MG;i++){
        if(G[i].gr!=NULL){
            no_groups+=1; /*count no_groups*/
        }
        printf("GROUP ID = %d " , G[i].gId);
        printf(",INFOLIST = ");
        BST_Print(G[i].gr);
        L_Print(G[i].gsub);
        printf("\n");
    }
    
    HT_Print();
    for(int i=0;i<m;i++){
        struct SubInfo* head=Hash_arr[i];
        while(head!=NULL){
            no_subs++;
            head = head->snext;
        }
    }
    printf("NO_GROUPS= %d , NO_SUBSCRIBERS = %d \n" , no_groups , no_subs);
    return EXIT_SUCCESS;
}



int main(){
	int gids_arr[5] = {1,2,3,4,-1};
	Subscriber_Registration(1000,2,gids_arr,5);
	//struct SubInfo* temp= HT_LookUp(2);
	//printf("%d \n" , temp->sId);

	return 0;
}