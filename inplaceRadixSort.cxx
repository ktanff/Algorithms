template <typename T> void BlockSwap1(T *A,int p, int q) {
	int i,j;
	for(i=p,j=q;i<q;i++,j++) {
		T temp = A[i];
		A[i] = A[j];
		A[j] = temp;
	}
}

#include <cstring>
template <typename T> void BlockSwap(T *A,int p, int q) {
	static const int chnk_size=1024;
	static const int tmp_size=chnk_size*sizeof(T);
	static char temp[tmp_size];
	char *x=(char *)(A+p);
	char *y=(char *)(A+q);
	int i;
	for(i=p+chnk_size;i<q;i+=chnk_size) {
		memcpy(temp,x,tmp_size);
		memcpy(x,y,tmp_size);
		memcpy(y,temp,tmp_size);
		x+=tmp_size;
		y+=tmp_size;
	}
	int rem_size=tmp_size-(i-q)*sizeof(T);
	memcpy(temp,x,rem_size);
	memcpy(x,y,rem_size);
	memcpy(y,temp,rem_size);
}

template <typename T> int StableRBA(T *A,int p,int q,int r) {
	int n0 = r-q;
	int n1= q-p;
	if( n1==0 ) return r;
	while(n0>0) {
		if( n1>n0) {
			p=q-n0;
			BlockSwap(A,p,q);
			n1=n1-n0;
			q=p;
			p=p-n1;
		}
		else {
			BlockSwap(A,p,q);
			n0=n0-n1;
			p=q;
			q=q+n1;
		}
	}
	return p;
}

template <typename T> int BinaryStableSort(T *A,int p,int r, bool (*key)(const T&) ) {
	if(r-p>1) {
		int q= (p+r)/2;
		p=BinaryStableSort(A,p,q,key);
		r=BinaryStableSort(A,q,r,key);
		p=StableRBA(A,p,q,r);
	}
	else if(!key(A[p])) return r;
	return p;
}


#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class aclass{
	int k;
	char satellite;
public:
    aclass(){}
	aclass(int key, char data){
		k=key;
		satellite=data;
	}
	bool key() const{ return k; }
	void print(){
		cout<<" ("<<k<<","<<satellite<<")";
	}
};

bool aclasskey(const aclass& a){ return a.key(); }

int mask;
bool radixkey(const int& x){ return x&mask; }

void inplaceRadixSort(int *x, int n){
	 mask=1;
	 while(mask){
	 	BinaryStableSort(x,0,n,radixkey);	 	
	 	mask<<=1;
	 }
}

int main(int argc, char *argv[]) {
	aclass A[26];
	int i;
	for(i=0;i<26;i++) {
		A[i]=aclass(rand()%2,i+97);
	}
	
	for(i=0;i<26;i++) A[i].print();
	 cout<<endl;
	 
	 BinaryStableSort(A,0,26,aclasskey);
	 
	for(i=0;i<26;i++) A[i].print();
	 cout<<endl;
	 
	 int n;
	 for(n=10000;n<=1010000;n+=100000){
		 //cout <<"n="<<n<<endl;
		 int *x=new int(n);
		 for(i=0;i<n;i++) x[i]= (n<=100 ? rand()%1000:rand());
		 //for(i=0;i<n;i++) x[i]=rand()%50000;
		 
		 clock_t start=clock();
		 inplaceRadixSort(x,n);
		 //sort(x,x+n);
		 //stable_sort(x,x+n);
		 clock_t end=clock();
		 
		 if(n<=100) for(i=0;i<n;i++) cout<<x[i]<<' ';
		 // cout<<endl;
		 if(!is_sorted(x,x+n))
		 	cout<<"Fail"<<endl;
		 //for(i=1;i<n;i++)
		 //  if(x[i]<x[i-1]){
		 //    cout<<"Fail"<<endl;
		 //    break;
		 //  }
		 // if(i==n) cout<<"OK"<<endl;
		 // cout<<"Elapsed time = "<< float(end-start)/CLOCKS_PER_SEC << " seconds." << endl;
		 cout<<n<<"\t"<< float(end-start)/CLOCKS_PER_SEC<< endl;
		 delete x;
	}
}