#include<string.h>

char* intToDec(int num)
{
  int val = num;
  int digits = 0;
  static char dec[16];
  if(num == 0)
  {
    dec[0]= '0';
    dec[1] = '\0';
    return dec;
  }

  while(val)
  {
    val /= 10;
    digits++;
  }
  int i = 0;
  if(num < 0)
  {
    dec[0]='-';
    i=1;
    num *= -1;
  }
  val = num;
  dec[i+digits]='\0';
  while(val)
  {
    dec[i+digits-1] = val % 10 + '0';
    digits--;
    val /=10;
  }
  return dec;
}

char* intToHex(unsigned num)
{
  unsigned flag = 0xf << 28;
  int shift = 28;
  static char hex[16];
  if(num == 0)
  {
    hex[0]='0';
    hex[1] = '\0';
    return hex;
  }
  while((flag & num) == 0)
  {
    shift -= 4;
    flag >>= 4;
  }
  int i = 0;
  while(flag)
  {
    unsigned val = flag & num;
    val >>= shift;
    if(val <= 9 && val >= 0)
      hex[i] = val + '0';
    else if(val > 9 && val <= 15)
      hex[i] = val - 10 + 'a';  
    else
    {
      return "";
    }
    i++;
    shift -=4;
    flag >>= 4;
  }
  hex[i] = '\0';
  return hex;

}

void sprintf(char *out, const char *fmt, ...) {
  void* arg = &fmt;
  arg += sizeof(fmt);
  while(*fmt)
  {
    if(*fmt == '%')
    {
      switch (*++fmt)
      {
        case 'c':
          *out =(char) *((int*)arg);
          arg += sizeof(int);
          break;
        case 'x':
          {
            int k = strcpy(out,intToHex(*((int*)arg)));
            out += k-1;
            arg += sizeof(int);
          }
          break;
        case 'd':
          {
            int k = strcpy(out,intToDec(*((int*)arg)));
            out += k-1;
            arg += sizeof(int);
          }
          break;
      }
    }
    else
    {
      *out = *fmt;
    }
    out++;
    fmt++;
  }
  *out='\0';
}
