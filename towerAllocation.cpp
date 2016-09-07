#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;

double bestAmount;
double currentAmount;

struct nodeBid{			//bid node structure
	int bidId;			//bid's id
	int companyId;		//company's id
	double bidAmount;	//cost of bid
	vector<int> regionIdList;		//number of regions in bid
};


//function to get possible sets of bids.
vector<int> getPossibleState(vector<nodeBid> listOfBids, vector< vector<int> > listCompanyBids, vector< vector<int> > listRegionBids, vector<int> selectedBids, int status){
	vector<int> possibleBids;
	currentAmount = 0;
	for(int i=0; i<listOfBids.size(); i++){		//for new start
		possibleBids.push_back(i);
	}
	if(status==0){		//if some bids already selected
		vector<int> tempSet=selectedBids;

		//will remove all the other bids which are not possible with given selected bids.
		while(!tempSet.empty()){	
			int lastBid = tempSet.back();
			currentAmount += listOfBids[lastBid].bidAmount;
			possibleBids.erase(remove(possibleBids.begin(), possibleBids.end(), lastBid), possibleBids.end());
			vector<int> bidsToRemove = listCompanyBids[listOfBids[lastBid].companyId];

			//will remove all the other bids of the companies whose one bid already been selected
			while(!bidsToRemove.empty()){
				possibleBids.erase(remove(possibleBids.begin(), possibleBids.end(), bidsToRemove.back()), possibleBids.end());
				bidsToRemove.pop_back();
			}

			vector<int> regionsToRemove = listOfBids[lastBid].regionIdList;

			//will remove all bids which contains the regions which already be in the selected bids
			while(!regionsToRemove.empty()){
				bidsToRemove = listRegionBids[regionsToRemove.back()];

				while(!bidsToRemove.empty()){
					possibleBids.erase(remove(possibleBids.begin(), possibleBids.end(), bidsToRemove.back()), possibleBids.end());
					bidsToRemove.pop_back();
				}
				regionsToRemove.pop_back();
			}
			tempSet.pop_back();
		}
	}

	//will run untill zero bid left in set of possible bids and after each loop select any random bid and remove all other bids which are not possible after selecting the bid
	while(!possibleBids.empty()){
		int randomIndex = rand()%possibleBids.size();
		int randomBid = possibleBids[randomIndex];		 
		possibleBids.erase (possibleBids.begin()+randomIndex);		
		selectedBids.push_back(randomBid);
		currentAmount += listOfBids[randomBid].bidAmount;
		vector<int> bidsToRemove = listCompanyBids[listOfBids[randomBid].companyId];

		//will remove all the other bids of the companies whose one bid already been selected
		while(!bidsToRemove.empty()){
			possibleBids.erase(remove(possibleBids.begin(), possibleBids.end(), bidsToRemove.back()), possibleBids.end());
			bidsToRemove.pop_back();
		}

		vector<int> regionsToRemove = listOfBids[randomBid].regionIdList;

		//will remove all bids which contains the regions which already be in the selected bids
		while(!regionsToRemove.empty()){
			bidsToRemove = listRegionBids[regionsToRemove.back()];
			while(!bidsToRemove.empty()){
				possibleBids.erase(remove(possibleBids.begin(), possibleBids.end(), bidsToRemove.back()), possibleBids.end());
				bidsToRemove.pop_back();
			}
			regionsToRemove.pop_back();
		}		
	}
	
	return selectedBids;
}

int main (){
	float time;		//time in mins
	int M,B,C;		//M,B and C are no. of regions, no. of bids and no. of companines

	cin>>time>>M>>B>>C;

	clock_t initTime = clock();
	vector<nodeBid> listOfBids;		//vector of type nodeBid
	vector< vector<int> > listCompanyBids;		//vector of vector int which store list of companies's bids corresponding to company's id.
	vector< vector<int> > listRegionBids;		//vector of vector int which store list of bids corresponding to region's id  
	int companyId;
	double bidAmount;
	int regionId;
	nodeBid temp;

	//initialize listRegionBids of size M
	for(int i=0; i<M; i++){
		vector<int> a;
		listRegionBids.push_back(a);
	}

	//initialize listofBids of size B
	for(int i=0; i<B; i++){
		nodeBid a;
		listOfBids.push_back(a);
	}

	//initialize listCompanyBids of size B
	for(int i=0; i<C; i++){
		vector<int> a;
		listCompanyBids.push_back(a);
	}

	int i=0;
	while(i<B){
		cin>>companyId;
		cin>>bidAmount;

		vector<int> regionIdList;		//list of regions in bid
		listCompanyBids[companyId].push_back(i);		//here "i" is bidId

		//all the regions in the bid
		while(cin>>regionId){
			regionIdList.push_back(regionId);
			listRegionBids[regionId].push_back(i);		//here "i" is bidId
		};

		cin.clear();
		cin.ignore();

		//construct tempory bid's node
		temp.bidId=i;
		temp.companyId=companyId;
		temp.bidAmount=bidAmount;
		temp.regionIdList=regionIdList;
		
		listOfBids[i] = temp;
		i++;
	}

	vector<int> selectedBids, bestBids;

	selectedBids = getPossibleState(listOfBids, listCompanyBids, listRegionBids, selectedBids, 1);		//get first random state(set of possible bids)
	bestBids = selectedBids;
	bestAmount = currentAmount;

	//run while loop till assign time is remaining
	while(time*60 -((float)(clock() - initTime)/CLOCKS_PER_SEC )>1){
		int randIndex = rand()%selectedBids.size();		//get random integer 

		if (randIndex==0){		//if randIndex is equal to 0 do fresh start
			vector<int> newStart;
			selectedBids = getPossibleState(listOfBids, listCompanyBids, listRegionBids, newStart, 1);		//1 and 0 for new state and neighbour state 
		}
		else{		//otherwise remove all the bids from selectedBids vector whose index equal to randIndex or greater than randIndex.
			int upto = selectedBids.size();
			selectedBids.erase (selectedBids.begin()+randIndex,selectedBids.begin()+upto);
			selectedBids = getPossibleState(listOfBids, listCompanyBids, listRegionBids, selectedBids, 0);
		}

		//compare two states
		if(currentAmount>bestAmount){
			bestBids = selectedBids;
			bestAmount = currentAmount;
		} 
	}

	//print best state that have been found
	for(int i =0; i<bestBids.size(); i++){
		cout << bestBids[i] << ' ';
	}

	cout<< '#';
	return 0;
}