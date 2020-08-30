#include <bits/stdc++.h>
#define ll long long
#define ull unsigned long long
#define FIO ios_base::sync_with_stdio(0); cin.tie(0);
#define mod 1000000007
using namespace std;
 
enum CardType {CLUB,DIAMOND,HEART,SPADES};
 
class Card{
	int number;
	CardType ct;
 public:
	Card(int num,CardType type){
		number = num;
		ct = type;
	}
	bool operator<(const Card& card)const{
		int a,b;
		a = number==1?14:number; 
		b = card.getNumber()==1?14:card.getNumber();
		return a < b;
	}
	bool operator > (const Card& card){
		int a,b;
		a = this->number==1?14:this->number; 
		b = card.number==1?14:card.number;
		return a > b;
	}
	int getNumber()const{
		return number;
	}
	int getVal(){
		return number==1?14:number;
	}
	CardType getType(){
		return ct;
	}
};
 
class Hand{
	vector<Card> cards;
 public:
	void addCard(Card cd){
		cards.push_back(cd);
	}
	int getScore(){
		sort(cards.begin(), cards.end());
		if(cards[0].getNumber()==cards[1].getNumber() && cards[1].getNumber()==cards[2].getNumber()) return 1;
		if(cards[0].getNumber()==cards[1].getNumber()-1 && cards[1].getNumber()==cards[2].getNumber()-1) return 2;
		if(cards[0].getNumber()==cards[1].getNumber() || cards[1].getNumber()==cards[2].getNumber()) return 3;
		return 4;
	}
	int getMax(){
		int mx=INT_MIN;
		for(auto x:cards){	
			mx=max(mx,x.getVal());
		}
		return mx;
	}
};
 
class People{
	int id;
	Hand hand;
 public:
	People(int idd){
		this->id = idd;
	}
	void addCardToHand(Card cd){
		hand.addCard(cd);
	}
	int getId(){
		return id;
	}
	Hand getHand(){
		return hand;
	}
};
 
vector<Card> getShuffledDeck(){
	vector<Card> deck;
	multiset<Card> tmpDeck;
	set<int> nums;
	for(int i=1;i<=13;i++) nums.insert(i);
 
	set<int> tmp;
	//CLUB 0
	tmp = nums;
	for(int i=1;i<=13;i++){
		auto it = tmp.begin();
		advance(it,rand()%tmp.size());
		tmpDeck.insert(Card(*it,CLUB));
		tmp.erase(it);
	}
 
	//DIAMOND 1
	tmp = nums;
	for(int i=1;i<=13;i++){
		auto it = tmp.begin();
		advance(it,rand()%tmp.size());
		tmpDeck.insert(Card(*it,DIAMOND));
		tmp.erase(it);
	}
 
	//HEART 2
	tmp = nums;
	for(int i=1;i<=13;i++){
		auto it = tmp.begin();
		advance(it,rand()%tmp.size());
		tmpDeck.insert(Card(*it,HEART));
		tmp.erase(it);
	}
 
	//SPADES 3
	tmp = nums;
	for(int i=1;i<=13;i++){
		auto it = tmp.begin();
		advance(it,rand()%tmp.size());
		tmpDeck.insert(Card(*it,SPADES));
		tmp.erase(it);
	}
	while(tmpDeck.size()){
		auto it = tmpDeck.begin();
		advance(it,rand()%tmpDeck.size());
		deck.push_back(*it);
		tmpDeck.erase(it);
	}
 
	return deck;
}
 
People getWinner(vector<People> &pep,vector<Card> Deck,int dp){
	vector<int> scores(pep.size());
	for(int i=0;i<pep.size();i++){
		scores[i] = pep[i].getHand().getScore();
	}
 
	int mn = *min_element(scores.begin(), scores.end());
 
	vector<People> tiedPeople;
 
	//removing hands with less score
	for(int i=0;i<pep.size();i++) if(scores[i]==mn) tiedPeople.push_back(pep[i]);
 
	if(tiedPeople.size()==1) return tiedPeople[0];
 
	int topCardVal=INT_MIN;
	for(int i=0;i<tiedPeople.size();i++){
		topCardVal = max(topCardVal,tiedPeople[i].getHand().getMax());
	}
	//removing with lower top card in case of tie
	vector<People> tiedTopCardPep;
	for(int i=0;i<tiedPeople.size();i++) if(tiedPeople[i].getHand().getMax()==topCardVal) tiedTopCardPep.push_back(tiedPeople[i]);
 
	if(tiedTopCardPep.size()==1) return tiedTopCardPep[0];
 
	vector<People> tied(tiedTopCardPep);
 
	//now keep distributing until someone is winner
	while(dp<52){
		vector<Card> lastDraw;
		for(int i=0;i<tied.size();i++){
			lastDraw.push_back(Deck[dp++]);
		}
 
		int mx;
		for(auto x:lastDraw) mx = max(x.getNumber(),mx);
 
		vector<People> temp;
		//removing the ones with less value in current draw
		for(int i=0;i<tied.size();i++) if(lastDraw[i].getNumber()==mx) temp.push_back(tied[i]);
 
		if(temp.size()==1) return temp[0];
		tied = temp;
	}
 
	return People(-1);//No winnner
}
 
void display(vector<Card> deck){
	for(int i=0;i<deck.size();i++) cout<<deck[i].getNumber()<<' '<<deck[i].getType()<<'\n';
}
 
int main() {
 
	srand(time(0));
 
	vector<Card> Deck = getShuffledDeck();
	display(Deck);
 
	int numOfPep = 4;
	vector<People> people;
 
	//initilializing people
	for(int i=0;i<numOfPep;i++)	people.push_back(People(i));
 
	//distributing a hand
	int dp=0,p=0;
	while(dp<numOfPep*3){
		people[p].addCardToHand(Deck[dp]);
		p = (p+1)%numOfPep;
		dp++;
		if(dp==52) exit(1);
	} 
	//display winner
	cout<<getWinner(people,Deck,dp).getId();
 
 
	return 0;
}