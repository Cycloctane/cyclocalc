## cyclocalc

Simple mathematical expression evaluator based on Pratt Parsing.

Supported operators:

- Arithmetic: `+ - * / %`
- Bitwise: `| & ^ << >>`
- Parentheses: `( )`

### build

```bash
make cyclocalc
```

### usage

```bash
./cyclocalc "6 * (3 + 4)"
```

repl:

```
$ ./cyclocalc
(calc)> 6 * (3 + 4)
42
(calc)>
```
