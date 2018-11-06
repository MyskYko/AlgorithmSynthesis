#include <iostream>

#define N 16
#define M 4

using namespace std;

int main() {
  int i;
  int j;
  int k;
  int l;
  int h;

  for(i = 0; i < M; i++) {
    cout << ".inputs";
    for(j = 0; j < N/M; j++) {
      cout << " Is" << i+j*M+1;
    }
    
    cout << endl;
    
    cout << ".inputs";
    for(j = 0; j < N/M; j++) {
      for(l=0; l < N/M; l++) {
	for(h=0; h < M; h++) {
	  int tmp = 0;
	  if(l != N/M-1) {
	    tmp = i+l*M+1-h;
	  }
	  else {
	    tmp = i+l*M+1-h-1;
	  }
	  if(tmp >= l*M+1) {
	    cout << " w" << tmp << "_" <<i+j*M+1;
	  }
	  else {
	    cout << " w" << tmp + M<< "_" <<i+j*M+1;
	  }
	}
      }
    }
    cout << endl;
  }


}
