# Luis Felipe Loera A01571914
# Evidence 2 — Parallel Syntax Highlighter
# How to run:
#   iex parallel.ex
#   ParallelHighlighter.measure_time(&ParallelHighlighter.run/1, ["cpp_examples"])
# To measure multiple times and compute average manually:
#   times = Enum.map(1..5, fn _ -> ParallelHighlighter.measure_time(&ParallelHighlighter.run/1, ["cpp_examples"]) end)
#   avg = Enum.sum(times) / length(times)

defmodule ParallelHighlighter do
  @moduledoc """
  Tokenises C++ source files and renders them as syntax-highlighted HTML pages.
  """

  @keywords ~w(
    auto break case catch char class const continue default do double else enum
    float for if int long namespace private protected public return short signed
    sizeof static struct switch template this throw try typedef typename union
    unsigned using virtual void while override include define string cout cin endl
    std main
  )

  @token_rules [
    {:comment,      ~r/^\/\*[\s\S]*?\*\//},
    {:comment,      ~r/^\/\/.*/},
    {:preprocessor, ~r/^#.*/},
    {:string,       ~r/^"([^"\\]|\\.)*"/},
    {:char,         ~r/^'([^'\\]|\\.)*'/},
    {:number,       ~r/^\d+(?:\.\d+)?/},
    {:operator,     ~r/^(\+\+|--|==|!=|<=|>=|&&|\|\||<<|>>)/},
    {:operator,     ~r/^[+\-*\/%=<>!&|^~]/},
    {:delimiter,    ~r/^[;,\(\)\{\}\[\]]/},
    {:whitespace,   ~r/^\s+/},
    {:keyword,      ~r/^\b(?:#{Enum.join(@keywords, "|")})\b/},
    {:function,     ~r/^[a-zA-Z_][a-zA-Z0-9_]*(?=\s*\()/},
    {:constant,     ~r/^[A-Z][A-Z0-9_]+\b/},
    {:class_name,   ~r/^[A-Z][a-zA-Z0-9_]*\b/},
    {:identifier,   ~r/^[a-zA-Z_][a-zA-Z0-9_]*/},
    {:unknown,      ~r/^./}
  ]

  @cpp_extensions [".cpp", ".h", ".hpp", ".cc", ".cxx"]

  # Public API

  @doc """
  Highlights every C++ file in `input_dir` in parallel and writes each HTML
  file.
  Returns `{:ok, count}` with the number of files processed.
  """
  def run(input_dir) do
    files = list_cpp_files(input_dir)
    cores = System.schedulers_online()
    IO.puts("  Using #{cores} scheduler(s) (CPU cores)")

    files
    |> Task.async_stream(
      fn path -> process_file(path) end,
      max_concurrency: cores,
      ordered: false
    )
    |> Stream.run()

    {:ok, length(files)}
  end

  @doc "Tokenises `code` into a list of `{type, text}` tuples."
  def tokenize(code), do: do_tokenize(code, [])

  @doc "Generates a complete highlighted HTML document for the given C++ source."
  def generate_html(code) do
    body =
      code
      |> tokenize()
      |> Enum.map(&render_token/1)
      |> Enum.join("")

    """
    <!DOCTYPE html>
    <html lang="en">
    <head>
      <meta charset="UTF-8">
      <title>Syntax Highlight</title>
      <link rel="stylesheet" href="token_colors.css">
    </head>
    <body>
      <pre>#{body}</pre>
    </body>
    </html>
    """
  end

  @doc "Reads `input_file`, highlights it, and writes HTML to `output_file`."
  def save_html(input_file, output_file) do
    with {:ok, code} <- File.read(input_file),
         html        <- generate_html(code),
         :ok         <- File.write(output_file, html) do
      :ok
    else
      {:error, reason} -> {:error, reason}
    end
  end
  # Private helpers

  defp list_cpp_files(dir) do
    dir
    |> File.ls!()
    |> Enum.map(&Path.join(dir, &1))
    |> Enum.filter(fn path ->
      File.regular?(path) and Path.extname(path) in @cpp_extensions
    end)
  end

  defp process_file(input_path) do
    filename    = Path.basename(input_path)
    output_path = Path.rootname(input_path) <> ".html"
    result      = save_html(input_path, output_path)

    case result do
      :ok              -> IO.puts("  [OK]  #{filename}")
      {:error, reason} -> IO.puts("  [ERR] #{filename} — #{inspect(reason)}")
    end

    result
  end

  defp do_tokenize("", acc), do: Enum.reverse(acc)
  defp do_tokenize(remaining, acc) do
    {type, matched} = match_token(remaining)
    rest = String.slice(remaining, String.length(matched)..-1//1)
    do_tokenize(rest, [{type, matched} | acc])
  end

  defp match_token(str) do
    Enum.find_value(@token_rules, {:unknown, String.slice(str, 0, 1)}, fn {type, regex} ->
      case Regex.run(regex, str) do
        [matched | _] -> {type, matched}
        nil           -> nil
      end
    end)
  end

  defp render_token({:whitespace, text}), do: text
  defp render_token({type, text}),        do: ~s(<span class="#{type}">#{html_escape(text)}</span>)

  defp html_escape(text) do
    text
    |> String.replace("&", "&amp;")
    |> String.replace("<", "&lt;")
    |> String.replace(">", "&gt;")
    |> String.replace("\"", "&quot;")
  end
  @doc """
  Returns the time in seconds taken by `function` called with `parameters`.
  Usage: ParallelHighlighter.measure_time(&ParallelHighlighter.run/1, ["cpp_examples"])
  """
  def measure_time(function, parameters) do
    {time, _result} = :timer.tc(function, parameters)
    time / 1_000_000
  end
end

# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------
input_directory = "cpp_examples"

IO.puts("Parallel syntax highlighter")
IO.puts(String.duplicate("-", 40))

seconds = ParallelHighlighter.measure_time(&ParallelHighlighter.run/1, [input_directory])

IO.puts("\nDone")
IO.puts("Time: #{Float.round(seconds, 4)} seconds")
