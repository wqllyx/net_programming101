#include <iostream>
#include <string>
using namespace std;
int main()
{
    string s;
    s.resize(10);
    s.clear();
    cout << "请输入字符串" << endl;
    cin >> s;
    char white;
    cin >> std::noskipws >> white;
    cout << int(white) << endl;
    return 0;
}
