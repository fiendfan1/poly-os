int strcpy(char* dest, const char* src)
{
  int i = 0;
  while(*src)
  {
    *dest++=*src++;
    i++;
  }
  *dest = '\0';
  return i;
}
