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


int convertTimeToIndex(int time, int infectedTime){ //���� ���� ���� ��ҹ迭�� index�� �����ϴ� �Լ� 
    int index=-1;
    if(time<=infectedTime && time>infectedTime-N_HISTORY){
        index=N_HISTORY-(infectedTime-time)-1;
	}
	return index;
}

int isMet(int innumber2, int inumber){ //������Ҹ� ã�ƿ� 
    int i;
    int j; 
    int flag=0;
    void *ifct_element;
    void *now_element;
    
    int now_movedate;
    int ifct_movedate;
    int people; //����ȯ�ڿ��� ������ ���
    int metdate;
    
    now_element=ifctdb_getData(innumber2); //����ȯ�ڿ� ���� ������ 
    ifct_element=ifctdb_getData(inumber); //�񱳴��ȯ�ڿ� ���� ������ 
					
	for(i=2;i<N_HISTORY;i++){ //1�� ���� 10,9,8 
		now_movedate=ifctele_getinfestedTime(now_element)-i; //����ȯ�� ��Һ� ���� 
		
		for(j=3;j<N_HISTORY;j++){ 
			ifct_movedate=ifctele_getinfestedTime(ifct_element)-(4-j);//��ȯ�� ��Һ� ���� 
			if(ifctele_getHistPlaceIndex(now_element, convertTimeToIndex(now_movedate,ifctele_getinfestedTime(now_element)))==ifctele_getHistPlaceIndex(ifct_element, j)){
				if(now_movedate==ifct_movedate){
					people=ifctele_getIndex(ifct_element); //����ȯ�ڿ��� ������ ��� 
					metdate=now_movedate; //����ȯ�ڿ� �����ڰ� �������� 
					flag++;
				}
					
			}							
		
			else{
				continue;
				}
						
		}	
	}
					
	if(flag==0){
		metdate=-1;
	}
									
	return metdate; //���� ���� ��ȯ 
}

int trackInfecter(int innumber2){ //�����ڸ� ã�ƿ� 
	int i;
	int j;
	
	void *ifct_element;
	void *now_element;
	
	int mettime2;
	int firstpeople1;
	
	
	for(i=0;i<ifctdb_len();i++){//�񱳴�� ȯ�� �ݺ��� 
		ifct_element=ifctdb_getData(i);					
		mettime2=isMet(innumber2,ifctele_getIndex(ifct_element)); //����ȯ���� ������ ���� �� ã��	
											
		if(mettime2>0){ //�����ٸ� 		
			firstpeople1=i; //������ 
			break;		
		}
		
		else{
			firstpeople1=-1; //�����ڰ� ���� ���(����������) 
		}		
	}
	
	return firstpeople1;
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
    
	//�����ڵ�
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
		   	printf("%i ��° ȯ�� ���� ��� : ", pIndex+1);
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
    char metplace;
    
    int mettime2;
    
    int now_movedate;
    int ifct_movedate;
   
    int people;
    int flag;

	int age_min;
	int age_max;
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
            	printf("ȯ�� ��ȣ �Է�:");
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
                printf("��� �Է�:");
            	scanf("%s",&inplace);
            //	int i;
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
                printf("������ �ּڰ� �Է�:");
                scanf("%i",&age_min);              
                printf("������ �ִ� �Է�:");
				scanf("%i",&age_max); 
//				int i;
				for(i=0;i<ifctdb_len();i++){
					ifct_element=ifctdb_getData(i);
					if(age_min<=ifctele_getAge(ifct_element)){
						if(age_max>=ifctele_getAge(ifct_element)){
							printf("age: %i\n",ifctele_getAge(ifct_element));
							printf("time: %i\n",ifctele_getinfestedTime(ifct_element));
							
							int j;
							for(j=0;j<N_HISTORY;j++){
								printf("move: %s\n",ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element,j)));
							}
						}
					}
				}
				
            	break;
                
            case MENU_TRACK:
            	//������ ���ڸ� �������� �����ڿ� �������� ���� �� ��Ҹ� ������ ���, ���������� ��� 
            	printf("����ȯ�ڸ� �Է��Ͻÿ�:");
            	scanf("%i",&innumber2); 
		
				while(innumber2!=-1){
					now_element=ifctdb_getData(innumber2);//����ȯ�ڿ� ���� ������
					firstpeople=trackInfecter(innumber2); //������ 
					
					if(firstpeople!=-1){
						printf("%i��ȯ�ڴ� %i�� ȯ�ڿ��� ���ĵ�\n",innumber2,firstpeople);
						int i;
						for(i=0;i<ifctdb_len();i++){ //�������� ������ ��� ����� ���� �ݺ��� 
							ifct_element=ifctdb_getData(i);
							metdate=isMet(innumber2,i);
	
							if(metdate>0){ //�����ٸ� 	
							
								printf("%i�Ͽ� %s���� ������\n",metdate,ifctele_getPlaceName(ifctele_getHistPlaceIndex(now_element,convertTimeToIndex(metdate,ifctele_getinfestedTime(now_element)))));
								break;		
							}
		
							else{
								firstpeople1=-1; //�����ڰ� ���� ���(����������) 
							}		
						}
						
						innumber2=firstpeople;
					}
					else{
						firstpeople=innumber2;
						innumber2=-1; //����������
						printf("%i�� ȯ�ڴ� �����������̴�\n",firstpeople); 
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

