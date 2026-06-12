# Evidence 2 — Parallel Syntax Highlighter

**Luis Felipe Loera — A01571914**

C++ syntax highlighter that processes multiple source files sequentially and in parallel, generating HTML output with token-based color highlighting.

---

## Project Structure

```
lexer/
├── cpp_examples/               # Input .cpp files
├── cpp_examples/               # HTML output 
├── sequential.ex               # Sequential highlighter
├── parallel.ex                 # Parallel highlighter
├── token_colors.css            # Token color styles its inside cpp_examples
└── README.md
```


## Usage

### Sequential

```bash
elixir sequential.ex
```

### Parallel

```bash
elixir parallel.ex
```

Both versions read from `cpp_examples/`.
### Measure execution time (in iex)

```bash
iex sequential.ex
```
```elixir
SequentialHighlighter.measure_time(&SequentialHighlighter.run/1, ["cpp_examples"])
```

```bash
iex parallel.ex
```
```elixir
ParallelHighlighter.measure_time(&ParallelHighlighter.run/1, ["cpp_examples"])
```

### Multiple runs and average

```elixir
times = Enum.map(1..5, fn _ -> SequentialHighlighter.measure_time(&SequentialHighlighter.run/1, ["cpp_examples"]) end)
avg = Enum.sum(times) / length(times)

times = Enum.map(1..5, fn _ -> ParallelHighlighter.measure_time(&ParallelHighlighter.run/1, ["cpp_examples"]) end)
avg = Enum.sum(times) / length(times)
```
´´´
times = Enum.map(1..5, fn _ -> SequentialHighlighter.measure_time(&SequentialHighlighter.run/1, ["cpp_examples"]) end)
this command can also be 1..6, 1..7, etc.

times = Enum.map(1..5, fn _ -> ParallelHighlighter.measure_time(&ParallelHighlighter.run/1, ["cpp_examples"]) end)
avg = Enum.sum(times) / length(times)
this command can also be 1..6, 1..7, etc.
´´´
---

## Token Types

| Token | Color | Example |
|---|---|---|
| `comment` | Green | `// comment`, `/* block */` |
| `preprocessor` | Purple | `#include`, `#define` |
| `keyword` | Blue | `int`, `return`, `class` |
| `string` | Orange | `"hello"` |
| `char` | Orange | `'a'` |
| `number` | Light green | `42`, `3.14` |
| `operator` | Default | `+`, `==`, `<<` |
| `delimiter` | Grey | `;`, `{`, `}` |
| `function` | Yellow | `main(`, `sort(` |
| `class_name` | Teal | `TreeNode`, `BinarySearchTree` |
| `constant` | Bright blue | `MAX_SIZE`, `NULL` |
| `identifier` | Light blue | `result`, `temp` |

---

## Implementation

### 1. Sequential version (`sequential.ex`)

Processes each file one at a time using `Enum.each`:

```elixir
def run(input_dir) do
  files = list_cpp_files(input_dir)
  Enum.each(files, fn path -> process_file(path) end)
  {:ok, length(files)}
end
```

### 2. Parallel version (`parallel.ex`)

Processes files concurrently using `Task.async_stream` with one task per logical CPU core:

```elixir
def run(input_dir) do
  files = list_cpp_files(input_dir)
  cores = System.schedulers_online()
  files
  |> Task.async_stream(fn path -> process_file(path) end,
      max_concurrency: cores, ordered: false)
  |> Stream.run()
  {:ok, length(files)}
end
```

Each file is independent — no shared state between tasks — making this an embarrassingly parallel workload. `ordered: false` allows faster files to release their core immediately.

### 3. Coding conventions

Both files follow Elixir conventions:
- `snake_case` for functions and variables
- Module attributes (`@keywords`, `@token_rules`, `@cpp_extensions`) for compile-time constants
- Pattern matching and the pipe operator `|>` for data transformations
- Private helpers marked with `defp`
- `@doc` strings on all public functions

### 4. Execution time measurements

Run both versions 5 times in `iex` and record the results:

