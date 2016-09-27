//mp3 part1

#include <iostream>
#include <cmath>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <map>

/* 5000 training digits, around 500 samples from each digit class. 
Each digit is of size 28x28, and the digits are concatenated together vertically.
' ' means a white (background) pixel, 
'+' means a gray pixel, and 
'#' means a black (foreground) pixel

*/

using namespace std;

class NB //NB stands for Naive Bayes Classification
{
	int i,j;//coordinate for every pixel location

	vector<float> prior;// for part 1.1. In Training, last line: You should also estimate the priors P(class) by the empirical frequencies of different classes in the training set.
	
	
	vector< vector<int> > trainset, testset ;
	//similar to dengfei's datalist idea
	//store set image, can be int to store ascii, or can be vector< vector<char> > trainset, testset ;


	void Train(const vector< vector<int> > & traindata, const vector<int> & ltrain);

	vector< vector<float> > prob_table; // for part 1.2, probability table for each class
// P(Fij = f | class) = (# of times pixel (i,j) has value f in training examples from this class) / (Total # of training examples from this class).



	vector<int> trainlist, testlist, classcount;

	vector<vector <float> > confusionMatrix;// for 1.1 evaluation and odd ratios confuction matrix
};


void NB::Train(ector< vector<int> > & traindata, vector<int> & ltrain)
{
	//train data is different from trainset, traindata will be formed in main with getline
	int chang = traindata.size();//length
	int kuan = traindata[0].size();//width
	int i,j;

	//Again fuckers:
	// P(Fij = f | class) = (# of times pixel (i,j) has value f in training examples from this class) / (Total # of training examples from this class).

	for(i=0; i < 10;i++)//10 because 0~9
	{
		vector<float> temp;
		for(j=0; j < kuan; j++){
			temp.push_back(0);
		}
		prob_table.push_back(temp);
		classcount.push_back(0);
	}
	for(i=0;i<chang;i++)
	{
		for(j=0;j<kuan;j++)
		{
			if(traindata[i][j]==1)
				prob_table[trainlist[j]][i]++;
		}
		classcount[trainlist[j]]++;
	}
	for(int i=0;i< 10 ;i++)//10 because 0~9
	{
		for(j=0,j<kuan;j++)
		{
			prob_table[j][i]=prob_table[j][i]/classcount[j];
		}
		prior.push_back(float(classcount[j]/kuan));
	}




}





//dengfei sets a list
// void set_list(vector< vector<int> > & images, vector<int> & labels, int row, int col, bool flag, bool overlap)
//

int main()
{
	int row = 28;
	int col = 28;



	ifstream myfile("testlabels");
	

	vector <string> lines;
	if(!myfile)
	{
		cout << "File does not exist." << endl;
	}

	string line;

	while(getline(myfile, line))
	{
		lines.push_back(line);
		line.clear();
	}
	   

}

