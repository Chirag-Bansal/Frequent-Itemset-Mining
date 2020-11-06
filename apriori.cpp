#include <iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<map>
#include<fstream>
#include<sstream>
#include<set>
using namespace std;

class Candidate{
public:
    set<int> itemset;
    float freq;
    int count;
};
bool check_sub(set<int> itemset, int i, vector<Candidate> freq_set) {
    itemset.erase(i);
    bool result = false;
    for(auto candidate: freq_set) {
        if (candidate.itemset == itemset) {
            result = true;
            break;
        }
    }
    return result;
}

vector<Candidate> gen_cand(vector<Candidate> freq_set){
    vector<Candidate> lk;

    for(auto i1=freq_set.begin(); i1!=freq_set.end();i1++){

        for(auto i2=i1+1;i2!=freq_set.end();i2++){

            set<int> difference;
            set_difference(i2->itemset.begin(), i2->itemset.end(), i1->itemset.begin(), i1->itemset.end(), inserter(difference, difference.end()));

            if(difference.size()==1){
                Candidate cd;
                cd.count=0;
                cd.freq=0;
                set_union(i1->itemset.begin(), i1->itemset.end(), difference.begin(), difference.end(),inserter(cd.itemset,cd.itemset.end()));

                bool res=true;
                for(auto &ex:cd.itemset){
                    res=check_sub(cd.itemset,ex,freq_set);
                    if(res==false){
                        break;
                    }

                }
                if(res==true){
                    bool result = true;
                    for(auto &val : lk) {
                        if (val.itemset == cd.itemset){
                            result = false;
                            break;
                        }
                    }
                    if(result==true){
                        lk.push_back(cd);
                    }
                }

            }
        }
    }
    return lk;
}

void apriori(string infile, string outfile, float threshold){
    ifstream input(infile);
    map<int,float> mp1;
    long long int total_count =0;
    int val;
    string str;

    //reading the input
    while(getline(input, str)){
        total_count++;
        istringstream values_gen(str);
        while(values_gen>>val){
                   if(mp1[val]>0){
                       mp1[val]++;
                   }else{
                       mp1[val]=1;
                   }
        }
    }
    vector<vector<Candidate>> freq_sets;
    vector<Candidate> c1;
    for(auto it=mp1.begin();it!=mp1.end();it++){

         float frequency= ((float )it->second)/ total_count;
         if(frequency>=threshold){
             Candidate cand;
             cand.itemset.insert(it->first);
             cand.freq=frequency;

             cand.count=it->second;
             c1.emplace_back(cand);
         }
    }

    freq_sets.push_back(c1);
    vector<Candidate> a;

    while(true){

        vector<Candidate> Ck=gen_cand(freq_sets.at(freq_sets.size()-1));
        input.clear();
        input.seekg(0, input.beg);
        while(getline(input,str)){

            istringstream values_gen(str);
            set<int> set;
            int x;
            while(values_gen>>x){
                set.insert(x);
            }
            for(Candidate &cn : Ck) {
                if(includes(set.begin(), set.end(), cn.itemset.begin(), cn.itemset.end())) {
                    cn.count++;
                }
            }

        }

        vector<Candidate> freq_set;

        for(auto &cn: Ck) {
            cn.freq = ((float)cn.count)/total_count;
            if (cn.freq >=threshold) {
                freq_set.push_back(cn);
            }

        }




       if(freq_set.size()!=0) {
           a = freq_set;
           freq_sets.push_back(a);

       }
        if(freq_set.size() == 0){
        /*   for(int i=0; i<a.size();i++){
                Candidate cn=a.at(i);
                for(auto it=cn.itemset.begin();it!=cn.itemset.end();it++){
                    cout<<*it<<" ";
                }
                cout<<endl;
            }
            */
            break;
        }






    }
    ofstream output(outfile);
   /* cout<<"########"<<endl;
   // cout<<"Size:"<<freq_set.size()<<endl;
    for(auto &freq_set : freq_sets) {
  //      cout<<"Size:"<<freq_set.size()<<endl;
        for(auto &cand : freq_set) {
            for(auto &item : cand.itemset) {
                output<< item << " ";
                cout<<item<<" ";
            }
            cout<<endl;
        }

    }
    cout<<"########"<<endl;
    */
    for(auto &freq_set: freq_sets){
        for(auto &cand:freq_set){
            vector<string> strvec;
            for(auto &item:cand.itemset){
                strvec.push_back(to_string(item));
            }
            sort(strvec.begin(), strvec.end());
            for(int i=0;i< strvec.size();i++){
                output<<strvec.at(i)<<" ";
            }
            output<<endl;
        }

    }


  /*  for(int i=0;i<a.size();i++){
        Candidate c= a.at(i);
        vector<string> strvec;
        for(auto &item: c.itemset){
            strvec.push_back(to_string(item));
        }
        sort(strvec.begin(), strvec.end());
        for(int i=0;i< strvec.size();i++){
            output<<strvec.at(i)<<" ";
        }
        output<<endl;
    }
    */
    output.close();
    input.close();


}
int main(int argc, char* argv[]) {

    double threshold = atoi(argv[2]) * 1.0 / 100;
    string infile = argv[1];
    string outfilename = argv[3];
    apriori(infile, outfilename, threshold);
    return 0;
}
