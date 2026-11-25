# TaskDock

Console launcher for programs on Windows: feed input, capture stdout and stderr, and compare a reference with a solution.

The repository already has an example where you can check this without TaskDock itself.

```sh
./examples/pair-sum/check.sh
```

`cases/ok.in`: both answers are 2. `cases/fail.in`: the reference prints 3, the solution prints 2.
The generator with `--seed 42 --case 7 --size 6` produces the same mismatch. The other cases, 1–6 and 8, match at seed 42.
