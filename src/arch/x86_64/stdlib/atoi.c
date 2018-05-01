#include <kernel.h>

/*
converts a string to an integer
*/
int atoi(char * str) {
  int value = 0;

  while (isspace(*str)){
    str++;
  }

  while (isdigit(*str)) 
  {
    value *= 10;
    value += (int) (*str - '0');
    str++;
  }

  return value;
}

/*
converts a string to an integer. the string can contain a hexadecimal number
*/
int atoi_hex(char * str) {
  int value = 0;
  int sign = 1;

  while (isspace(*str)){
    str++;
  }

  if( *str == '+' || *str == '-' )
  {
    if( *str == '-' ){
      sign = -1;
    }
    str++;
  }else if(isdigit(*str) || (*str >= 'a' && *str <= 'f') || (*str >= 'A' && *str <= 'F')){
    //continue
  }else{
    return 0;
  }

  while (isdigit(*str) || (*str >= 'a' && *str <= 'f') || (*str >= 'A' && *str <= 'F')) 
  {
    value *= 16;
    if(isdigit(*str)){
      value += (int) (*str - '0');
    }else if(*str >= 'a' && *str <= 'f'){
      value += (int) (*str - 'a') + 10;
    }else if(*str >= 'A' && *str <= 'F'){
      value += (int) (*str - 'A') + 10;
    }
    str++;
  }

  return value * sign;
}