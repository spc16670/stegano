#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <bitset>

using namespace std;

// global variables
vector< bitset<8> > bits;

// payload to vector of bitsets representing each character
void payloadToBits(string fileName) {
  string line;
  ifstream infile(fileName.c_str());
  while(getline(infile,line)) {
    for (size_t i = 0; i < line.size(); ++i)
    {
      bitset<8> bs = bitset<8>(line.c_str()[i]);
      bits.push_back(bs);
    }
  }
  infile.close();
}

// bits start from 1 and got to bits.size() * 8 inclusive
int getPayloadBit(unsigned int bit) {
  if (bit >= (bits.size() * 8)) {
    return -1;
  } else {
    int c = (int)(bit / 8); int b = bit % 8; 
    return (int)(bits[c][b]);
  }
}

// print bits
void printBits() {
  int n = 0;
  int b = getPayloadBit(n);
  while (b != -1) {
    if (n % 8 == 0) {
      printf("\n");
    }
    printf("%d",b);
    n++;
    b = getPayloadBit(n);
  }
  printf("\n");
}

void writeStegano(string cover,string stegano) {
  int bitIndex = 0;
  string line;
  ifstream infile(cover.c_str());
  ofstream outfile(stegano.c_str());
  while(getline(infile,line)) {
    int nextBit = getPayloadBit(bitIndex);
    bool written = false;
    if (nextBit == 1) {
      outfile << line << ' ' << endl;
    } else if (nextBit == -1 && written == false) {
      outfile << line << '\6' << endl;
      written = true;
    } else {
      outfile << line << endl;
    }
    bitIndex++;
  }
  infile.close();
  outfile.close();
}

void readStegano(string stegano) {
  vector< bitset<8> > bits;
  string line;
  char ch = ' ';
  string charBits ("");
  ifstream infile(stegano.c_str());
  while(getline(infile,line) && ch != '\6') {
    char ch = *line.rbegin();
    if (ch == ' ') {
      charBits = "1" + charBits;
    } else if (ch == '\6') {
      // end
    } else {
      charBits = "0" + charBits;
    }
    if (charBits.length() == 8) {
      bitset<8> bs (charBits);
      cout << char(bs.to_ulong());
      charBits = "";
    } 
  }
  infile.close();
}



int main(int argc,char* argv[]) {
  bool args = false;

  if (argv[1] != NULL) {
    if (strcmp(argv[1],"-w") == 0) {
      if (argv[2] != NULL) {
        if (strcmp(argv[2],"") != 0) {
          if (argv[3] != NULL) {
            if (strcmp(argv[3],"") != 0) {
              if (argv[4] != NULL) {
                if (strcmp(argv[4],"") != 0) {
                  args = true;
                  string cover = string(argv[2]);
                  string payload = string(argv[3]);
                  string stegano = string(argv[4]);
		  payloadToBits(payload);
  		  printBits(); 
  		  printf("\nWriting %s ... \n",stegano.c_str());
  		  writeStegano(cover,stegano);
                  printf("Done\n");
                }
              }
            }
          }
        }
      }
    } else if (strcmp(argv[1],"-r") == 0) {
      if (argv[2] != NULL) {
        if (strcmp(argv[2],"") != 0) {
          args = true;
          string stegano = string(argv[2]);
          printf("\nReading %s ... \n",stegano.c_str());
	  readStegano(stegano);
        }
      }
    }
  }

  if (!args) {
    if (argc > 0) {
      printf("%s [-w cover_file payload_file output_file | -r input_file ]",argv[0]);
    }
  }

  return 0;
}
