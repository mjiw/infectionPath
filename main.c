//
//  main.c
//  infestPath
//
//  Created by Juyeop Kim on 2022/10/20.
//

#include <stdio.h>
#include <string.h>
#include "ifct_element.h"
#include "ifct_database.h"

#define MENU_PATIENT        1
#define MENU_PLACE          2
#define MENU_AGE            3
#define MENU_TRACK          4
#define MENU_EXIT           0

#define TIME_HIDE           2


int trackInfester(int patient_no, int *detected_time, int *place);

int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int pIndex, age, time;
    int placeHist[N_HISTORY];
    
    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }
    
    fp = fopen(argv[1],"r");
    
	//에러코드
    if (fp == NULL)
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1;
    }
    
    //1-2. loading each patient informations
   // int i=0;
   	while(3==fscanf(fp,"%d %d %d", &pIndex, &age, &time)){
   		int i=0;
		   for(i=0;i<N_HISTORY;i++){
		   	fscanf(fp,"%d",&placeHist[i]);
		   	ifct_element=ifctele_genElement(pIndex,age,time,placeHist);
		   }
		   	printf("%i 번째 환자 감염 경로 : ", pIndex+1);
		   	ifctele_printElement(ifct_element);
		   	ifctdb_addTail(ifct_element);
			printf("\n");
	} 
    
    //1-3. FILE pointer close
    fclose(fp);
    
    int innumber;
    int outnumber;
    //void *ifct_element2;
    char inplace[20];
    
    int inplacehist[N_HISTORY];
    int minage;
    int maxage;
    
    int place1, place2;
    printf("the first place is %n");

    do {
        printf("\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK
        printf("0. Exit.\n");                                               //MENU_EXIT
        printf("=============== ------------------------------------------------------- =============\n\n");
        
        printf("Select a menu :");
        scanf("%d", &menu_selection);
        fflush(stdin);
        
        switch(menu_selection)
        {
            case MENU_EXIT:
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT:
            	printf("환자 번호 입력:");
            	scanf("%i",&innumber);
            	ifct_element=ifctdb_getData(innumber);
            	
				if(innumber==ifctele_getIndex(ifct_element)) {
					printf("age: %i\n",ifctele_getAge(ifct_element));
					printf("time: %i\n",ifctele_getinfestedTime(ifct_element));
					int i;
					for(i=0;i<N_HISTORY;i++){
						printf("move: %s\n",ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,i)));
					}		
				}  
                break;
                
            case MENU_PLACE:
                printf("장소 입력:");
            	scanf("%s",&inplace);
            	int i;
            	int j;
           		int t;
            	for(i=0;i<ifctdb_len();i++){
            			ifct_element=ifctdb_getData(i);
            			
            			for(j=0;j<N_HISTORY;j++){
            				
							if(strcmp(inplace,ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,j)))==0){
									inplacehist[j]=ifctele_getHistPlaceIndex(ifct_element, j);
									printf("age: %i\n",ifctele_getAge(ifct_element));
									printf("time: %i\n",ifctele_getinfestedTime(ifct_element));
									printf("move: %s\n",ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,j)));
							}
								 // inplacehist[j]의 값 설정 부분 코드를 우선 수정을 해야 할듯 하네요.
                                 //(아니면 printf로 출력하는 부분을 재고하던가 해야 할 것 같습니다.)
							else{
								continue;
							}	
							
						}
						
						for(j=0;j<N_HISTORY;j++){
							if(strcmp(inplace,ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,j)))==0){
								printf("age: %i\n",ifctele_getAge(ifct_element));
								printf("time: %i\n",ifctele_getinfestedTime(ifct_element));
					//			outnumber=ifctele_getIndex(ifct_element);
					//			ifct_element2=ifctdb_getData(outnumber);
								printf("move: %s\n",ifctele_getPlaceName(inplacehist[j]));
					//				printf("move: %s\n",ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,j)));
							}
					//		else{
							//	printf("move: %s\n",ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,j)));
					//		}
						//	printf("move: %s\n",ifctele_getPlaceName(inplacehist[j]));
						}
						
					//	printf("age: %i\n",ifctele_getAge(ifct_element2));
					//	printf("time: %i\n",ifctele_getinfestedTime(ifct_element2));
					//	for(j=0;j<N_HISTORY;j++){
					//		printf("move: %s\n",ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,j)));	
					//	}
				}
            	
                break;
                
            case MENU_AGE:
                
                break;
                
            case MENU_TRACK:
                    
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    
    return 0;
}
