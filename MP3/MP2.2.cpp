//mp3 part2

#include <iostream>
#include <cmath>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

int test_MNB(map<string, vector<double> > & dict, map<string, double> & frequency, double prob[]);
void Multinomial_Naive_Bayes(map<string, vector<double> > & dict, int num[], int num_word[], vector<vector<int> > & matrix, vector<int> & act);

/*-------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------MNB------------------------------------------------------------*/
int test_MNB(map<string, vector<double> > & dict, map<string, double> & frequency, double prob[])
{
	//the probability of spam for current email
	double p[8] = {0};
	//the probability of spam for current email is the sum of P(spam|wordi)P(wordi)
	for(map<string, double>::iterator it = frequency.begin(); it != frequency.end(); it++)
	for(int i=0; i<8; i++)
	{
		if(dict.find(it->first) != dict.end())
		{
			p[i] += it->second * log ((dict[it->first])[i]*frequency[it->first]);
		}
	}
	for(int i=0; i<8; i++)
		p[i] += log(prob[i]);
	//find the maximum probability
	int k=0;
	double max_p=p[0];
	for(int i=0; i<8; i++)
		if(p[i]>max_p)
		{
			k=i;
			max_p = p[i];
		}
	return k;

}

void Multinomial_Naive_Bayes(map<string, vector<double> > & dict, int num[], int num_word[], vector<vector<int> > & matrix, vector<int> & act)
{
	double prob[8] = {0};
	int sum = 0;
	for(int i=0; i<8; i++)
		sum += num[i];
	for(int i=0; i<8; i++)
		prob[i] = double(num[i])/double(sum);
	//read the file
	ifstream myfile("8category.testing.txt");
	
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

	//this is to map the frequency of a specific word in the message
	map<string, double> frequency;
	
	//check for each massage
	int dis_match = 0, match = 0;
	int N=0; //word count of current message
	for(int i=0; i<=lines.size(); i++)
	{
		int label;
		//check the label;
		if(!frequency.empty())
		{
			matrix[label][test_MNB(dict, frequency, prob)]++;
			if(label == test_MNB(dict, frequency, prob))
				match++;
			else
				dis_match++;
		}
		if(i == lines.size())
			break;
		label = lines[i][0] - '0';
		act[label]++;
		N=0;
		frequency.clear();
		for(int j=2; j<lines[i].size(); j++)
		{
			//get the word
			string word;
			while(lines[i][j] != ' ' && lines[i][j] != ':')
			{
				word += lines[i][j];
				j++;
			}
			if(lines[i][j] == ':')
			{

				j++;
				string cur_num;
				while(lines[i][j] <='9' && lines[i][j] >= '0')
					cur_num += lines[i][j++];
				int current=0;
				for(int k=0; k<cur_num.size(); k++)
				{
					current *= 10;
					current += (cur_num[k] - '0');
				}
				//push back the numbers;
				frequency.insert(frequency.end(), pair<string, double> (word, current));
				N += current;
			}
		}
	}	
	cout<<"number of matched cases:"<<match<<','<<"number of unmatched cases:"<<dis_match<<endl;
}

/*-------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------main-----------------------------------------------------------*/
int main()
{

	ifstream myfile("8category.training.txt");
	

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
	//store the number of each classes
	int num[8]={0};
	int num_word[8]={0};
	
	//for Multinomial_Naive_Bayes
	//pair: normal, spam
	//probability or each word based on its class, i.e., P(wordi|spam) and P(wordi|normal)
	map<string, vector<double> > dict;
	//for Bernoulli_Naive_Bayes
	map<string, vector<double> > dict2;
	
	for(int i=0; i<lines.size(); i++)
	{
		//check the label;
		int label = lines[i][0] - '0';
		num[label]++;
		for(int j=2; j<lines[i].size(); j++)
		{
			//get the word
			string word;
			while(lines[i][j] != ' ' &&lines[i][j] != ':')
			{
				word += lines[i][j++];
			}
			//push back the numbers;
			if(lines[i][j] == ':')
			{
				j++; //count of current word
				//get the number
				string cur_num;
				while(lines[i][j] <='9' && lines[i][j] >= '0')
					cur_num += lines[i][j++];
				int current=0;
				for(int k=0; k<cur_num.size(); k++)
				{
					current *= 10;
					current += (cur_num[k] - '0');
				}
				num_word[label] += current;
				//if word exists
				if(dict.find(word) != dict.end())
				{
					(dict[word])[label] += current;
				}
				//if word does not exist
				else
				{
					vector<double> a;
					for(int k=0; k<8; k++)
						a.push_back(0);
					a[label] = double(current);
					dict.insert(dict.end(), pair<string, vector<double> > (word, a));
				}
				//for BNB
				if(dict2.find(word) != dict2.end())
				{
					(dict2[word])[label]++;
				}
				//if word does not exist
				else
				{
					vector<double> a;			
					for(int k=0; k<8; k++)
						a.push_back(0);
					a[label] = 1;
					dict2.insert(dict2.end(), pair<string, vector<double> > (word, a));				
				}		
			}
		}
	}
	//print the top twenty words for each class;
	vector<string> twenty;
	for(int i=0; i<8; i++)
	{
		twenty.clear();
		cout<<"top twenty words for class "<<i<<":"<<endl;
		for(map<string, vector<double> >::iterator it = dict.begin(); it != dict.end(); it++)
		{
			if(twenty.size()<20)
				twenty.push_back(it->first);
			else
				for(int j=0; j<20; j++)
					if((it->second)[i] > (dict[twenty[j]])[i])
					{
						twenty.erase(twenty.begin()+j);
						twenty.insert(twenty.begin()+j, it->first);
						break;
					}
		}
		for(int j=0; j<20; j++)
			cout<<twenty[j]<<" : "<<(dict[twenty[j]])[i]<<endl;
	}

	//update the probability table.	
	for(map<string, vector<double> >::iterator it = dict.begin(); it != dict.end(); it++)
	for(int j=0; j<8; j++)
	{
		(it->second)[j] = ((it->second)[j]+1) / double(num_word[j] + dict.size());
	}
	
	for(map<string, vector<double> >::iterator it = dict2.begin(); it != dict2.end(); it++)
	for(int j=0; j<8; j++)
		(it->second)[j] = ((it->second)[j]+1) / double(num[j] + 2);
	
	vector<vector<int> > matrix;
	vector<int> act;
	for(int i=0; i<8; i++)
	{
		act.push_back(0);
		vector<int> temp;
		for(int j=0; j<8; j++)
			temp.push_back(0);
		matrix.push_back(temp);
	}

	cout<<endl<<"Multinomial_Naive_Bayes"<<endl;
	Multinomial_Naive_Bayes(dict, num, num_word, matrix, act);
	for(int i=0; i<8; i++)
	{
		for(int j=0; j<8; j++)
		{
			cout.precision(4);
			cout<<double(matrix[i][j]) / double(act[i])<<"  ";
			matrix[i][j] = 0;
		}
		cout<<endl;
		act[i] = 0;
	}
	cout<<endl<<"Bernoulli_Naive_Bayes"<<endl;
	Multinomial_Naive_Bayes(dict2, num, num_word, matrix, act);
	for(int i=0; i<8; i++)
	{
		for(int j=0; j<8; j++)
		{
			cout<<double(matrix[i][j]) / double(act[i])<<"  ";
			matrix[i][j] = 0;
		}
		cout<<endl;
		act[i] = 0;
	}
	dict.clear();
	dict2.clear();

	return 0;
	
}
