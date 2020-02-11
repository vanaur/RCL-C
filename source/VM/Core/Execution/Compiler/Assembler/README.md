# Compiling RCL IR to assembly

## Compiling RCL IR into efficient assembly code using continuation passing style

- $\epsilon$ is a memory location, a register.
- $\omega$ is a value (number, string, object, ...).
- $[.]$ is an adress, a pointer.
- $[\epsilon-(k \times \omega)]$ is therefore the adress of a value above $k$ number of other values of the same type of $\omega$, above the location $0$ of $\epsilon$.
- $\Mu = \Delta\langle\Epsilon,k\rangle$ is a dictionnary between registers ($\epsilon$) and their own context deep ($k$).
- $\Gamma = \Delta\langle\Mu,\Omega\rangle$ is the environment register and value (values are not needed to be explicite ( $\theta$ )).
- Basics assembly instructions :
    - $push$
    - $pop$
    - $mov$
    - $...$
- $\rho$(x) = arity of x
- $\alpha$ is the register ($\epsilon$) for function parameter.
- $\beta$ is the register ($\epsilon$) for the returned value of a function.

In the compilation pass of a RCL IR program, operations are read in the opposite side (from right to left). This will allow on the one hand to better explore and compile the lazy evaluation (indeed we will then only need on the stack what is relevant to the current flow) and on the other hand, it will allow to consider any call as a continuation (in the CPS form) of the previous operations (up to the first position). For example, in the following program: `3 2 square +` (supposing it is valid and correctly type checked) we will read as:
- **`+`** take 2 numbers and produce one ($+:(\Z\to\Z)\to\Z$);
- **`square`** take 1 number and produce one ($square:\Z\to\Z$);
- **`2`** take 0 object and produce one number ($2 : \Z$);
- **`3`** take 0 objets and produce one number ($3 : \Z$);

`+` is the continuation of `square` being theirself the continuation of `2` being all of them the continuation of `3`. So, we have formed a graph, with `3` as the root parameter for a CPS form:

![](iugiusdhf.png)

showed in another way:

![](oidf.png)

In terms of continuation passing style, we are computing each time only one argument to be passed to another function being the continuation and so one until we reach the final result. This case of simple addition with function call is really trivial in practice and is of course managed easier, but this example will give us a first simple approach to understand the algorithm.

## The simple algorithm

We read the IR from right to left, having as first information the arity of the operation being processed. Depending on the arity, we will reserve registers taking into account the size of each expected argument. If the size is variable, we will use a pointer (explained after). Still in the same instance of the operation being processed, we now check its trace on the IR stack (what it returns) and reserve registers accordingly, in the environment. For example, with `7 dup *` :
1) $\rho$(`*`) = `2`. </br>
  Type = $\Z . \Z \to \Z$. </br>
  Reserve 2 registers of size $\Z$ for input. </br>
  Reserve 1 register of size $\Z$ for output. </br>
2) $\rho$(`dup`) = `1`. </br>
  Type = $a \to a.a$. </br>
  Reserve 1 register of size $a$ for input. </br>
  Reserve 2 registers of size $a$ for output. </br>
  $\implies$ Infers that the output of (2) = input arity of (1). </br>
  $\implies$ Type of $a$ = $\Z$. </br>
  $\implies$ Registers for (2) output match with registers for (1) input.
3) $\rho$(`7`) = `0`. </br>
   Type = $() \to Z$. </br>
   Put the value into a register. </br>
   $\implies$ Infers that the output of (3) = input arity of (2) </br> $\implies$ Register for (3) = register for input of (2).

Resulting:

```
mov     [eax - 4], 7            ; 7
mov     [eax - 8], [eax - 4]    ; dup
mul     [eax - 8], [eax - 4]    ; *
push    [eax - 8]
pop     eax
```

Beyond the optimizations that the IR could receive, this generated code is not the most efficient, but has the advantage of being universally compatible with all IR constructs. However, we will not stop at this code and will not bring the rest of the optimizations. During a JIT, optimizations will only be done on the hot parts of the program, in order not to lose too much time before execution.

## The extended algorithm
The extended algorithm attacks the quotes, compiling them into separate labels.



___

```
.define
    square = dup * ;

.main
    2 [square] apply
```

```
def_Main_square
:   `*`
     > ω is infered `Num`
     > k is infered `2`
     > ε is infered `Alpha (= ESP)`
        -> will use [ε - k * ω] == [ESP - Num * 2]
        -> will produce [ε - k * ω] == [ESP - Num * 1]
    `dup`
     > ω is infered `Num`
     > k is infered 1
     > ε is infered `Alpha (= ESP)`
        -> will use [ε - k * ω] == [ESP - Num * 1]
        -> will produce [ε - k * ω] == [ESP - Num * 2]
=>
    push    ESP
    mov     ESP, EBP
    mov     ESP, EDI
    pop     EDI
    pop     [ESP - 4]
    pop     [ESP - 8]
    mul     [ESP - 4], [ESP - 8]
    mov     EAX, [ESP - 4]
    mov     EBP, ESP
    pop     ESP
    ret
```

```haskell
.define
    square = dup * ;

.main
    2 [square] apply

___________________________

.def_Main_square

    // Infos on `*`
    -> will use [EDI - SIZEOF(Num) * 1]
    -> will use [EDI - SIZEOF(Num) * 2]
    -> will produce [EDI - SIZEOF(Num) * 1]

    => Need 2 * (ω = Num)
    => Need REGISTERY (ε = EDI)

    // Infos on `dup`
    -> will use [ε - SIZEOF(ω) * 1]
    -> will produce [ε - SIZEOF(ω) * 2]

    => Since `dup` is the first operation of a function:
       Since `dup` will 
```