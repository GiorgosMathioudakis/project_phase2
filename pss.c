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


int a,b,p,m; /*a,b are random integers generated to help with hash_universal , p is prime number and m is size of hash_table*/
struct SubInfo** Hash_arr; /*hash table*/
struct Group G[MG]; /*groups*/

/*assistant function that identifies if a node is leaf or not*/
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

/*function prints nodes of LO_BST*/
void LO_BST_Print(struct TreeInfo* root){
    if(root==NULL){
        printf("Empty");
        return;
    }
    while(root->tlc!=NULL){
        root=root->tlc;
    }
    while(root!=NULL){
        printf("%d " , root->tId);
        root = root->next;
    }
}

/*no_null is a integer that counts how many of the hash table's cells are empty */
int no_null;
/*function prints out sorted linked list*/
void SL_Print(struct SubInfo *S){

    struct SubInfo *current= S;

    //printf("SUBSCRIBERLIST = ");
    if(current==NULL){
        no_null++;
        if(no_null==m){
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
                LO_BST_Print(current->tgp[i]);
                printf("\n");
            }
            
        }
        printf("\n");
        current = current->snext;
    }
    return;
}
/*function returns index for Hash_table given a key*/
int Universal_Hash_Function(int key){
    return (((a*key)^b)%p)%m; 
}
/*function inserts node in Hash table */
void HT_Insert(int sId,int stm,struct TreeInfo* sgp[MG],struct TreeInfo* tgp[MG]){
    int pos = Universal_Hash_Function(sId);
    SL_Insert(&Hash_arr[pos],sId,stm,sgp,tgp);
}
/*function deletes node from hash_table*/
void HT_Delete(int sId){
    int pos = Universal_Hash_Function(sId);
    SL_Delete(&Hash_arr[pos],sId);
}
/*function searches for node in Hash table*/
struct SubInfo *HT_LookUp(int sId){
    int pos = Universal_Hash_Function(sId);
    SL_LookUp(Hash_arr[pos],sId);
}

