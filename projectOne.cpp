#include <iostream>

class Employee {
public:
 std::string Name;
 std::string Company;
 int Age;

 void IntroduceUrself(){

std::cout << "Name - " << Name << std::endl;
std::cout << "Company name - " << Company << std::endl;
std::cout << "Age - " << Age << std::endl;
 }
 }
;
int main()
{

Employee employee1;
employee1.Name = "saldina";
employee1.Company = "YT";
employee1.Age = 22;
employee1.IntroduceUrself();
}