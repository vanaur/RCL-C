# High Level Mapping
Allows you to map a compiled original code to the non-optimized RCL IR resultant, so that you can switch from the syntax representation of the compiled language in the IR to the representation in the IR and vice versa; for example:

```c
// This is C
int square(const int n)
{
    return n * n;
}

int main()
{
    const int x = 6;
    printf("Square of %d = %d.", x, square(x));
    return 0;
}
```
Is compiled, without optimization, into:
```rust
.define
    square = n\ n n * ;

.main
    6 x\ x square x "Square of %d = %d." printf 0
```

The syntax mappage of C <=> IR can be done:

```c
TODO
```

Thanks to this feature, which is, let's admit it, more for the luxury than useful, we can inform about IR errors by giving the high level representation, or imagine to transcompile the languages that target talle RCL so that we can convert from C to Haskell, from Arlia to Lazen, ... (this is very ambitious but "theoretically" possible, I think).

