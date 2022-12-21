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


int convertTimeToIndex(int time, int infectedTime){ //시점 값을 통해 장소배열의 index를 산출하는 함수 
    int index=-1;
    if(time<=infectedTime && time>infectedTime-N_HISTORY){
        index=N_HISTORY-(infectedTime-time)-1;
	}
	return index;
}

int isMet(int innumber2, int inumber){ //만난시점을 찾는 함수 
    int i;
    int j; 
    int flag=0;
    void *ifct_element;
    void *now_element;
    
    int now_movedate;
    int ifct_movedate;
    int metdate;
    
    now_element=ifctdb_getData(innumber2); //현재환자에 대한 데이터 
    ifct_element=ifctdb_getData(inumber); //비교대상환자에 대한 데이터 
					
	for(i=2;i<N_HISTORY;i++){ // 입력한 환자의 2,3,4일 전 장소 반복문 
		now_movedate=ifctele_getinfestedTime(now_element)-i; //현재환자 장소별 시점(2,3,4일 전 시점계산) 
		
		for(j=3;j<N_HISTORY;j++){  //비교할 환자의 1일전, 당일 장소 (전파시킬 수 있는 시점) 반복문 
			ifct_movedate=ifctele_getinfestedTime(ifct_element)-(4-j);//비교환자 장소별 시점 (1일 전,당일 시점 계산)
			
			if(ifctele_getHistPlaceIndex(now_element, convertTimeToIndex(now_movedate,ifctele_getinfestedTime(now_element)))==ifctele_getHistPlaceIndex(ifct_element, j)){ //입력한 환자와 비교환자의 장소가 같을 때 
				if(now_movedate==ifct_movedate){ //시점도 같을 때 
					metdate=now_movedate; //현재환자와 전파자가 만난시점 
					flag++; // 만난시점 반환을 위한 표시 
				}		
			}							
		
			else{ //입력한 환자와 비교환자의 장소가 같지 않을 때 
				continue;
				}
						
		}	
	}
					
	if(flag==0){ //입력한 환자에게 전파한 환자가 없는 경우 
		metdate=-1;
	}
									
	return metdate; //만난 시점 반환 
}