/*assistant function that prints subscriber list plus some info*/
void HT_Print(){
    printf("SUBSCRIBERLIST= ");
    for(int i=0;i<m;i++){
        struct SubInfo* head = Hash_arr[i];
        while(head!=NULL){
            printf("%d " , head->sId);
            head = head->snext;
        }
    }
    no_null =0;
    for(int i=0;i<m;i++){
        SL_Print(Hash_arr[i]);
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
/*function inserts node in lo_bst*/
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
/*function searches node in lo_bst given a timestamp*/
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
/*function inserts node in bst*/
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
/*assistance function that returns minimum value of given tree*/
struct Info* minValueNode(struct Info* node)
{
    struct Info* current = node;
    /* loop down to find the leftmost leaf */
    while (current!=NULL && current->ilc != NULL){
        current = current->ilc;
    }

    return current;
}
/*function searches for node in bst given iId*/
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
/*function deletes node from bst given iId*/
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



/*function prints bst*/
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
int initialize(hashTableSize, universalHashingNumber){
    m = hashTableSize;
    p = universalHashingNumber;
    Hash_arr = (struct SubInfo**)malloc(m * sizeof(struct SubInfo*));
    for(int i=0;i<m;i++) Hash_arr[i] = NULL;
    
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
void Prune_Copy(struct Info *root,int tm,int i){
    if(root==NULL){
        return;
    }
    Prune_Copy(root->ilc,tm,i);
    Prune_Copy(root->irc,tm,i);
    if(root->itm<=tm){
        for(int k=0;k<m;k++){
            struct SubInfo *temp = Hash_arr[k];
            while(temp!=NULL){
                if(temp->tgp[i]!=1){
                    LO_BST_Insert(&temp->tgp[i],root->iId,root->itm,NULL);
                }
                temp = temp->snext;
            }
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
        BST_Delete(root,head->iId);
    }else if(head->itm<=tm){
        BST_Delete(&head,head->iId);

    }
}

/**
 * @brief Prune Information from server and forward it to client
 *
 * @param tm Information timestamp of arrival
 * @return 0 on success
 *          1 on failure
 */
int Prune(int tm){
    for(int i=0;i<MG;i++){
        Prune_Copy(G[i].gr,tm,i);
        Prune_Delete(&G[i].gr,tm);
        if(G[i].gr!=NULL){
            printf("GROUPID = <%d> , INFOLIST =<" , i);
            BST_Print(G[i].gr);
            printf(">");
            L_Print(G[i].gsub);
            printf("\n");
        }
    }

    for(int i=0;i<m;i++){
        struct SubInfo* head = Hash_arr[i];
        while(head!=NULL){
            printf("SUBSCRIBERID=%d ,GROUPLIST= \n" , head->sId);
            for(int i=0;i<MG;i++){
                if(head->tgp[i]!=1){
                    printf("%d ,TREELIST =" , i);
                    LO_BST_Print(head->tgp[i]);
                    printf("\n");
                }
            }
            head = head->snext;
            printf("\n");
        }
    }
    printf("\n");

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

    struct SubInfo *current = HT_LookUp(sId);
    for(int i=0;i<MG;i++){
        if(current->tgp[i]!=1 && current->tgp[i]!=NULL){
            printf("GROUPID= %d ," , i);
            printf("TREELIST= ");
            if(current->sgp[i]==NULL || current->sgp[i]!=1){ /*sgp null but tgp not null*/
                current->sgp[i]=current->tgp[i];
                while(Isleaf(current->sgp[i])==0){  /*find first node of list*/
                    current->sgp[i]=current->sgp[i]->tlc;
                }
                while(current->sgp[i]->next!=NULL){ /*move through whole list*/
                    printf("%d " , current->sgp[i]->tId);
                    current->sgp[i] = current->sgp[i]->next;
                }
                printf("%d ,NEWSGP = %d" , current->sgp[i]->tId , current->sgp[i]->tId);
            }else{
                printf("Empty");
            }
            printf("\n");
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
    printf("SUBSCRIBER LIST = ");
    for(int i=0;i<m;i++){
        struct SubInfo *current = Hash_arr[i];
        while(current!=NULL){
            if(current->sId!=sId) { printf("%d " , current->sId ); }
            current=current->snext;
        }
    }
    printf("\n");
    struct SubInfo* curr = HT_LookUp(sId);
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


/**
 * @brief Parsing event arguments from buffer
 *
 * @param buff String of current line in the test file
 * @param int_arr_size Pointer to integer that represents the number of arguments in the test after the Event char
 *
 * @return 0 on success
 *         1 on failure
 */
static int * event_args(char *buff,unsigned int * int_arr_size){
    int * int_arr;
    char event='\0';
    int value;
    char *p =buff+1;
    unsigned int len = 0;
    unsigned int count_args=1;
    unsigned int i =0;
    sscanf(buff, "%c", &event);
    while (sscanf(p, "%d%n", &value, &len)==1 && value!=-1 ) {
        count_args++;
        p += len;
    }
    p=buff+1;
    (*int_arr_size)=count_args;
    int_arr= malloc(count_args*sizeof(int));
    while (sscanf(p, "%d%n", &value, &len)==1) {
        int_arr[i++]=value;
        p += len;
        if(value ==-1){
            break;
        }
    }

    return int_arr;
}

/**
 * @brief The main function
 *
 * @param argc Number of arguments
 * @param argv Argument vector
 *
 * @return 0 on success
 *         1 on failure
 */
int main(int argc, char **argv)
{
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event='\0';
    for(int i=0;i<MG;i++){
        G[i].gId=i;
        G[i].gr = NULL;
        G[i].gsub = NULL;
    }

	/* Check command buff arguments */
	if (argc != 4)
	{
		fprintf(stderr, "Usage: %s <m> <p> <input_file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Parse command buff arguments */
	int hashTableSize = atoi(argv[1]);
	int universalHashingNumber = atoi(argv[2]);

	if (hashTableSize <= 0)
	{
		fprintf(stderr, "\n Invalid hash table size: %d\n", hashTableSize);
		perror("Parsing command line argument\n");
		return EXIT_FAILURE;
	}

	if (universalHashingNumber <= 0)
	{
		fprintf(stderr, "\n Invalid universal hashing number: %d\n", universalHashingNumber);
		perror("Parsing command line argument\n");
		return EXIT_FAILURE;
	}

	/* Open input file */
	if ((fin = fopen(argv[3], "r")) == NULL)
	{
		fprintf(stderr, "\n Could not open file: %s\n", argv[3]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}
    
	/* Initializations */
	initialize(hashTableSize, universalHashingNumber);

	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin))
	{
		DPRINT("\n>>> Event: %s\n", buff);
		switch (buff[0])
		{

		/* Comment */
		case '#':
			break;

		/* Insert_Info
		 * I <itm> <iId> <gId1> <gId2> ... <gIdk> -1 */
		case 'I':
		{
			int itm;
			int iId;
            unsigned int num_of_gids;
            int * event_args_arr;
            int * gids_arr;
            event_args_arr=event_args(buff, &num_of_gids);
            itm=event_args_arr[0];
            iId=event_args_arr[1];
            gids_arr= event_args_arr+2;
            num_of_gids-=2;
			if (Insert_Info(G,itm,iId,gids_arr,num_of_gids)==0)
			{
				DPRINT("%c <%d> <%d> DONE\n",buff[0], itm,iId);
			}
			else
			{
				fprintf(stderr, "%s failed\n", buff);
			}
            num_of_gids=0;
            free(event_args_arr);
			break;
		}

		/* Subscriber_Registration
		 * S <sΤΜ> <sId> <gId1> <gId2> ... <gIdm> -1  */
		case 'S':
		{
            int sTM, sId;
            unsigned int num_of_gids;
            int * event_args_arr;
            int * gids_arr;
            event_args_arr=event_args(buff, &num_of_gids);
            sTM=event_args_arr[0];
            sId=event_args_arr[1];
            gids_arr= event_args_arr+2;
            num_of_gids-=2;
            if (Subscriber_Registration(sTM,sId,gids_arr,num_of_gids)==0)
            {
                DPRINT("%c <%d> <%d> DONE\n", buff[0],sTM,sId);
            }
            else
            {
                fprintf(stderr, "%s failed\n", buff);
            }
            num_of_gids=0;
            free(event_args_arr);
			break;
		}

		/* Prune
		 * R <tm> */
		case 'R':
		{
            int tm;
			sscanf(buff, "%c %d", &event, &tm);
			if (Prune(tm)==0)
			{
				DPRINT("%c <%d> DONE\n", event, tm);
			}
			else
			{
				fprintf(stderr, "%c %d failed\n", event, tm);
			}
			break;
		}

		/* Consume
		 * C <sId> */
		case 'C':
		{
            int sId;
			sscanf(buff, "%c %d", &event, &sId);
			if (Consume(sId)==0)
			{
				DPRINT("%c <%d> DONE\n", event,sId);
			}
			else
			{
				fprintf(stderr, "%c %d failed\n", event,sId);
			}
			break;
		}

		/* Delete_Subscriber
		 * D <sId>: */
		case 'D':
		{
			int sId;
			sscanf(buff, "%c %d", &event, &sId);
            if (Delete_Subscriber(sId)==0)
			{
				DPRINT("%c <%d> DONE\n", event, sId);
			}
			else
			{
				fprintf(stderr, "%c %d failed\n", event, sId);
			}
			break;
		}

		/* Print
		 * P */
		case 'P':
		{
			if (Print_all()==0)
			{
				DPRINT("%c DONE\n", buff[0]);
			}
			else
			{
				fprintf(stderr, "%c failed\n", buff[0]);
			}

			break;
		}
		/* Empty line */
		case '\n':
			break;
		/* Ignore everything else */
		default:
			DPRINT("Ignoring line: %s \n", buff);
			break;
		}
	}
	free_all();
	return (EXIT_SUCCESS);
}
