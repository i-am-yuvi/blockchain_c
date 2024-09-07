#include "Blockchain.h"

int main()
{
     Blockchain bchain = Blockchain();

     int n;
     cout << "Enter the number of block you want to mine: ";
     cin >> n;

     for (int i = 1; i <= n; i++)
     {
          cout << "Mining block " << i << "..." << endl;
          bchain.AddBlock(Block(i, "Block Data"));
     }

     return 0;
}