| Run | Sequential (s) | Parallel (s) |
|-----|---------------|-------------|
| 1   |0.835084       |0.282454     |
| 2   |1.033866       |0.313571     |
| 3   |0.85035        |0.353907     |
| 4   |0.858183       |0.381622     |
| 5   |0.907707       |0.358292     |
| AVG |0.816216       |0.3379692

```
Speedup = Sequential average / Parallel average

2.42x
```

### 5. Proposed solution and algorithms

The tokeniser is a **greedy linear scanner**. At each position in the source string it tries 16 regex rules in order and advances past the first match:

```
do_tokenize(remaining, acc)
  ├─ base case  : remaining == "" → reverse(acc)
  └─ recursive  : match_token(remaining) → {type, text}
                  rest = remaining[length(text)..]
                  do_tokenize(rest, [{type, text} | acc])
```

Rules are ordered from most specific (block comments, string literals) to least specific (identifiers, unknown fallback), so the most common tokens are matched early. Because each match advances the cursor by at least one character, the scanner always makes progress and terminates.

The parallel version does not change the algorithm at all — it simply runs one independent instance of the tokeniser per file on a separate Erlang process.

### 6. Time complexity

**Per-file cost:**

| Step | Cost |
|---|---|
| Read file | O(n) |
| Tokenise (n chars, k=16 rules) | O(n · k) = O(n) |
| Render tokens to HTML | O(n) |
| Write file | O(n) |
| **Total per file** | **O(n)** |

**Sequential — F files of average length n̄:**

```
T_seq = F · O(n̄) = O(F · n̄)
```

**Parallel — F files, P cores:**

```
T_par ≈ ⌈F / P⌉ · O(n̄) = O(F · n̄ / P)
```

**Theoretical speedup:**

```
Speedup = T_seq / T_par ≈ P
```

This matches the measured results. Because each file is processed independently with no synchronisation, the workload scales linearly with the number of cores — limited in practice by I/O contention and process spawn overhead.

### 7. Complexity analysis and ethical reflection

#### Findings

The tokeniser achieves O(n) per file because the number of regex rules k is a fixed constant (16). Doubling the size of a source file doubles the processing time — no worse. The sequential version therefore scales as O(F · n̄) with the total input size, and the parallel version reduces wall-clock time by a factor close to P (the number of CPU cores).

The measured speedup is slightly below P due to:
- **Process spawn overhead** — each `Task.async_stream` task is an Erlang process
- **I/O saturation** — multiple tasks reading/writing the same disk simultaneously
- **Small input penalty** — when files are small, fixed overhead dominates and speedup shrinks toward 1×

For large codebases (hundreds of files, thousands of lines each) the parallel version provides a meaningful real-world speedup that grows with hardware.

#### Ethical implications

**Accessibility.** Syntax highlighting reduces the cognitive load of reading code. Providing this as a local, open tool means developers in low-resource environments — students, educators, indie developers — get the same quality of tooling as those at well-funded companies, without depending on expensive IDEs or cloud services.

**Privacy.** Because the highlighter runs entirely locally with no network calls, source code never leaves the user's machine. This matters when working with proprietary algorithms, unreleased products, or sensitive business logic. Tools that require uploading code to a remote service for processing create legal and privacy risks that a local tool avoids.

**Energy consumption.** Parallel execution consumes more CPU power simultaneously. On shared infrastructure a parallel job that saturates all cores can starve other workloads and increase total energy use. Developers should choose the degree of parallelism appropriate to the context — `System.schedulers_online()` is a sensible default, but workloads running in shared environments may benefit from an explicit concurrency cap.

**Democratisation of computing.** Multi-core parallelism, once the domain of supercomputing, is now available on every laptop. Tools that take advantage of this compress time that would otherwise be wasted. For educators processing hundreds of student submissions, or maintainers running documentation pipelines, these gains have real productivity value.

**Determinism.** The parallel version uses `ordered: false`, meaning HTML files are written in non-deterministic order. For a syntax highlighter this is benign, but in contexts where output order carries meaning — audit trails, reproducible builds — non-determinism introduces subtle risks that engineers must consciously address.
