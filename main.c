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


//int trackInfecter(int patient_no, int *detected_time, int *place){
	
//}

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
    int outnumber=1;
    //void *ifct_element2;
    char inplace[20];
    
    int innumber2;
    int firstpeople;
    int firstpeople1;
    
    
    int inplacehist[N_HISTORY];
    int minage;
    int maxage;
    
    int place1, place2;
    
    void *now_element;
    int metdate;
    int mettime1=-1;
    int mettime2;
    
    int now_movedate;
    int ifct_movedate;
   
    int people;
    int flag;
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
           		int count=0;
            	for(i=0;i<ifctdb_len();i++){
            		ifct_element=ifctdb_getData(i);
            			
            		for(j=0;j<N_HISTORY;j++){		
						if(strcmp(inplace,ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,j)))==0){
						
							for(t=0;t<N_HISTORY;t++){
								inplacehist[t]=ifctele_getHistPlaceIndex(ifct_element, t);
							}
							inplacehist[j]=ifctele_getHistPlaceIndex(ifct_element, j);
							outnumber=ifctele_getIndex(ifct_element);
							count++;
						}
							
						else if(ifctele_getIndex(ifct_element)==outnumber){
							inplacehist[j]=ifctele_getHistPlaceIndex(ifct_element, j);
							
						}
						else{
							count=0;
							continue;
						}		
					}
						
					if(count==0){
						continue;
					}
					else if(count!=0){
						printf("%i",count);
						printf("age: %i\n",ifctele_getAge(ifct_element));
						printf("time: %i\n",ifctele_getinfestedTime(ifct_element));
					}
					
					for(j=0;j<N_HISTORY;j++){
			
						if(count!=0){
							printf("move: %s\n",ifctele_getPlaceName(inplacehist[j]));
						}
						else{
							break;
						}
					}
					count=0;
				}
            	
                break;
                
            case MENU_AGE:
                
                break;
                
            case MENU_TRACK:
            	//지정된 롼자를 시작으로 전파자와 감염당한 시점 및 장소를 순차적 출력, 최종전파자 출력 
            //	printf("현재환자를 입력하시오:");
            //	scanf("%i",&innumber2);
            //	now_element=ifctdb_getData(innumber2); //현재환자에 대한 데이터-현재환자의 2~4일전 이동장소와 비교대상환자의 당일과 1일전 비교 
            //	int flag=0;
				int convertTimeToIndex(int time, int infectedTime){ //시점 값을 통해 장소배열의 index를 산출하는 함수 
            		int index=-1;
            		if(time<=infectedTime && time>infectedTime-N_HISTORY){
            			index=N_HISTORY-(infectedTime-time)-1;
					}
					return index;
				}
				
            	//두환자의 장소별 시점 계산 -> 비교잘됨 
				int isMet(int innumber2, int inumber){ //만난장소를 찾아요 
            		int i;
            		int j; 
            		int flag=0;
            		ifct_element=ifctdb_getData(inumber);//비교대상환자에 대한 데이터 
					
					for(i=2;i<N_HISTORY;i++){
						now_movedate=ifctele_getinfestedTime(now_element)-i; //현재환자 장소별 시점 
						printf("%i\n",now_movedate);
						for(j=3;j<N_HISTORY;j++){
							ifct_movedate=ifctele_getinfestedTime(ifct_element)-(4-j);//비교환자 장소별 시점 
							if(ifctele_getHistPlaceIndex(now_element, convertTimeToIndex(now_movedate,ifctele_getinfestedTime(now_element)))==ifctele_getHistPlaceIndex(ifct_element, j)){
								if(now_movedate==ifct_movedate){
									people=ifctele_getIndex(ifct_element); //현재환자에게 전파한 사람 
									flag++;
									metdate=ifct_movedate;
									printf("%i\n",metdate);
								}
								
								else{
									printf("!\n");
									
								}
							
							//	printf("%i\n",ifctele_getHistPlaceIndex(now_element, convertTimeToIndex(movedate,ifctele_getinfestedTime(now_element))));
							//	printf("%i\n",convertTimeToIndex(movedate,ifctele_getinfestedTime(now_element)));
							//	printf("%i\n",ifctele_getHistPlaceIndex(ifct_element, j));
								
							
							}
							else{
							//	printf("!\n");
								continue;
							}
							
						}	
					}
					
					if(flag==0){
						metdate=-1;
					}
										
					return metdate;
				}
				 
				 
            	int trackInfecter(int innumber2){ //전파자를 찾아요 
					int i;
					int j;
					for(i=0;i<ifctdb_len();i++){//비교대상 환자 반복문 
						ifct_element=ifctdb_getData(i);
						
						mettime2=isMet(innumber2,ifctele_getIndex(ifct_element)); //현재환자의  전파자 
					
						
						printf("==>%i\n",mettime2);
				
						
						if(mettime2>0){	
							firstpeople1=people;
						}
						else{
							firstpeople1=-1;
						}		
					}
					return firstpeople1;
				}
				
				printf("현재환자를 입력하시오:");
            	scanf("%i",&innumber2);
            	now_element=ifctdb_getData(innumber2);
            	
				firstpeople=trackInfecter(innumber2);
				printf("%i\n",firstpeople);
				
				if(firstpeople!=-1){
					printf("%i번 환자는 %i환자에게 전파됨\n",innumber2,firstpeople);
				}
				else{
					firstpeople=innumber2;
					innumber2=-1; //최초전파자
					printf("%i번 환자는 최초전파자이다\n",firstpeople);
				}
					
				innumber2=firstpeople;
				
				printf("%i\n",innumber2); 
				
				firstpeople=trackInfecter(innumber2);
//				printf("%i\n",firstpeople); 
				
		
/*				while(innumber2!=-1){
					firstpeople=trackInfecter(innumber2);
					
					if(firstpeople!=-1){
						printf("%i환자는 %i환자에게 전파됨\n",innumber2,firstpeople);
						innumber2=firstpeople;
					}
					else{
						firstpeople=innumber2;
						innumber2=-1; //최초전파자
						printf("%i번 환자는 최초전파자이다\n",firstpeople); 
					}

			
				} */
                
				//전파자와 감염당한 시점 및 장소 순차적 출력, 최종전파자 최종적으로 출력  
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    } while(menu_selection != 0);
    
    
    return 0;
}

