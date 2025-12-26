# DOCUMENTATION

## Data Type
- Containing Four Basic Data Type 'int, short, char, bool'
```
int number
bool isRun = 0
```
- Can be array:
```
int number[] = [1, 2, 3, 4, 5, 6, 7, 8]
char message[] = "Hello, World"
```
- Can be declare without initializing
```
int number[5]
char message[16]
```


## Variable
- Outside of function it is constant Variable, which mean it is global Variable
- This variable cannot be has other variable that already declare into this variable
The right way:
```
int num1 = 3 * 2
int num2 = 2 + 3 * 5 * ( 4 - 2 )
```

The wrong way
```
int num1 = 3 * 4
int num2 = 3 * num1     // Wrong
```

## Function
Without Parameters:
```
func main() -> void
    [CODE_SEGMENT]
endfunc
```

With Parameters:
```
func add( int a, int b ) -> int
    return a + b
endfunc
```

## If-Elif-Else Statement
- On the end of this if-elif-else statement must be include 'endif'
```
if ( condition )
    <Statement 1>
elif ( condition )
    <Statement 2>
else
    <Statement 3>
endif
```


