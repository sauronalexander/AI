//mp3 part2

#include <iostream>
#include <cmath>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <map>


using namespace std;

bool test_MNB(map<string, double> & probability_normal, map<string, double> & probability_spam, map<string, double> & frequency, int N, vector<vector<int> > matrix, int & act_norm, int & act_spam);
void Multinomial_Naive_Bayes(map<string, pair<double, double> > & dict, int normal, int spam, int num_normal, int num_spam);

/*-------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------MNB------------------------------------------------------------*/
bool test_MNB(map<string, pair<double, double> > & dict, double prob_spam, double prob_normal, map<string, double> & frequency, int N)
{
	//the probability of spam for current email
	double p_normal = 0;
	double p_spam = 0;
	//the probability of spam for current email is the sum of P(spam|wordi)P(wordi)
	for(map<string, double>::iterator it = frequency.begin(); it != frequency.end(); it++)
	{
		if(dict.find(it->first) != dict.end())
			p_normal += it->second * log(dict[it->first].first);
		if(dict.find(it->first) != dict.end())
			p_spam += it->second * log(dict[it->first].second);
	}
	p_normal += log(prob_normal);
	p_spam += log(prob_spam);
	//cout<<p_normal<<";"<<p_spam<<endl;
	if(p_spam>p_normal)
		return true;
	else
		return false;
}