int trackInfecter(int innumber2){ //전파자를 찾는 함수 
	int i;
	int j;
	
	void *ifct_element;
	void *now_element;
	
	int metdate;
	int min=-1;
	int firstpeople;
	int firstpeople1;
	
	
	for(i=0;i<ifctdb_len();i++){//비교대상 환자 반복문 
		ifct_element=ifctdb_getData(i);					
		metdate=isMet(innumber2,ifctele_getIndex(ifct_element)); //현재환자와 환자들 비교해서 전파자 만난 날 찾기	
									
		if(metdate>0){ //만났다면
		//만난 환자 중 가장 이른 시간에 만난 환자 찾기 
			if(min==-1){
				min=metdate;
				firstpeople=i;
				firstpeople1=i;
				break;
			}
			else if(metdate>min){
				firstpeople=firstpeople1;
				break;
			}
			else if(min>metdate){
				firstpeople=i;
				break;
			} 

		}
		
		else{
			firstpeople=-1; //전파자가 없는 경우(최초전파자) 
		}		
	}
	
	return firstpeople; //전파자의 번호 반환 
}
				
				
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
    
    int innumber; //menu 1번에서 입력받는 환자정보 저장 
    
    //menu 2번에서 사용되는 변수 
    int outnumber; //입력한 장소가 있는 환자 번호 저장 
    char inplace[20]; //입력받는 장소 저장 
    int inplacehist[N_HISTORY]; //입력한 장소가 있는 환자 정보에 대한 5개 장소 저장 

	//menu 3번에 사용되는 변수 
	int age_min;
	int age_max;
	
    //menu 4번에 사용되는 변수 
    void *now_element;
    int innumber2;
    int firstpeople; 
	int metdate;
    int people;
   
	//case문마다 반복문에 사용되는 변수 
	int i;

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
            	ifct_element=ifctdb_getData(innumber); //입력한 환자번호의 정보 저장 
            	
            	//정보 출력 
            	printf("index: %i\n",ifctele_getIndex(ifct_element));
            	printf("age: %i\n",ifctele_getAge(ifct_element));
				printf("time: %i\n",ifctele_getinfestedTime(ifct_element));
				
				for(i=0;i<N_HISTORY;i++){ //최근 5개 이동장소 출력 
					printf("move: %s\n",ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,i)));
				}
				
                break;
                
            case MENU_PLACE:
                printf("장소 입력:");
            	scanf("%s",&inplace);
            
            	int j;
           		int t;
           		int count=0;
            	for(i=0;i<ifctdb_len();i++){// 환자 정보 불러오는 반복문 
            		ifct_element=ifctdb_getData(i);
            			
            		for(j=0;j<N_HISTORY;j++){	//5개의 장소 불러오는 반복문 	
						if(strcmp(inplace,ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,j)))==0){	//입력한 장소가 환자정보에 있을때					
							for(t=0;t<N_HISTORY;t++){
								inplacehist[t]=ifctele_getHistPlaceIndex(ifct_element, t); //환자정보를 배열에 저장 
							}
							inplacehist[j]=ifctele_getHistPlaceIndex(ifct_element, j);
							outnumber=ifctele_getIndex(ifct_element); //입력한 장소가 있는 환자 번호 저장 
							count++; //입력한 장소가 환자정보에 있을 때를 표시 
						}
							
						else if(ifctele_getIndex(ifct_element)==outnumber){ //입력한 장소가 포함된 환자정보에 있는 다른 장소 저장 
							inplacehist[j]=ifctele_getHistPlaceIndex(ifct_element, j); 		
						}
						
						else{ //입력한 장소가 환자정보에 없을 때 
							count=0;
							continue;
						}		
					}
						
					if(count==0){ //입력한 장소가 환자정보에 없을 때  
						continue;
					}
					else if(count!=0){  //입력한 장소가 환자 정보에 있을 때 환자 정보 순차적으로 출력 
						printf("index: %i\n",i); //ifctele_getIndex(ifct_element)
						printf("age: %i\n",ifctele_getAge(ifct_element));
						printf("time: %i\n",ifctele_getinfestedTime(ifct_element));
						for(j=0;j<N_HISTORY;j++){
							printf("move: %s\n",ifctele_getPlaceName(inplacehist[j]));
						}
					}

					count=0; //다음 환자정보에서 입력한 장소가 있는지 구별하기 위해서 count를 0으로 돌려줌 
				}
     	
                break;
                
            case MENU_AGE:
                printf("나이의 최솟값 입력:");
                scanf("%i",&age_min);              
                printf("나이의 최댓값 입력:");
				scanf("%i",&age_max); 

				for(i=0;i<ifctdb_len();i++){
					ifct_element=ifctdb_getData(i);
					if(age_min<=ifctele_getAge(ifct_element)){ //최솟값 이상이면서 
						if(age_max>=ifctele_getAge(ifct_element)){ //최댓값 이하일 때 환자 정보출력 
							printf("index: %i\n",i);
							printf("age: %i\n",ifctele_getAge(ifct_element));
							printf("time: %i\n",ifctele_getinfestedTime(ifct_element));						
							int j;
							for(j=0;j<N_HISTORY;j++){ //장소 출력 
								printf("move: %s\n",ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,j)));
							}
						}
					}
				}
				
            	break;
                
            case MENU_TRACK:
            	//지정된 롼자를 시작으로 전파자와 감염당한 시점 및 장소를 순차적 출력, 최종전파자 출력 
            	printf("환자 번호 입력:");
            	scanf("%i",&innumber2); 
		
				while(innumber2!=-1){
					now_element=ifctdb_getData(innumber2);//현재환자에 대한 데이터
					firstpeople=trackInfecter(innumber2); //전파자 
					
					if(firstpeople!=-1){
						printf("%i번환자는 %i번 환자에게 전파됨\n",innumber2,firstpeople);
						
						for(i=0;i<ifctdb_len();i++){ //감염당한 시점과 장소 출력을 위한 반복문 
							ifct_element=ifctdb_getData(i);
							metdate=isMet(innumber2,i);
	
							if(metdate>0){ //만났다면 감염시점과 감염장소 출력 
								printf("%i일에 %s에서 감염됨\n",metdate,ifctele_getPlaceName(ifctele_getHistPlaceIndex(now_element,convertTimeToIndex(metdate,ifctele_getinfestedTime(now_element)))));
								break;		
							}	
						}
						
						innumber2=firstpeople;
					}
					
					else{ // 전파자가 없는경우 
						firstpeople=innumber2;
						innumber2=-1; //최초전파자
						printf("%i번 환자는 최초전파자이다\n",firstpeople); //최종전파자 출력 
					}
		
				} 
                		 
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    } while(menu_selection != 0);
    
    
    return 0;
}

