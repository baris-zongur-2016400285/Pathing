#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
class Road{
public:
    int town1;   // vertexes
    int  town2;
    int walkAmount; // weight of edge
    vector<int> thives; // which group of thieves
    Road(int t1, int t2, int walkA){
        this->town1=t1;
        this->town2=t2;
        this->walkAmount = walkA;
    }
    void addThives(int th){
        this->thives.push_back(th);
    }
};
class Town{
public:
    int name; // name of the city "Im from the great city of 3" :)
    vector<Road*> roads; // roads connected to town
    vector<int> jewelers; // jewelers in town
    Town(int nm){
        this->name = nm;
    }
    void addRoad(Road *a){
        roads.push_back(a);
    }
    void addJewel(int jw){
        jewelers.push_back(jw);
    }
};
class Graph{
public:
    vector<Road> myRoads;
    vector<Town> myTowns;
    void initializeT(int twns){ //initialize towns
        for(int i=1;i<=twns;i++){
            myTowns.push_back((i));
        }
    }
    void initializeR(int tw1,int tw2,int wlk,vector<int> thv){ //initialize roads
        myRoads.push_back({tw1,tw2,wlk});
        for(int i=0;i<thv.size();i++){
            myRoads.back().addThives(thv[i]);
        }
        myRoads.push_back({tw2,tw1,wlk});
        for(int i=0;i<thv.size();i++){
            myRoads.back().addThives(thv[i]);
        }
    }
    void addJ(int twid, vector<int> jw){ // add jewelers to spesific town
        for (int i=0;i<jw.size();i++){
            myTowns[twid-1].addJewel(jw[i]);
        }
    }
    void finalIni(){ // for each town add roads connected to them
        for(int i=0;i<myTowns.size();i++){
            for (int j=0;j<myRoads.size();j++){
                if(myTowns[i].name==myRoads[j].town1||myTowns[i].name==myRoads[j].town2){
                    Road *thisR;
                    thisR=&myRoads[j];
                    myTowns[i].addRoad(thisR);
                }
            }
        }
    }
};
string shortPath(Graph myGraph,int numTowns, int startTown, int endTown);
string shortPath2(Graph myGraph,int numTowns, int startTown, int endTown,vector<int> jjews);
string shortPath2(Graph myGraph,int numTowns, int startTown, int endTown,vector<int> jjews){
    // nearly same one as shortPath2, just added this because i need to pass the jewels to recursive algorith
    vector<Town> myQ;
    vector<int> jewels;
    for(int i=0;i<jjews.size();i++){
        jewels.push_back(jjews[i]);
    }
    string path[1000];
    bool isPassed[numTowns+1];
    int cost[numTowns+1];
    path[startTown]="";
    isPassed[startTown]=false;
    cost[startTown]=0;
    for(int i=1;i<=numTowns;i++){
        if(i!=startTown) {
            path[i] = "";
            isPassed[i] = false;
            cost[i] = 10000000;
        }
    }
    myQ.push_back(myGraph.myTowns[startTown-1]);
    while(myQ.empty()==false){
        int minnum;
        int mincst=1000000;
        for(int i=0;i<myQ.size();i++){

            if(mincst>cost[myQ[i].name]) {
                if (isPassed[i] == false) {
                    minnum = i;
                    mincst=cost[myQ[i].name];
                }
            }
        }
        Town thTown = myQ[minnum];
        isPassed[thTown.name]=true;
        myQ.erase(myQ.begin()+minnum);
        int jewAdd=0;
        for(int i=0;i<thTown.jewelers.size();i++){
            int isIn=0;
            for(int j=0;j<jewels.size();j++){
                if(thTown.jewelers[i]==jewels[j]){
                    isIn++;
                }
            }
            if(isIn==0){
                jewels.push_back(thTown.jewelers[i]);
                jewAdd++;
            }
        }
        if(jewAdd>0){
            for(int i=0;i<myGraph.myRoads.size();i++){
                if(myGraph.myRoads[i].thives.size()!=0){
                    if(isPassed[myGraph.myRoads[i].town2]==false){
                        int jewCount=0;
                        for (int j = 0; j < myGraph.myRoads[i].thives.size(); j++) {
                            for (int k = 0; k < jewels.size(); k++) {
                                if (myGraph.myRoads[i].thives[j] == jewels[k]) {
                                    jewCount++;
                                }
                            }
                        }
                        if(jewCount==myGraph.myRoads[i].thives.size()){
                            string valStr=shortPath2(myGraph,myGraph.myTowns.size(),thTown.name,myGraph.myRoads[i].town2,jewels);
                            if(valStr[0]!='-'){
                                int eqPlace=0;
                                for(int k=0;k<valStr.size();k++){
                                    if(valStr[k]=='-'){
                                        eqPlace = k;
                                    }
                                }
                                int newCost=stoi(valStr.substr(eqPlace+1,valStr.size()));
                                if(cost[myGraph.myRoads[i].town2]>newCost+cost[thTown.name]){
                                    cost[myGraph.myRoads[i].town2]=newCost+cost[thTown.name];
                                    path[myGraph.myRoads[i].town2]=path[thTown.name]+valStr.substr(0,eqPlace);
                                    myQ.push_back(myGraph.myTowns[myGraph.myRoads[i].town2-1]);
                                }
                            }
                        }
                    }
                }
            }
        }
        for(int i=0;i<thTown.roads.size();i++){
            if(isPassed[thTown.roads[i]->town2]==false){
                if(cost[thTown.roads[i]->town2]>cost[thTown.roads[i]->town1]+thTown.roads[i]->walkAmount) {
                    int jewCount=0;
                    for (int j = 0; j < thTown.roads[i]->thives.size(); j++) {
                        for (int k = 0; k < jewels.size(); k++) {
                            if (thTown.roads[i]->thives[j] == jewels[k]) {
                                jewCount++;
                            }
                        }
                    }
                    if (jewCount == thTown.roads[i]->thives.size()) {
                        cost[thTown.roads[i]->town2] = cost[thTown.roads[i]->town1] + thTown.roads[i]->walkAmount;
                        path[thTown.roads[i]->town2] = path[thTown.roads[i]->town1] + to_string(thTown.roads[i]->town1) + "/";
                        myQ.push_back(myGraph.myTowns[thTown.roads[i]->town2-1]);
                    }
                }
            }
        }
    }
    return path[endTown]+"-"+to_string(cost[endTown]);
}
string shortPath(Graph myGraph,int numTowns, int startTown, int endTown){
    //djkstra algorith with little changes its so similar i will comment important changes
    //shortPath2 is nearly identical to this one, i will explain why i needed that
    vector<Town> myQ;
    vector<int> jewels;
    string path[1000];
    bool isPassed[numTowns+1];
    int cost[numTowns+1];
    path[startTown]="";
    isPassed[startTown]=false;
    cost[startTown]=0;
    for(int i=1;i<=numTowns;i++){
        if(i!=startTown) {
            path[i] = "";
            isPassed[i] = false;
            cost[i] = 10000000;
        }
        }
    myQ.push_back(myGraph.myTowns[startTown-1]);
    while(myQ.empty()==false){ // while my vector is not empty it proceesses like djkstra
        int minnum=0;
        int mincst=1000000;
        for(int i=0;i<myQ.size();i++){ // this for finds min cost in vector

                if(mincst>cost[myQ[i].name]){
                    minnum =i;
                    mincst=cost[myQ[i].name];
                }
        }
        Town thTown = myQ[minnum];
        isPassed[thTown.name]=true;
        myQ.erase(myQ.begin()+minnum);
        int jewAdd=0;
     for(int i=0;i<thTown.jewelers.size();i++){ // if at this town AliBaba get any new coins or not
         int isIn=0;
         for(int j=0;j<jewels.size();j++){
             if(thTown.jewelers[i]==jewels[j]){
                 isIn++;
             }
         }
         if(isIn==0){
             jewels.push_back(thTown.jewelers[i]);
             jewAdd++;
         }
     }
     if(jewAdd>0){ // if AliBaba finds a new coin, new roads are open for him
                    // just check what changed after that coin and update, doing this by if alibaba gets a new coin check
         // if you can go now to blocked roads and how can you get there by recursive algorithm
         for(int i=0;i<myGraph.myRoads.size();i++){
             if(myGraph.myRoads[i].thives.size()!=0){
                 if(isPassed[myGraph.myRoads[i].town2]==false){
                     int jewCount=0;
                     for (int j = 0; j < myGraph.myRoads[i].thives.size(); j++) {
                         for (int k = 0; k < jewels.size(); k++) {
                             if (myGraph.myRoads[i].thives[j] == jewels[k]) {
                                 jewCount++;
                             }
                         }
                     }
                     if(jewCount==myGraph.myRoads[i].thives.size()){
                         string valStr=shortPath2(myGraph,myGraph.myTowns.size(),thTown.name,myGraph.myRoads[i].town2,jewels);
                         if(valStr[0]!='-'){
                             int eqPlace=0;
                             for(int k=0;k<valStr.size();k++){
                                 if(valStr[k]=='-'){
                                     eqPlace = k;
                                 }
                             }
                             int newCost=stoi(valStr.substr(eqPlace+1,valStr.size()));
                             if(cost[myGraph.myRoads[i].town2]>newCost+cost[thTown.name]){
                                 cost[myGraph.myRoads[i].town2]=newCost+cost[thTown.name];
                                 path[myGraph.myRoads[i].town2]=path[thTown.name]+valStr.substr(0,eqPlace);
                                 myQ.push_back(myGraph.myTowns[myGraph.myRoads[i].town2-1]);
                             }
                         }
                     }
                 }

             }
         }
     }
     for(int i=0;i<thTown.roads.size();i++){//pretty much dijksta algorith just checks if any road is blocked
         if(isPassed[thTown.roads[i]->town2]==false){
            if(cost[thTown.roads[i]->town2]>cost[thTown.roads[i]->town1]+thTown.roads[i]->walkAmount) {
             int jewCount=0;
             for (int j = 0; j < thTown.roads[i]->thives.size(); j++) {
                 for (int k = 0; k < jewels.size(); k++) {
                     if (thTown.roads[i]->thives[j] == jewels[k]) {
                         jewCount++;
                     }
                 }
             }
             if (jewCount == thTown.roads[i]->thives.size()) {
                 cost[thTown.roads[i]->town2] = cost[thTown.roads[i]->town1] + thTown.roads[i]->walkAmount;
                 path[thTown.roads[i]->town2] = path[thTown.roads[i]->town1] + to_string(thTown.roads[i]->town1) +"/";
                 myQ.push_back(myGraph.myTowns[thTown.roads[i]->town2-1]);
             }
         }
         }
     }
    }
    return path[endTown]+"-"+to_string(cost[endTown]);
}
int main(int argc, char* argv[]) {
    vector<string> inputS;
    if(argc != 3){ //this part reads the input and checks if it is ok
        cout<<"terminal error"<<endl;
    }
    else {
        ifstream infile(argv[1]); //classic read line by line
        if (!infile.is_open()) {
            cout << "couldn't open file" << endl;
            return 0;
        } else {
            string s;
            while (getline(infile, s)) {
                inputS.push_back(s);
            }
        }
        infile.close();
    }
    //below this does initilize graph using functions in my classes
    int numofTowns;
    int numofRoads;
    int numofJews;
    vector<string> values;
    string nowuse="";
    for(int i=0;i<inputS[0].length();i++){
        if(inputS[0][i]!=' '){
            nowuse = nowuse +inputS[0][i];
        }
        else{
            values.push_back(nowuse);
            nowuse="";
        }
    }
    values.push_back(nowuse);
    numofTowns=stoi(values[0]);
    numofRoads=stoi(values[1]);
    numofJews=stoi(values[3]);
    Graph myGrph;
    myGrph.initializeT(numofTowns);
    vector<int> myNums;
    values.clear();
    nowuse="";
    for(int i=1;i<=numofJews;i++){
        myNums.clear();
        values.clear();
        nowuse="";
        for(int j=0;j<inputS[i].length();j++){
            if(inputS[i][j]!=' '){
                nowuse = nowuse +inputS[i][j];
            }
            else{
                values.push_back(nowuse);
                nowuse="";
            }
        }
        values.push_back(nowuse);
        int townName = stoi(values[0]);
        int loopCount = stoi(values[1]);
        for(int k=0;k<loopCount;k++){
            myNums.push_back(stoi(values[2+k]));
        }
        myGrph.addJ(townName,myNums);
    }
    myNums.clear();
    values.clear();
    nowuse="";
    for(int i=numofJews+1;i<=numofJews+numofRoads;i++){
        myNums.clear();
        values.clear();
        nowuse="";
        for(int j=0;j<inputS[i].length();j++){
            if(inputS[i][j]!=' '){
                nowuse = nowuse +inputS[i][j];
            }
            else{
                values.push_back(nowuse);
                nowuse="";
            }
        }
        values.push_back(nowuse);
        int town1 = stoi(values[0]);
        int town2 = stoi(values[1]);
        int walkAmount = stoi(values[2]);
        int loopCount = stoi(values[3]);
        for(int k=0;k<loopCount;k++){
            myNums.push_back(stoi(values[4+k]));
        }
        myGrph.initializeR(town1,town2,walkAmount,myNums);
    }
        myGrph.finalIni();
    string complete =shortPath(myGrph,numofTowns,1,numofTowns);
    string output="";
    for(int i=0;i<complete.size();i++){
        if(complete[i]=='-'){ // my original output must have to give cost to cause i used recursive algorith
            break;
        }
        stringstream ss;
        string out;
        ss<<complete[i];
        ss>>out;
        output = output + out;
    }
    if(complete[0]=='-'){
        output="-1";
    }
    else{
        output = output+to_string(numofTowns);
    }
    FILE *myfile;
   myfile = fopen(argv[2],"w");
    for(int i=0;i<output.size();i++){
        if(output[i]=='/'){
         fprintf(myfile," ");
        }
        else{
            fprintf(myfile,"%c",output[i]);
        }

    }
    fclose(myfile);
}