void Multinomial_Naive_Bayes(map<string, pair<double, double> > & dict, int normal, int spam, int num_normal, int num_spam, vector<vector<int> > & matrix, int & act_norm, int & act_spam)
{
	double prob_normal = double(normal)/double(normal+spam);
	double prob_spam = double(spam)/double(normal+spam);
	//probability of spam for each word, i.e.P(spam|wordi)
	//P(spam|wordi) = P(wordi|spam)P(spam)/(P(wordi|spam)P(spam)+P(wordi|normal)P(normal))
	//read the file
	ifstream myfile("test_email.txt");
	

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
	
	//this is to map the frequency of a specific word in the email
	map<string, double> frequency;
	//check for each email
	int dis_match = 0, match = 0;
	int N=0; //word count of current email
	for(int i=0; i<=lines.size(); i++)
	{
		bool flag; //true for spam. flase for normal
		if(!frequency.empty())
		{
			if(flag && test_MNB(dict, prob_spam, prob_normal, frequency, N))
			{
				act_spam++;
				matrix[1][1]++;
				match++;
			}
			else if(!flag && !test_MNB(dict, prob_spam, prob_normal, frequency, N))
			{
				act_norm++;
				matrix[0][0]++;
				match++;
			}
			else if(flag && !test_MNB(dict, prob_spam, prob_normal, frequency, N))
			{
				act_spam++;
				matrix[1][0]++;
				dis_match++;
			}
			else
			{
				act_norm++;
				matrix[0][1]++;
				dis_match++;
			}
		}
		frequency.clear();
		N = 0;
		//check the label
		if(i == lines.size())
			break;
		if(lines[i][0] == '1')
			flag = true;
		else
			flag = false;


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

	ifstream myfile("train_email.txt");
	

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

	int spam=0, normal=0; //number of spam emails and normal emails.
	int num_spam=0, num_normal=0; //number of words in spam and normal.
	
	//for Multinomial_Naive_Bayes
	//pair: normal, spam
	//probability or each word based on its class, i.e., P(wordi|spam) and P(wordi|normal)
	map<string, pair<double, double> > dict;
	//for Bernoulli_Naive_Bayes
	map<string, pair<double, double> > dict2;
	for(int i=0; i<lines.size(); i++)
	for(int j=0; j<lines[i].size(); j++)
	{
		//check the label;
		bool flag;
		if(j==0 && lines[i][j] == '0') //normal
		{
			flag = true;
			j += 2;
			normal++;
		}

		else if(j==0 && lines[i][j] == '1') //spam
		{
			flag = false;
			j += 2;
			spam++;
		}

		string word;
		while(lines[i][j] != ' ' &&lines[i][j] != ':')
		{
			word += lines[i][j++];
		}
		//push back the numbers;
		if(lines[i][j] == ':')
		{
			j++; //count of current word
			//if word exists
			string cur_num;
			while(lines[i][j] <='9' && lines[i][j] >= '0')
				cur_num += lines[i][j++];
			int current=0;
			for(int k=0; k<cur_num.size(); k++)
			{
				current *= 10;
				current += (cur_num[k] - '0');
			}
			if(dict.find(word) != dict.end())
			{
				if(flag)
				{
					num_normal += current;
					dict[word].first += current;
				}
				else
				{
					num_spam += current;
					dict[word].second += current;
				}
			}
			//if word does not exist
			else
			{
				if(flag)
				{
					num_normal += current;
					dict.insert(dict.end(), pair<string, pair<double, double> > (word,  pair<double, double> (current, 0)));
				}
				else
				{
					num_spam += current;
					dict.insert(dict.end(), pair<string, pair<double, double> > (word,  pair<double, double> (0, current)));
				}	
			}
			//for BNB
			if(dict2.find(word) != dict2.end())
			{
				if(flag)
				{
					dict2[word].first++;
				}
				else
				{
					dict2[word].second++;
				}
			}
			//if word does not exist
			else
			{
				if(flag)
				{
					dict2.insert(dict2.end(), pair<string, pair<double, double> > (word,  pair<int, int> (1, 0)));
				}
				else
				{
					dict2.insert(dict2.end(), pair<string, pair<double, double> > (word,  pair<int, int> (0, 1)));
				}	
			}
		}
	}
	
	cout<<"top twenty words for normal:"<<endl;
	vector<string> normal_twenty;
	for(map<string, pair<double, double> >::iterator it = dict.begin(); it != dict.end(); it++)
	{
		if(normal_twenty.size() < 20)
			normal_twenty.push_back(it->first);
		else
			for(int i=0; i<20; i++)
				if((it->second.first)>(dict[normal_twenty[i]].first))
				{
					normal_twenty.erase(normal_twenty.begin()+i);
					normal_twenty.insert(normal_twenty.begin()+i, it->first);
					break;
				}
	}
	for(int i=0; i<normal_twenty.size(); i++)
	{
		cout<<normal_twenty[i]<<" : "<<dict[normal_twenty[i]].first<<endl;
	}

	cout<<"top twenty words for spam:"<<endl;
	vector<string> spam_twenty;
	for(map<string, pair<double, double> >::iterator it = dict.begin(); it != dict.end(); it++)
	{
		if(spam_twenty.size() < 20)
			spam_twenty.push_back(it->first);
		else
			for(int i=0; i<20; i++)
				if((it->second.second)>(dict[spam_twenty[i]].second))
				{
					spam_twenty.erase(spam_twenty.begin()+i);
					spam_twenty.insert(spam_twenty.begin()+i, it->first);
					break;
				}
	}
	for(int i=0; i<spam_twenty.size(); i++)
	{
		cout<<spam_twenty[i]<<" : "<<dict[spam_twenty[i]].second<<endl;
	}
	
	//update the probability table.
	for(map<string, pair<double, double> >::iterator it = dict.begin(); it != dict.end(); it++)
	{
		it->second.first = (it->second.first+1) / double(num_normal + dict.size());
		it->second.second = (it->second.second+1) / double(num_spam + dict.size());
	}

	for(map<string, pair<double, double> >::iterator it = dict2.begin(); it != dict2.end(); it++)
	{
		it->second.first = (it->second.first+1) / double(normal + 2);
		it->second.second = (it->second.second+1) / double(spam + 2);
	}
	int act_norm = 0, act_spam = 0;
	vector<vector<int> > matrix;
	vector<int> temp;
	temp.push_back(0);
	temp.push_back(0);
	matrix.push_back(temp);
	matrix.push_back(temp);
	cout<<endl<<"Multinomial_Naive_Bayes"<<endl;
	Multinomial_Naive_Bayes(dict, normal, spam, num_normal, num_spam, matrix, act_norm, act_spam);
	cout<<"confusion matrix:"<<endl;
	cout<<double(matrix[0][0])/double(act_norm) <<" "<<double(matrix[0][1])/double(act_norm)<<endl;
	cout<<double(matrix[1][0])/double(act_spam)<<" "<<double(matrix[1][1])/double(act_spam)<<endl;
	///double(normal+spam)
	matrix[0][0] = 0;
	matrix[0][1] = 0;
	matrix[1][0] = 0;
	matrix[1][1] = 0;
	act_norm =0;
	act_spam = 0;
	cout<<endl<<"Bernoulli_Naive_Bayes"<<endl;
	Multinomial_Naive_Bayes(dict2, normal, spam, num_normal, num_spam, matrix, act_norm, act_spam);
	//Bernoulli_Naive_Bayes(dict2, normal, spam, num_normal, num_spam);
	cout<<"confusion matrix:"<<endl;
	cout<<double(matrix[0][0])/double(act_norm) <<" "<<double(matrix[0][1])/double(act_norm)<<endl;
	cout<<double(matrix[1][0])/double(act_spam)<<" "<<double(matrix[1][1])/double(act_spam)<<endl;

	return 0;
	
}

