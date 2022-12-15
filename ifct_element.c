//
//  ifs_element.c
//  InfestPath
//
//  Created by Juyeop Kim on 2020/10/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ifct_element.h"

typedef enum place {
    Seoul,          //0
    Jeju,           //1
    Tokyo,          //2
    LosAngeles,     //3
    NewYork,        //4
    Texas,          //5
    Toronto,        //6
    Paris,          //7
    Nice,           //8
    Rome,           //9
    Milan,          //10
    London,         //11
    Manchester,     //12
    Basel,          //13
    Luzern,         //14
    Munich,         //15
    Frankfurt,      //16
    Berlin,         //17
    Barcelona,      //18
    Madrid,         //19
    Amsterdam,      //20
    Stockholm,      //21
    Oslo,           //22
    Hanoi,          //23
    Bangkok,        //24
    KualaLumpur,    //25
    Singapore,      //26
    Sydney,         //27
    SaoPaulo,       //28
    Cairo,          //29
    Beijing,        //30
    Nairobi,        //31
    Cancun,         //32
    BuenosAires,    //33
    Reykjavik,      //34
    Glasgow,        //35
    Warsow,         //36
    Istanbul,       //37
    Dubai,          //38
    CapeTown        //39
} place_t;

char countryName[N_PLACE+1][MAX_PLACENAME] =
{   "Seoul",
    "Jeju",
    "Tokyo",
    "LosAngeles",
    "NewYork",
    "Texas",
    "Toronto",
    "Paris",
    "Nice",
    "Rome",
    "Milan",
    "London",
    "Manchester",
    "Basel",
    "Luzern",
    "Munich",
    "Frankfurt",
    "Berlin",
    "Barcelona",
    "Madrid",
    "Amsterdam",
    "Stockholm",
    "Oslo",
    "Hanoi",
    "Bangkok",
    "KualaLumpur",
    "Singapore",
    "Sydney",
    "SaoPaulo",
    "Cairo",
    "Beijing",
    "Nairobi",
    "Cancun",
    "BuenosAires",
    "Reykjavik",
    "Glasgow",
    "Warsow",
    "Istanbul",
    "Dubai",
    "CapeTown",
    "Unrecognized"
};

char* ifctele_getPlaceName(int placeIndex){
	return countryName[placeIndex];
}

typedef struct ifs_ele{
	int number;
	int age;
	int date;
	place_t move[N_HISTORY]; //***
}ifs_ele_t;


void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY]){
	ifs_ele_t *ptr;
	ptr=malloc(sizeof(ifs_ele_t));

	ptr->number=index;
	ptr->age=age;
	ptr->date=detected_time;
	int i=0;
	for(i=0;i<N_HISTORY;i++){
		ptr->move[i]=history_place[i];
	}
	
	return ptr; 
	//return (void*)&ifsarray[ifs_cnt]; //동적메모리 할당할 때 사용 
}
//main.c에서 정보가 들어오면 ifctele_genElement에서 구조체에 저장 


void ifctele_printElement(void* obj){
	ifs_ele_t* strPtr=(ifs_ele_t*)obj;

	int j;
	printf("%i",strPtr->number);
	printf("%i",strPtr->age);
	printf("%i",strPtr->date);
		
	for(j=0;j<N_HISTORY;j++){
		printf("%s", countryName[strPtr->move[j]]);
	}
			
}

int ifctele_getIndex(void* obj){
	ifs_ele_t* strPtr=(ifs_ele_t*)obj;
	return strPtr->number;
}

int ifctele_getAge(void* obj){
	ifs_ele_t* strPtr=(ifs_ele_t*)obj;
	return strPtr->age;
}

unsigned int ifctele_getinfestedTime(void* obj){
	ifs_ele_t* strPtr=(ifs_ele_t*)obj;
	return strPtr->date;
}

int ifctele_getHistPlaceIndex(void* obj, int index){
	ifs_ele_t* strPtr=(ifs_ele_t*)obj;
	return strPtr->move[index]; //배열 
}

