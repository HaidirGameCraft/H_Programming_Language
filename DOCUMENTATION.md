# DOCUMENTATION

## Data Type
- Containing Four Basic Data Type 'int, short, char, bool'
```
int number
bool isRun = 0
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

Extern from other source code;
```
extern func print( int textAddr ) -> void
```

## Control Structure
### Selection Control Structure
```
if ( condition )
    <Statement 1>
endif
```

### Dual Selection Control Structure
```
if ( condition )
    <Statement 1>
else
    <Statement 2>
endif
```

### Multiple Selection Control Structure
```
if ( condition )
    <Statement 1>
elif ( condition )
    <Statement 2>
else
    <Statement 3>
endif
```

### Repetition Control Structure
```
while ( condition )
    <Statement>
endwhile
```


