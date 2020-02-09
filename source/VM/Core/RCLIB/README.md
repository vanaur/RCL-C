# The RCLIB (Runtime Core Libraries)

RCLIB is simply the "standard builtin library" of the RCL, usable by programs running on the RCL. The proposed functions can be used in any execution mode (i/c and s/l/o).

## Modules
There are different modules, each containing specific basic functions:

 - Math
 - String
   - String
   - Char
 - IO
 - File
   - File
   - Folder
 - Env
   - Direct
   - User
   - Computer
   - Plateform
 - Stack
   - Main
   - Separeted
 - Array
 - WEB
   - Backend
   - Frontend
   - Server
 - Misc
   - Misc
   - Thread
   - CFFI

## Functions
**Note:** All builtin functions can of course be implemented natively -- or not (depending on purity) -- but RCLIB allows to optimize their use as well as to have a common basis for any program for any platform for any language.

### Math

#### Trigonometry
```
sin(num) -> num
cos(num) -> num
tan(num) -> num
asin(num) -> num
acos(num) -> num
atan(num) -> num
toRad(num) -> num
toDeg(num) -> num
```
#### Logarithms
```
log(num) -> num
exp(num) -> num
```
#### Number modification / choice
```
round(num) -> num
min(num, num) -> num
max(num, num) -> num
abs(num) -> num
```
#### Roots
```
root(num, num) -> num
sqr(num) -> num
```
#### Convertions
```
stoi(string) -> num
ctoi(char) -> num
stof(string) -> num
ctof(char) -> num
```
#### Sum and product
```
sum({num}) -> num
product({num}) -> num
```
#### Others
```
seamesign(num, num) -> num
```

### String.String
