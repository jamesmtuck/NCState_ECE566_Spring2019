#include <list>
#include <map>

int main()
{
  std::list<int> l;
  std::map<int, const char*> m;

  l.push_front(5);
  l.push_front(4);
  l.push_front(1);

  std::list<int>::iterator it;

  for(it=l.begin(); it!=l.end(); it++)
    printf("%d ", *it);


  m[0] = "zero";
  m[1] = "one";
  m[2] = "two";

  printf("%s \n", m[1]);

  if ( m.find(2) == m.end() ) {
    // 2 not present
  }
  

  return 0;
}
