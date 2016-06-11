//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Dan Loman
// 6/4/2016
// Test jig for magic packet writing code to not need arudino

#include "fakeEEPROM.hpp"

#include <iostream>

using namespace std;

int main()
{
  extEEPROM eProm;
  cout << "starting to write" << endl;
  for (int i = 8; i > 0; i-=2)
  {
    auto temp = i;
    eProm.writeData(temp);
    cout << "wrote " << temp << endl;
    eProm.writeData(++temp);
    cout << "wrote " << temp << endl;
    eProm.writeData(++temp);
    cout << "wrote " << temp << endl;
    eProm.writeData(++temp);
    cout << "wrote " << temp << endl;
  }
  cout << "done writing" << endl;
  cout << "starting to read" << endl;

  int Value;

  for (int i = 18; i > 0; i-=2)
  {
    for (auto j = 0; j < 4; ++j)
    {
      if (eProm.readData(Value))
      {
        cout << Value;
      }
    }
  }
}

