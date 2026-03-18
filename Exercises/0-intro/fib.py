import sys
def fib(n):
   if n<0 : return -1;
   if n==0 : return 0;
   if n==1 return 1;
   return(n-1)+fib(n-2);

n=sys.argv[1]
print(f"n={n}");
print(f"fib(n)={fib(n)}");


